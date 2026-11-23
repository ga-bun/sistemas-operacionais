import { MESSAGES, PAGES_PER_PROCESS, PROCESSES } from './config';
import { cloneMemory, createMemory, entryOffset, writeEntry } from './memory';
import type { FrameArea, MemoryState, Tcb, Translation } from './types';
import { frameStart } from './utils';

// Parte 2: papel do sistema operacional na paginação sob demanda (Maziero, cap. 17).

export interface PendingFault {
  pid: number;
  page: number;
  entryOffset: number;
  state: 'NAL' | 'SWAP';
  swapBlock: number | null;
}

export interface Candidate {
  frame: number;
  free: boolean;
  owner: number | null;
  page: number | null;
  start: number;
}

export type ResolveResult =
  | { ok: true; memory: MemoryState; events: string[] }
  | { ok: false; message: string };

// 0: tabelas | 1–3: kernel | 4–11: usuário | 12–15: reservado
function areaOf(frame: number): FrameArea {
  if (frame === 0) return 'tables';
  if (frame <= 3) return 'kernel';
  if (frame <= 11) return 'user';
  return 'reserved';
}

// Todas as páginas de todos os processos começam NAL.
export function createPart2Memory(): MemoryState {
  const memory = createMemory(areaOf);
  PROCESSES.forEach((tcb) => {
    for (let page = 0; page < PAGES_PER_PROCESS; page++) {
      writeEntry(memory, entryOffset(tcb.ptbr, page), { state: 'NAL', location: null });
    }
  });
  return cloneMemory(memory);
}

// Registra um acesso (relógio lógico); se a página estava na RAM, marca o quadro como usado agora.
export function registerAccess(memory: MemoryState, frame: number | null): MemoryState {
  const next = cloneMemory(memory);
  next.clock += 1;
  if (frame !== null) {
    next.meta[frame].lastUse = next.clock;
  }
  return next;
}

export function createFault(tcb: Tcb, translation: Translation): PendingFault | null {
  const { status, entry, entryOffset: offset } = translation;
  if (status !== 'PAGE_FAULT_NAL' && status !== 'PAGE_FAULT_SWAP') return null;
  if (entry === null || offset === null) return null;

  return {
    pid: tcb.pid,
    page: translation.page,
    entryOffset: offset,
    state: status === 'PAGE_FAULT_SWAP' ? 'SWAP' : 'NAL',
    swapBlock: status === 'PAGE_FAULT_SWAP' ? entry.location : null,
  };
}

export function listCandidates(memory: MemoryState): Candidate[] {
  return memory.frames
    .filter((frame) => frame.area === 'user')
    .map((frame) => ({
      frame: frame.index,
      free: frame.owner === null,
      owner: frame.owner,
      page: frame.page,
      start: frameStart(frame.index),
    }));
}

// Só há sugestão quando a RAM está cheia: FIFO = quadro mais antigo, LRU = menos usado recentemente.
export function suggestVictims(memory: MemoryState): { fifo: number; lru: number } | null {
  const occupied = listCandidates(memory).filter((c) => !c.free);
  if (occupied.length === 0 || occupied.length < listCandidates(memory).length) return null;

  let fifo = occupied[0].frame;
  let lru = occupied[0].frame;
  occupied.forEach((c) => {
    if (memory.meta[c.frame].loadedAt < memory.meta[fifo].loadedAt) fifo = c.frame;
    if (memory.meta[c.frame].lastUse < memory.meta[lru].lastUse) lru = c.frame;
  });
  return { fifo, lru };
}

// Resolve o page fault usando o quadro escolhido pelo aluno.
export function resolveFault(memory: MemoryState, fault: PendingFault, frameIndex: number): ResolveResult {
  const target = memory.frames[frameIndex];

  if (target.area !== 'user') {
    return { ok: false, message: MESSAGES.PROTECTION };
  }

  const next = cloneMemory(memory);
  const events: string[] = [];

  // Quadro ocupado: a página que está nele (a vítima) vai para um bloco livre do swap.
  if (target.owner !== null) {
    const block = next.swap.find((b) => b.owner === null);
    if (!block) {
      return { ok: false, message: MESSAGES.SWAP_FULL };
    }
    const victim = PROCESSES.find((p) => p.pid === target.owner) as Tcb;
    const victimPage = target.page as number;

    block.owner = victim.pid;
    block.page = victimPage;
    writeEntry(next, entryOffset(victim.ptbr, victimPage), { state: 'SWAP', location: block.index });
    events.push(`Substituição: ${victim.name} pág ${victimPage} sai do quadro ${frameIndex} para o bloco de swap ${block.index}.`);
  }

  const tcb = PROCESSES.find((p) => p.pid === fault.pid) as Tcb;

  if (fault.state === 'SWAP' && fault.swapBlock !== null) {
    const origin = next.swap.find((b) => b.index === fault.swapBlock);
    if (origin) {
      origin.owner = null;
      origin.page = null;
    }
    events.push(`Swap-in: ${tcb.name} pág ${fault.page} vem do bloco ${fault.swapBlock} para o quadro ${frameIndex}.`);
  } else {
    events.push(`Alocação: ${tcb.name} pág ${fault.page} alocada no quadro ${frameIndex}.`);
  }

  next.frames[frameIndex].owner = fault.pid;
  next.frames[frameIndex].page = fault.page;
  next.meta[frameIndex] = { loadedAt: next.clock, lastUse: next.clock };
  writeEntry(next, fault.entryOffset, { state: 'RAM', location: frameIndex });

  return { ok: true, memory: next, events };
}
