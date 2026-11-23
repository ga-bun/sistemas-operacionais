import { NUM_FRAMES, PAGES_PER_PROCESS, PAGE_SIZE, PT_ENTRY_SIZE, SWAP_BLOCKS, SWAP_FIRST_BLOCK } from './config';
import type { FrameArea, MemoryState, PageTableEntry, PageTableRow, Tcb } from './types';

export function createMemory(areaOf: (frame: number) => FrameArea): MemoryState {
  return {
    frame0: new Array<PageTableEntry | null>(PAGE_SIZE / PT_ENTRY_SIZE).fill(null),
    frames: Array.from({ length: NUM_FRAMES }, (_, index) => ({
      index,
      area: areaOf(index),
      owner: null,
      page: null,
    })),
    swap: Array.from({ length: SWAP_BLOCKS }, (_, i) => ({
      index: SWAP_FIRST_BLOCK + i,
      owner: null,
      page: null,
    })),
    meta: {},
    clock: 0,
  };
}

// O estado é copiado antes de cada alteração, assim o React percebe a mudança.
export function cloneMemory(memory: MemoryState): MemoryState {
  return structuredClone(memory);
}

// Endereço (dentro do quadro 0) da entrada de uma página: PTBR + página × tamanho da entrada.
export function entryOffset(ptbr: number, page: number): number {
  return ptbr + page * PT_ENTRY_SIZE;
}

export function readEntry(memory: MemoryState, offset: number): PageTableEntry | null {
  return memory.frame0[offset / PT_ENTRY_SIZE] ?? null;
}

// Altera o objeto recebido: chame só em uma cópia (cloneMemory).
export function writeEntry(memory: MemoryState, offset: number, entry: PageTableEntry): void {
  memory.frame0[offset / PT_ENTRY_SIZE] = entry;
}

export function readTable(memory: MemoryState, tcb: Tcb): PageTableRow[] {
  const rows: PageTableRow[] = [];
  for (let page = 0; page < PAGES_PER_PROCESS; page++) {
    const entry = readEntry(memory, entryOffset(tcb.ptbr, page));
    rows.push({ page, state: entry ? entry.state : 'NAL', location: entry ? entry.location : null });
  }
  return rows;
}
