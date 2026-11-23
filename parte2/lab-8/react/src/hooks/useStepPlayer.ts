import { useEffect, useRef, useState } from 'react';
import type { Translation, TranslationStep } from '../core/types';

export interface Flash {
  frame: number;
  kind: 'access' | 'error';
  id: string; // muda a cada passo, para reiniciar a animação de piscar
}

interface Run {
  id: number;
  translation: Translation;
  visible: number; // quantos passos já aparecem
}

// Mostra os passos da tradução um a um: automático (com atraso) ou manual (botão "Próximo").
export function useStepPlayer(manual: boolean, delayMs = 1100) {
  const [run, setRun] = useState<Run | null>(null);
  const nextId = useRef(1);

  useEffect(() => {
    if (run === null || manual || run.visible >= run.translation.steps.length) return;

    const timer = setTimeout(() => {
      setRun((current) => (current ? { ...current, visible: current.visible + 1 } : current));
    }, delayMs);
    return () => clearTimeout(timer);
  }, [run, manual, delayMs]);

  function start(translation: Translation) {
    setRun({ id: nextId.current++, translation, visible: 1 });
  }

  function reset() {
    setRun(null);
  }

  function next() {
    setRun((current) => {
      if (current === null || current.visible >= current.translation.steps.length) return current;
      return { ...current, visible: current.visible + 1 };
    });
  }

  const steps: TranslationStep[] = run ? run.translation.steps.slice(0, run.visible) : [];
  const finished = run !== null && run.visible >= run.translation.steps.length;

  // O último passo exibido que acessa um quadro define qual quadro pisca na RAM.
  let flash: Flash | null = null;
  if (run !== null) {
    for (let i = 0; i < steps.length; i++) {
      if (steps[i].flashFrame !== null) {
        flash = {
          frame: steps[i].flashFrame as number,
          kind: steps[i].kind === 'error' ? 'error' : 'access',
          id: `${run.id}-${i}`,
        };
      }
    }
  }

  return { steps, finished, flash, running: run !== null, start, reset, next };
}
