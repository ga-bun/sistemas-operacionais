import { ADDRESS_BITS, MESSAGES, OFFSET_BITS, PAGES_PER_PROCESS, PAGE_BITS, PAGE_SIZE, PT_ENTRY_SIZE } from './config';
import { entryOffset, readEntry } from './memory';
import type { MemoryState, Tcb, Translation, TranslationStep } from './types';
import { bin, hex } from './utils';

function makeStep(title: string, options: Partial<TranslationStep> = {}): TranslationStep {
  return {
    title,
    lines: [],
    detail: '',
    kind: 'normal',
    flashFrame: null,
    regs: {},
    addressBits: null,
    tablePage: null,
    ...options,
  };
}

// Passos 4 e 5: interpreta a entrada lida e, se a página está na RAM, calcula o endereço físico.
function describeEntry(memory: MemoryState, result: Translation): void {
  const { page, offset, entry } = result;
  if (entry === null) return;

  if (entry.state === 'NAL') {
    result.status = 'PAGE_FAULT_NAL';
    result.message = MESSAGES.PAGE_FAULT_NAL;
    result.steps.push(makeStep('Entrada lida: página não alocada', {
      lines: [`entrada[${page}] = { estado: NAL, localização: – }`],
      detail: result.message,
      kind: 'fault',
    }));
    return;
  }

  if (entry.state === 'SWAP') {
    result.status = 'PAGE_FAULT_SWAP';
    result.message = MESSAGES.PAGE_FAULT_SWAP;
    result.steps.push(makeStep('Entrada lida: página na área de swap', {
      lines: [`entrada[${page}] = { estado: SWAP, bloco: ${entry.location} }`],
      detail: result.message,
      kind: 'fault',
    }));
    return;
  }

  const frame = entry.location as number;
  result.frame = frame;

  // Frames 0–3 e 12–15 (Parte 2) pertencem ao SO: processos não podem mapeá-los.
  if (memory.frames[frame].area !== 'user') {
    result.status = 'PROTECTION';
    result.message = MESSAGES.PROTECTION;
    result.steps.push(makeStep('Quadro reservado ao sistema', {
      lines: [`entrada[${page}] → quadro ${frame}`],
      detail: result.message,
      kind: 'error',
    }));
    return;
  }

  result.steps.push(makeStep('Quadro físico correspondente', {
    lines: [`entrada[${page}] = { estado: RAM, quadro: ${frame} }`],
    detail: `A página ${page} está no quadro ${frame}.`,
    regs: { frame: String(frame) },
  }));

  result.physicalAddress = frame * PAGE_SIZE + offset;
  result.status = 'OK';
  result.steps.push(makeStep('Cálculo do endereço físico', {
    lines: [
      `end. físico = quadro × ${PAGE_SIZE} + deslocamento`,
      `= ${frame} × ${PAGE_SIZE} + ${offset} = ${result.physicalAddress} = ${hex(result.physicalAddress)}`,
    ],
    detail: 'O dado é acessado no quadro físico calculado.',
    flashFrame: frame,
    kind: 'success',
    regs: { physical: hex(result.physicalAddress) },
  }));
}

// MMU (Maziero, cap. 14): traduz um endereço lógico em físico usando a tabela do processo ativo.
// Não altera a memória; devolve o resultado e a lista de passos para a interface mostrar.
export function translate(memory: MemoryState, tcb: Tcb, logicalAddress: number): Translation {
  const page = Math.floor(logicalAddress / PAGE_SIZE);
  const offset = logicalAddress % PAGE_SIZE;

  const result: Translation = {
    status: 'OK',
    message: '',
    logicalAddress,
    page,
    offset,
    entryOffset: null,
    entry: null,
    frame: null,
    physicalAddress: null,
    steps: [],
  };

  result.steps.push(makeStep('CPU gera o endereço lógico', {
    lines: [`${hex(logicalAddress)} = ${bin(logicalAddress, ADDRESS_BITS)}₂`],
    detail: 'O processo acessa uma variável usando um endereço relativo ao seu espaço de endereçamento.',
    regs: { logical: hex(logicalAddress) },
  }));

  result.steps.push(makeStep('MMU decompõe o endereço em página e deslocamento', {
    lines: [
      `página = ${logicalAddress} ÷ ${PAGE_SIZE} = ${page}`,
      `deslocamento = ${logicalAddress} mod ${PAGE_SIZE} = ${offset} (${hex(offset, 3)})`,
    ],
    detail: `Os ${PAGE_BITS} bits mais significativos são o número da página; os ${OFFSET_BITS} restantes, o deslocamento.`,
    addressBits: { value: logicalAddress, pageBits: PAGE_BITS, totalBits: ADDRESS_BITS },
    regs: { page: String(page), offset: hex(offset, 3) },
  }));

  if (page >= PAGES_PER_PROCESS) {
    result.status = 'OUT_OF_RANGE';
    result.message = MESSAGES.OUT_OF_RANGE;
    result.steps.push(makeStep('Endereço inválido', {
      lines: [`página ${page} ≥ ${PAGES_PER_PROCESS} páginas do processo`],
      detail: result.message,
      kind: 'error',
    }));
    return result;
  }

  result.entryOffset = entryOffset(tcb.ptbr, page);
  result.entry = readEntry(memory, result.entryOffset);

  result.steps.push(makeStep('MMU consulta a tabela de páginas (quadro 0)', {
    lines: [
      `end. da entrada = PTBR + página × ${PT_ENTRY_SIZE}`,
      `= ${hex(tcb.ptbr)} + ${page} × ${PT_ENTRY_SIZE} = ${hex(result.entryOffset)}`,
    ],
    detail: 'A tabela fica no quadro 0, que é sempre acessado antes de qualquer dado do processo.',
    flashFrame: 0,
    tablePage: page,
    regs: { pte: hex(result.entryOffset) },
  }));

  describeEntry(memory, result);
  return result;
}
