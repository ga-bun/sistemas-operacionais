import { useState } from 'react';
import EventLog from '../components/EventLog';
import MmuPanel from '../components/MmuPanel';
import ProcessPanel from '../components/ProcessPanel';
import RamPanel from '../components/RamPanel';
import StepControls from '../components/StepControls';
import { PROCESSES } from '../core/config';
import { readTable } from '../core/memory';
import { translate } from '../core/mmu';
import { createPart1Memory } from '../core/part1';
import { hex, logicalAddressOf } from '../core/utils';
import { useEventLog } from '../hooks/useEventLog';
import { useStepPlayer } from '../hooks/useStepPlayer';

// Parte 1: mapeamento fixo; o aluno escolhe o processo e acessa suas variáveis.
export default function Part1Page() {
  const [memory] = useState(createPart1Memory);
  const [activePid, setActivePid] = useState<number | null>(null);
  const [manual, setManual] = useState(false);
  const player = useStepPlayer(manual);
  const log = useEventLog('Clique em um processo para ativá-lo e depois em uma de suas variáveis.');

  const activeTcb = PROCESSES.find((p) => p.pid === activePid) ?? null;

  function selectProcess(pid: number) {
    const tcb = PROCESSES.find((p) => p.pid === pid);
    if (!tcb) return;
    setActivePid(pid);
    player.reset();
    log.add('os', `${tcb.name} é o processo ativo: MMU carregada com PTBR = ${hex(tcb.ptbr)}.`);
  }

  function accessVariable(index: number) {
    if (!activeTcb) return;
    const address = logicalAddressOf(activeTcb, index);
    const result = translate(memory, activeTcb, address);

    log.add('info', `${activeTcb.name} acessa a variável '${activeTcb.variables[index].name}' (endereço lógico ${hex(address)}).`);
    log.add('ok', `Tradução: ${hex(address)} → ${hex(result.physicalAddress ?? 0)} (quadro ${result.frame}).`);
    player.start(result);
  }

  return (
    <div className="row g-3">
      <section className="col-lg-3">
        <h5>Processos e TCB</h5>
        <ProcessPanel processes={PROCESSES} activePid={activePid} onSelect={selectProcess} onAccess={accessVariable} />
      </section>

      <section className="col-lg-5">
        <h5>MMU</h5>
        <StepControls manual={manual} canNext={player.running && !player.finished} onManualChange={setManual} onNext={player.next} />
        <MmuPanel
          tcb={activeTcb}
          table={activeTcb ? readTable(memory, activeTcb) : []}
          steps={player.steps}
          showState={false}
        />
        <h6 className="text-uppercase text-muted mt-3">Eventos</h6>
        <EventLog entries={log.entries} />
      </section>

      <section className="col-lg-4">
        <h5>RAM (16 KiB)</h5>
        <RamPanel frames={memory.frames} flash={player.flash} />
      </section>
    </div>
  );
}
