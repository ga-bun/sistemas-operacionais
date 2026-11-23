import { PROCESSES } from '../core/config';
import type { Frame } from '../core/types';
import type { Flash } from '../hooks/useStepPlayer';
import { frameEnd, frameStart, hex } from '../core/utils';

interface Props {
  frames: Frame[];
  flash: Flash | null;
  selectable?: boolean; // Parte 2: permite clicar em um quadro para escolhê-lo
  onFrameClick?: (frame: number) => void;
}

function labelOf(frame: Frame) {
  if (frame.area === 'tables') {
    return (
      <>
        Tabelas de páginas{' '}
        {PROCESSES.map((p) => (
          <span key={p.pid} className="badge me-1 font-monospace" style={{ backgroundColor: p.color }}>
            {p.name} {hex(p.ptbr)}
          </span>
        ))}
      </>
    );
  }
  if (frame.area === 'kernel') return 'Kernel / SO';
  if (frame.area === 'reserved') return 'Reservado ao SO';

  const owner = PROCESSES.find((p) => p.pid === frame.owner);
  return owner ? `${owner.name} · pág ${frame.page}` : 'livre';
}

// Vetor de quadros com o endereço físico ao lado; a cor indica o processo dono.
export default function RamPanel({ frames, flash, selectable = false, onFrameClick }: Props) {
  return (
    <div>
      {frames.map((frame) => {
        const owner = PROCESSES.find((p) => p.pid === frame.owner);
        const myFlash = flash !== null && flash.frame === frame.index ? flash : null;

        const classes = ['ram-row', `area-${frame.area}`, 'd-flex', 'align-items-center', 'gap-2', 'border', 'rounded', 'px-2', 'py-1', 'mb-1'];
        if (owner) classes.push('is-owned');
        else if (frame.area === 'user') classes.push('is-free');
        if (selectable) classes.push('selectable');
        if (myFlash) classes.push(myFlash.kind === 'error' ? 'flash-error' : 'flash');

        return (
          <div
            // a chave muda quando o quadro deve piscar de novo, o que reinicia a animação
            key={myFlash ? `${frame.index}-${myFlash.id}` : frame.index}
            className={classes.join(' ')}
            style={owner ? { backgroundColor: owner.color } : undefined}
            onClick={() => selectable && onFrameClick && onFrameClick(frame.index)}
          >
            <span className="font-monospace small lh-1" style={{ width: 70 }}>
              {hex(frameStart(frame.index))}
              <br />
              <span className="ram-end">{hex(frameEnd(frame.index))}</span>
            </span>
            <strong className="font-monospace" style={{ width: 32 }}>Q{frame.index}</strong>
            <span className="small">{labelOf(frame)}</span>
          </div>
        );
      })}
    </div>
  );
}
