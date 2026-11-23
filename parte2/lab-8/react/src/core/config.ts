import type { Tcb } from './types';

// Parâmetros do enunciado do mini-projeto.
export const PAGE_SIZE = 1024;
export const OFFSET_BITS = 10;
export const PAGE_BITS = 2;
export const ADDRESS_BITS = PAGE_BITS + OFFSET_BITS;
export const PAGES_PER_PROCESS = 4;
export const NUM_FRAMES = 16;
export const PT_ENTRY_SIZE = 4; // bytes por entrada da tabela de páginas
export const SWAP_FIRST_BLOCK = 16;
export const SWAP_BLOCKS = 8;

export const MESSAGES = {
  PAGE_FAULT_NAL: 'Page Fault – página ainda não alocada',
  PAGE_FAULT_SWAP: 'Page Fault – página na área de swap',
  PROTECTION: 'Violação de proteção – área reservada ao sistema',
  OUT_OF_RANGE: 'Endereço fora do espaço de endereçamento do processo',
  SWAP_FULL: 'Área de swap cheia – não é possível liberar o quadro',
};

// Cada processo tem 4 variáveis, uma em cada página.
export const PROCESSES: Tcb[] = [
  {
    pid: 1, name: 'P1', ptbr: 0x0000, color: '#d9480f',
    variables: [
      { name: 'a', page: 0, offset: 0x010 },
      { name: 'b', page: 1, offset: 0x0a4 },
      { name: 'c', page: 2, offset: 0x1f0 },
      { name: 'd', page: 3, offset: 0x2c8 },
    ],
  },
  {
    pid: 2, name: 'P2', ptbr: 0x0100, color: '#1971c2',
    variables: [
      { name: 'a', page: 0, offset: 0x020 },
      { name: 'b', page: 1, offset: 0x0b8 },
      { name: 'c', page: 2, offset: 0x1a0 },
      { name: 'd', page: 3, offset: 0x3e0 },
    ],
  },
  {
    pid: 3, name: 'P3', ptbr: 0x0200, color: '#2f9e44',
    variables: [
      { name: 'a', page: 0, offset: 0x004 },
      { name: 'b', page: 1, offset: 0x0c0 },
      { name: 'c', page: 2, offset: 0x234 },
      { name: 'd', page: 3, offset: 0x3f0 },
    ],
  },
];
