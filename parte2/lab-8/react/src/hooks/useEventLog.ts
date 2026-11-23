import { useRef, useState } from 'react';

export type LogKind = 'info' | 'ok' | 'fault' | 'error' | 'os';

export interface LogEntry {
  id: number;
  kind: LogKind;
  text: string;
}

const MAX_ENTRIES = 300;

export function useEventLog(firstMessage: string) {
  const [entries, setEntries] = useState<LogEntry[]>([{ id: 0, kind: 'info', text: firstMessage }]);
  const nextId = useRef(1);

  function add(kind: LogKind, text: string) {
    const id = nextId.current;
    nextId.current += 1;
    setEntries((previous) => [...previous.slice(-(MAX_ENTRIES - 1)), { id, kind, text }]);
  }

  return { entries, add };
}
