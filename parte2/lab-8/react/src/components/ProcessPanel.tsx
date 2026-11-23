import type { PageTableRow, Tcb } from '../core/types';
import { hex, logicalAddressOf } from '../core/utils';
import StateBadge from './StateBadge';

interface Props {
  processes: Tcb[];
  activePid: number | null;
  locked?: boolean;
  getTable?: (tcb: Tcb) => PageTableRow[]; // quando informado, mostra a tabela de cada processo
  onSelect: (pid: number) => void;
  onAccess: (variableIndex: number) => void;
}

export default function ProcessPanel({ processes, activePid, locked = false, getTable, onSelect, onAccess }: Props) {
  return (
    <div>
      {processes.map((tcb) => {
        const active = tcb.pid === activePid;
        const table = getTable ? getTable(tcb) : null;

        return (
          <div key={tcb.pid} className="card mb-3" style={{ borderLeft: `6px solid ${tcb.color}` }}>
            <div className="card-body p-2">
              <button
                type="button"
                className={`btn btn-sm w-100 d-flex justify-content-between ${active ? 'btn-dark' : 'btn-outline-secondary'}`}
                disabled={locked && !active}
                title="Clique para tornar este processo ativo (a MMU é atualizada)"
                onClick={() => onSelect(tcb.pid)}
              >
                <strong>{tcb.name}</strong>
                <span>{active ? 'em execução' : 'pronto'}</span>
              </button>

              <div className="small text-uppercase text-muted mt-2">TCB</div>
              <dl className="row small font-monospace bg-light rounded mx-0 mb-2 py-1">
                <dt className="col-5 fw-normal text-muted">PID</dt>
                <dd className="col-7 mb-0">{tcb.pid}</dd>
                <dt className="col-5 fw-normal text-muted">PTBR</dt>
                <dd className="col-7 mb-0">{hex(tcb.ptbr)}</dd>
                <dt className="col-5 fw-normal text-muted">Tabela em</dt>
                <dd className="col-7 mb-0">quadro 0 + {hex(tcb.ptbr)}</dd>
              </dl>

              {table && (
                <table className="table table-sm table-bordered text-center small mb-2">
                  <thead className="table-light">
                    <tr><th>Pág</th><th>Estado</th><th>Local</th></tr>
                  </thead>
                  <tbody>
                    {table.map((row) => (
                      <tr key={row.page}>
                        <td>{row.page}</td>
                        <td><StateBadge state={row.state} /></td>
                        <td>{row.state === 'NAL' ? '–' : row.location}</td>
                      </tr>
                    ))}
                  </tbody>
                </table>
              )}

              <div className="small text-uppercase text-muted">Variáveis (clique para acessar)</div>
              <div className="row g-1 mt-0">
                {tcb.variables.map((variable, index) => (
                  <div key={variable.name} className="col-6">
                    <button
                      type="button"
                      className="btn btn-sm btn-outline-primary w-100 text-start"
                      disabled={!active || locked}
                      onClick={() => onAccess(index)}
                    >
                      <strong>{variable.name}</strong>
                      <br />
                      <small className="font-monospace">pág {variable.page} · {hex(logicalAddressOf(tcb, index))}</small>
                    </button>
                  </div>
                ))}
              </div>
            </div>
          </div>
        );
      })}
    </div>
  );
}
