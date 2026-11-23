import { useState } from 'react';
import EventLog from '../components/EventLog';
import FaultPanel from '../components/FaultPanel';
import MmuPanel from '../components/MmuPanel';
import ProcessPanel from '../components/ProcessPanel';
import RamPanel from '../components/RamPanel';
import StepControls from '../components/StepControls';
import SwapPanel from '../components/SwapPanel';
import { PROCESSES } from '../core/config';
import { readTable } from '../core/memory';
import { translate } from '../core/mmu';
import {
  createFault,
  createPart2Memory,
  listCandidates,
  registerAccess,
  resolveFault,
  suggestVictims,
} from '../core/pagingOs';
import type { PendingFault } from '../core/pagingOs';
import type { MemoryState, Tcb } from '../core/types';
import { hex, logicalAddressOf } from '../core/utils';
import { useEventLog } from '../hooks/useEventLog';
import type { Flash } from '../hooks/useStepPlayer';
import { useStepPlayer } from '../hooks/useStepPlayer';

// Parte 2: tudo começa NAL; o aluno faz o papel do SO e escolhe o quadro de cada page fault.
export default function Part2Page() {
  const [memory, setMemory] = useState<MemoryState>(createPart2Memory);
  const [activePid, setActivePid] = useState<number | null>(null);
  const [fault, setFault] = useState<PendingFault | null>(null);
  const [faultMessage, setFaultMessage] = useState('');
  const [faultError, setFaultError] = useState('');
  const [errorFlash, setErrorFlash] = useState<Flash | null>(null);
  const [lastVariable, setLastVariable] = useState(0);
  const [manual, setManual] = useState(false);
  const player = useStepPlayer(manual);
  const log = useEventLog('Todas as páginas começam NAL. Ative um processo e acesse suas variáveis.');

  const activeTcb = PROCESSES.find((p) => p.pid === activePid) ?? null;
  const choosing = fault !== null && player.finished; // só decide depois de ver todos os passos

  function selectProcess(pid: number) {
    const tcb = PROCESSES.find((p) => p.pid === pid);
    if (!tcb || fault) return;
    setActivePid(pid);
    player.reset();
    setErrorFlash(null);
    log.add('os', `${tcb.name} é o processo ativo: MMU carregada com PTBR = ${hex(tcb.ptbr)}.`);
  }

  // 'mem' é passado como parâmetro porque, logo após resolver um fault, o estado novo ainda não chegou.
  function runAccess(tcb: Tcb, index: number, mem: MemoryState) {
    const address = logicalAddressOf(tcb, index);
    const result = translate(mem, tcb, address);

    setLastVariable(index);
    setErrorFlash(null);
    setMemory(registerAccess(mem, result.status === 'OK' ? result.frame : null));
    player.start(result);

    if (result.status === 'OK') {
      log.add('ok', `Tradução normal: ${hex(address)} → ${hex(result.physicalAddress ?? 0)} (quadro ${result.frame}).`);
      return;
    }

    const pending = createFault(tcb, result);
    if (pending) {
      setFault(pending);
      setFaultMessage(result.message);
      setFaultError('');
      log.add('fault', result.message);
    } else {
      log.add('error', result.message);
    }
  }

  function accessVariable(index: number) {
    if (!activeTcb || fault) return;
    log.add('info', `${activeTcb.name} acessa a variável '${activeTcb.variables[index].name}' (endereço lógico ${hex(logicalAddressOf(activeTcb, index))}).`);
    runAccess(activeTcb, index, memory);
  }

  function chooseFrame(frame: number) {
    if (!fault || !player.finished || !activeTcb) return;

    const outcome = resolveFault(memory, fault, frame);
    if (!outcome.ok) {
      log.add('error', outcome.message);
      setFaultError(outcome.message);
      setErrorFlash({ frame, kind: 'error', id: `erro-${Date.now()}` });
      return;
    }

    outcome.events.forEach((text) => log.add('os', text));
    setFault(null);
    setFaultError('');
    log.add('info', 'A instrução que causou o page fault é reexecutada.');
    runAccess(activeTcb, lastVariable, outcome.memory);
  }

  return (
    <div className="row g-3">
      <section className="col-lg-3">
        <h5>Processos, TCB e tabelas</h5>
        <ProcessPanel
          processes={PROCESSES}
          activePid={activePid}
          locked={fault !== null}
          getTable={(tcb) => readTable(memory, tcb)}
          onSelect={selectProcess}
          onAccess={accessVariable}
        />
      </section>

      <section className="col-lg-5">
        <h5>MMU</h5>
        <StepControls manual={manual} canNext={player.running && !player.finished} onManualChange={setManual} onNext={player.next} />
        {choosing && fault && (
          <FaultPanel
            message={faultMessage}
            fault={fault}
            candidates={listCandidates(memory)}
            hints={suggestVictims(memory)}
            error={faultError}
            onChoose={chooseFrame}
          />
        )}
        <MmuPanel
          tcb={activeTcb}
          table={activeTcb ? readTable(memory, activeTcb) : []}
          steps={player.steps}
          showState
        />
        <h6 className="text-uppercase text-muted mt-3">Eventos</h6>
        <EventLog entries={log.entries} />
      </section>

      <section className="col-lg-4">
        <h5>RAM (quadros 0–15)</h5>
        <RamPanel frames={memory.frames} flash={errorFlash ?? player.flash} selectable={choosing} onFrameClick={chooseFrame} />
        <h5 className="mt-3">Swap (blocos 16–23)</h5>
        <SwapPanel blocks={memory.swap} />
      </section>
    </div>
  );
}
