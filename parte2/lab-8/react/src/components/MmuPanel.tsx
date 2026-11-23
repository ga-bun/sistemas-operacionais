import type { PageTableRow, RegisterKey, StepKind, Tcb, TranslationStep } from '../core/types';
import { bin, hex } from '../core/utils';
import StateBadge from './StateBadge';

const REGISTERS: [RegisterKey, string][] = [
  ['ptbr', 'PTBR'],
  ['logical', 'End. lógico'],
  ['page', 'Página'],
  ['offset', 'Deslocamento'],
  ['pte', 'End. da entrada'],
  ['frame', 'Quadro'],
  ['physical', 'End. físico'],
];

const STEP_CLASSES: Record<StepKind, string> = {
  normal: 'border-primary bg-primary-subtle',
  success: 'border-success bg-success-subtle',
  fault: 'border-warning bg-warning-subtle',
  error: 'border-danger bg-danger-subtle',
};

interface Props {
  tcb: Tcb | null;
  table: PageTableRow[];
  steps: TranslationStep[]; // só os passos já exibidos
  showState: boolean; // Parte 2: mostra a coluna Estado
}

// Os registradores são calculados a partir dos passos exibidos: cada passo define alguns deles.
function registersFrom(steps: TranslationStep[], tcb: Tcb | null): Partial<Record<RegisterKey, string>> {
  const values: Partial<Record<RegisterKey, string>> = {};
  if (tcb) values.ptbr = hex(tcb.ptbr);
  steps.forEach((step) => Object.assign(values, step.regs));
  return values;
}

export default function MmuPanel({ tcb, table, steps, showState }: Props) {
  const values = registersFrom(steps, tcb);
  const lastStep = steps.length > 0 ? steps[steps.length - 1] : null;

  // Linha da tabela de páginas consultada (último passo que a leu).
  let highlightPage: number | null = null;
  for (const step of steps) {
    if (step.tablePage !== null) highlightPage = step.tablePage;
  }

  if (!tcb) {
    return <p className="text-muted">Clique em um processo para carregar o PTBR na MMU.</p>;
  }

  return (
    <div>
      <div className="mb-2">Processo ativo: <strong>{tcb.name} (PID {tcb.pid})</strong></div>

      <div className="row row-cols-4 g-2 mb-3">
        {REGISTERS.map(([key, label]) => {
          const updated = lastStep !== null && key in lastStep.regs;
          return (
            <div className="col" key={key}>
              <div className="border rounded px-2 py-1 bg-light">
                <div className="text-uppercase text-muted" style={{ fontSize: '0.65rem' }}>{label}</div>
                <output key={updated ? `${key}-${steps.length}` : key} className={`font-monospace fw-semibold d-block ${updated ? 'reg-updated' : ''}`}>
                  {values[key] ?? '—'}
                </output>
              </div>
            </div>
          );
        })}
      </div>

      <h6 className="text-uppercase text-muted">Tabela de páginas (lida do quadro 0)</h6>
      <table className="table table-sm table-bordered text-center font-monospace small">
        <thead className="table-light">
          <tr>
            <th>Página</th>
            {showState && <th>Estado</th>}
            <th>{showState ? 'Localização' : 'Quadro'}</th>
          </tr>
        </thead>
        <tbody>
          {table.map((row) => (
            <tr key={row.page} className={row.page === highlightPage ? 'table-warning fw-bold' : ''}>
              <td>{row.page}</td>
              {showState && <td><StateBadge state={row.state} /></td>}
              <td>{row.state === 'NAL' ? '–' : row.location}</td>
            </tr>
          ))}
        </tbody>
      </table>

      <h6 className="text-uppercase text-muted">Passos da tradução</h6>
      {steps.length === 0 && (
        <p className="text-muted small">Clique em uma variável do processo ativo para ver a tradução.</p>
      )}
      {steps.map((step, index) => (
        <div key={index} className={`d-flex gap-2 border-start border-4 rounded p-2 mb-2 step-in ${STEP_CLASSES[step.kind]}`}>
          <span className="badge rounded-pill bg-dark align-self-start">{index + 1}</span>
          <div className="min-w-0">
            <div className="fw-semibold">{step.title}</div>

            {step.addressBits && (
              <div className="d-flex gap-2 my-1 font-monospace">
                <span className="px-2 rounded bg-warning-subtle text-center">
                  {bin(step.addressBits.value, step.addressBits.totalBits).slice(0, step.addressBits.pageBits)}
                  <small className="d-block font-sans-serif">página</small>
                </span>
                <span className="px-2 rounded bg-info-subtle text-center">
                  {bin(step.addressBits.value, step.addressBits.totalBits).slice(step.addressBits.pageBits)}
                  <small className="d-block font-sans-serif">deslocamento</small>
                </span>
              </div>
            )}

            {step.lines.map((line) => (
              <code key={line} className="d-block text-dark">{line}</code>
            ))}
            {step.detail && <div className="small text-muted mt-1">{step.detail}</div>}
          </div>
        </div>
      ))}
    </div>
  );
}
