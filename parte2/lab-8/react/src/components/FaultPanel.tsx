import { PROCESSES } from '../core/config';
import type { Candidate, PendingFault } from '../core/pagingOs';
import { hex } from '../core/utils';

interface Props {
  message: string;
  fault: PendingFault;
  candidates: Candidate[];
  hints: { fifo: number; lru: number } | null; // null quando há quadro livre
  error: string;
  onChoose: (frame: number) => void;
}

// O aluno, no papel do sistema operacional, escolhe o quadro que receberá a página.
export default function FaultPanel({ message, fault, candidates, hints, error, onChoose }: Props) {
  const owner = PROCESSES.find((p) => p.pid === fault.pid);
  const hasFree = candidates.some((c) => c.free);

  return (
    <div className="alert alert-warning mb-3">
      <h6 className="alert-heading">⚠ {message}</h6>
      <p className="mb-1">
        {owner?.name} precisa da página {fault.page}
        {fault.swapBlock !== null && ` (está no bloco de swap ${fault.swapBlock})`} em um quadro de usuário (4–11).
      </p>
      <p className="fw-semibold mb-1">
        {hasFree
          ? 'Há quadros livres: escolha onde alocar a página.'
          : 'Não há quadros livres: escolha a vítima. Ela será movida para a área de swap.'}
      </p>
      {hints && (
        <p className="small mb-2">Sugestões (opcionais): FIFO → Q{hints.fifo} · LRU → Q{hints.lru}</p>
      )}

      <div className="row g-1 mb-2">
        {candidates.map((c) => {
          const dono = PROCESSES.find((p) => p.pid === c.owner);
          return (
            <div key={c.frame} className="col-6">
              <button
                type="button"
                className="btn btn-sm btn-light border w-100 text-start"
                style={dono ? { borderLeft: `5px solid ${dono.color}` } : undefined}
                onClick={() => onChoose(c.frame)}
              >
                <strong>Q{c.frame}</strong> <small className="font-monospace text-muted">{hex(c.start)}</small>
                {hints && hints.fifo === c.frame && <span className="badge bg-primary ms-1">FIFO</span>}
                {hints && hints.lru === c.frame && <span className="badge bg-info text-dark ms-1">LRU</span>}
                <br />
                <small>
                  {dono ? `${dono.name} · pág ${c.page}` : 'livre'} → {c.free ? 'alocar aqui' : 'liberar para o swap'}
                </small>
              </button>
            </div>
          );
        })}
      </div>

      <p className="small text-muted mb-1">
        Também é possível clicar direto em um quadro na RAM (inclusive os reservados, para provocar a violação de proteção).
      </p>
      {error && <p className="text-danger fw-semibold mb-0">{error}</p>}
    </div>
  );
}
