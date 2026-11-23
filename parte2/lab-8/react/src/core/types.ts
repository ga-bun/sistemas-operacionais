export type PageState = 'NAL' | 'RAM' | 'SWAP';

// Entrada da tabela de páginas (Maziero, cap. 14): estado + localização (quadro ou bloco de swap).
export interface PageTableEntry {
  state: PageState;
  location: number | null;
}

export interface PageTableRow extends PageTableEntry {
  page: number;
}

// 'tables' = quadro 0 | 'kernel' = SO | 'user' = páginas dos processos | 'reserved' = SO (indisponível)
export type FrameArea = 'tables' | 'kernel' | 'user' | 'reserved';

export interface Frame {
  index: number;
  area: FrameArea;
  owner: number | null; // pid do dono da página
  page: number | null;
}

export interface SwapBlock {
  index: number; // 16 a 23
  owner: number | null;
  page: number | null;
}

// Só usado para sugerir vítimas (FIFO = loadedAt, LRU = lastUse).
export interface FrameMeta {
  loadedAt: number;
  lastUse: number;
}

export interface MemoryState {
  frame0: (PageTableEntry | null)[]; // conteúdo do quadro 0: entradas de 4 bytes
  frames: Frame[];
  swap: SwapBlock[];
  meta: Record<number, FrameMeta>;
  clock: number;
}

export interface Variable {
  name: string;
  page: number;
  offset: number; // deslocamento dentro da página
}

// Task Control Block: guarda o PTBR do processo.
export interface Tcb {
  pid: number;
  name: string;
  ptbr: number; // offset da tabela de páginas dentro do quadro 0
  color: string;
  variables: Variable[];
}

export type RegisterKey = 'ptbr' | 'logical' | 'page' | 'offset' | 'pte' | 'frame' | 'physical';
export type StepKind = 'normal' | 'fault' | 'error' | 'success';

export interface TranslationStep {
  title: string;
  lines: string[];
  detail: string;
  kind: StepKind;
  flashFrame: number | null; // quadro que pisca neste passo
  regs: Partial<Record<RegisterKey, string>>;
  addressBits: { value: number; pageBits: number; totalBits: number } | null;
  tablePage: number | null; // linha da tabela de páginas a destacar
}

export type TranslationStatus = 'OK' | 'PAGE_FAULT_NAL' | 'PAGE_FAULT_SWAP' | 'PROTECTION' | 'OUT_OF_RANGE';

export interface Translation {
  status: TranslationStatus;
  message: string;
  logicalAddress: number;
  page: number;
  offset: number;
  entryOffset: number | null;
  entry: PageTableEntry | null;
  frame: number | null;
  physicalAddress: number | null;
  steps: TranslationStep[];
}
