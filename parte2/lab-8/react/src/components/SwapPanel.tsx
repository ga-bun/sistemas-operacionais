import { PROCESSES } from '../core/config';
import type { SwapBlock } from '../core/types';

// Área de swap: blocos de 16 a 23.
export default function SwapPanel({ blocks }: { blocks: SwapBlock[] }) {
  return (
    <div>
      {blocks.map((block) => {
        const owner = PROCESSES.find((p) => p.pid === block.owner);
        return (
          <div
            key={block.index}
            className={`ram-row d-flex justify-content-between border rounded px-2 py-1 mb-1 ${owner ? 'is-owned' : 'is-free'}`}
            style={owner ? { backgroundColor: owner.color } : undefined}
          >
            <span className="font-monospace small">Bloco {block.index}</span>
            <span className="small">{owner ? `${owner.name} · pág ${block.page}` : 'livre'}</span>
          </div>
        );
      })}
    </div>
  );
}
