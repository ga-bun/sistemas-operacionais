import type { PageState } from '../core/types';

const CLASSES: Record<PageState, string> = {
  NAL: 'bg-secondary',
  RAM: 'bg-success',
  SWAP: 'bg-warning text-dark',
};

export default function StateBadge({ state }: { state: PageState }) {
  return <span className={`badge ${CLASSES[state]}`}>{state}</span>;
}
