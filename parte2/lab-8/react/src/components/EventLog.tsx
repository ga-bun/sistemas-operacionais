import { useEffect, useRef } from 'react';
import type { LogEntry, LogKind } from '../hooks/useEventLog';

const COLORS: Record<LogKind, string> = {
  info: '',
  ok: 'text-success',
  fault: 'text-warning-emphasis fw-semibold',
  error: 'text-danger fw-semibold',
  os: 'text-primary',
};

export default function EventLog({ entries }: { entries: LogEntry[] }) {
  const boxRef = useRef<HTMLDivElement>(null);

  // Mantém a rolagem no evento mais recente.
  useEffect(() => {
    if (boxRef.current) boxRef.current.scrollTop = boxRef.current.scrollHeight;
  }, [entries]);

  return (
    <div ref={boxRef} className="border rounded bg-light p-2 small event-log">
      {entries.map((entry) => (
        <div key={entry.id} className={COLORS[entry.kind]}>{entry.text}</div>
      ))}
    </div>
  );
}
