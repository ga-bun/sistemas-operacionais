import { useState } from 'react';
import Part1Page from './pages/Part1Page';
import Part2Page from './pages/Part2Page';

export default function App() {
  const [part, setPart] = useState<1 | 2>(1);

  return (
    <>
      <header className="navbar navbar-dark bg-dark px-3">
        <span className="navbar-brand mb-0 h1 fs-6">Simulador de Memória Virtual por Paginação</span>
        <ul className="nav nav-pills">
          <li className="nav-item">
            <button type="button" className={`nav-link ${part === 1 ? 'active' : 'text-light'}`} onClick={() => setPart(1)}>
              Parte 1 – Tradução
            </button>
          </li>
          <li className="nav-item">
            <button type="button" className={`nav-link ${part === 2 ? 'active' : 'text-light'}`} onClick={() => setPart(2)}>
              Parte 2 – Page fault e swap
            </button>
          </li>
        </ul>
      </header>

      {/* key força o React a recriar a página (e reiniciar o simulador) ao trocar de parte */}
      <main className="container-fluid py-3">
        {part === 1 ? <Part1Page key="p1" /> : <Part2Page key="p2" />}
      </main>
    </>
  );
}
