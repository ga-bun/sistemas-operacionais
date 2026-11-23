# Simulador de paginação – React + TypeScript + Bootstrap

```
npm install
npm run dev      # abre em http://localhost:5173
npm run build    # confere os tipos (tsc) e gera a versão final
```

## Organização

| Pasta | Responsabilidade |
|---|---|
| `src/core/` | Regras do sistema em TypeScript puro, **sem React**: `config`, `types`, `utils`, `memory` (quadro 0, RAM, swap), `mmu` (tradução), `part1` (mapeamento fixo), `pagingOs` (Parte 2: page fault, swap-in/out, FIFO/LRU sugerido) |
| `src/hooks/` | `useStepPlayer` (mostra os passos um a um) e `useEventLog` |
| `src/components/` | Um componente por painel: processos/TCB, MMU, RAM, swap, page fault, eventos |
| `src/pages/` | `Part1Page` e `Part2Page`: guardam o estado e ligam o core aos componentes |

## Como o estado funciona

O estado da memória (`MemoryState`) é um objeto simples. Toda função que o altera **copia** o estado antes (`cloneMemory`) e devolve o novo, assim o React percebe a mudança e redesenha a tela.

A MMU (`translate`) não altera nada: devolve o resultado e a lista de passos. O hook `useStepPlayer` revela os passos aos poucos, e os registradores, a linha consultada da tabela e o quadro que pisca são calculados a partir dos passos já exibidos.

## Referência
Maziero, *Sistemas Operacionais: Conceitos e Mecanismos* — cap. 14 (hardware de memória: página, deslocamento, tabela de páginas, MMU, PTBR) e cap. 17 (paginação em disco: page fault, swap, substituição de páginas).
