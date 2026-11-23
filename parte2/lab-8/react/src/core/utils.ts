import { PAGE_SIZE } from './config';
import type { Tcb } from './types';

export function hex(value: number, digits = 4): string {
  return '0x' + value.toString(16).toUpperCase().padStart(digits, '0');
}

export function bin(value: number, bits: number): string {
  return value.toString(2).padStart(bits, '0');
}

export function frameStart(frame: number): number {
  return frame * PAGE_SIZE;
}

export function frameEnd(frame: number): number {
  return frame * PAGE_SIZE + PAGE_SIZE - 1;
}

// Endereço lógico = página × tamanho da página + deslocamento.
export function logicalAddressOf(tcb: Tcb, variableIndex: number): number {
  const variable = tcb.variables[variableIndex];
  return variable.page * PAGE_SIZE + variable.offset;
}
