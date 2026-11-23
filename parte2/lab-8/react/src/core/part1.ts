import { PROCESSES } from './config';
import { createMemory, cloneMemory, entryOffset, writeEntry } from './memory';
import type { MemoryState } from './types';

// Mapeamento fixo do enunciado: quadro de cada página (pág 0..3) de P1, P2 e P3.
const INITIAL_FRAMES: Record<number, number[]> = {
  1: [5, 8, 9, 11],
  2: [1, 2, 12, 13],
  3: [3, 4, 14, 15],
};

// Parte 1: quadro 0 guarda as tabelas; todos os outros quadros são de páginas dos processos.
export function createPart1Memory(): MemoryState {
  const memory = createMemory((frame) => (frame === 0 ? 'tables' : 'user'));

  PROCESSES.forEach((tcb) => {
    INITIAL_FRAMES[tcb.pid].forEach((frame, page) => {
      writeEntry(memory, entryOffset(tcb.ptbr, page), { state: 'RAM', location: frame });
      memory.frames[frame].owner = tcb.pid;
      memory.frames[frame].page = page;
    });
  });

  return cloneMemory(memory);
}
