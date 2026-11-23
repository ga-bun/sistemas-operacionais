# Como explicar o projeto – Simulador de Memória Virtual por Paginação

Referência: Maziero, *Sistemas Operacionais: Conceitos e Mecanismos* — cap. 14 (hardware de memória) e cap. 17 (paginação em disco).

A ordem que funciona melhor é: problema → conceitos → exemplo com números → demonstração → código.

## 1. O problema (1 minuto)

- Vários processos rodam ao mesmo tempo e cada um acha que tem a memória só para si.
- A memória física (RAM) é pequena e compartilhada. Aqui são 16 KiB.
- O SO e o hardware precisam traduzir os endereços do processo para posições reais da RAM e isolar um processo do outro. É isso que o simulador mostra.

## 2. Conceitos, nesta ordem

**Endereço lógico e endereço físico.** O processo usa endereços lógicos (virtuais), como se tivesse a memória inteira. O físico é a posição real na RAM.

**Paginação.** O espaço lógico é dividido em **páginas** e a RAM em **quadros** (frames), todos do mesmo tamanho, aqui 1 KiB. Qualquer página pode ir para qualquer quadro, sem precisar ser contígua.

**Página e deslocamento.** O endereço lógico tem 12 bits: 2 bits de **página** (4 páginas) e 10 bits de **deslocamento** (1024 bytes). Por isso:

```
página       = endereço ÷ 1024
deslocamento = endereço mod 1024
```

**Tabela de páginas.** Cada processo tem uma tabela que diz em qual quadro cada página está. Aqui todas ficam no **quadro 0**.

**PTBR e TCB.** O PTBR (Page Table Base Register) diz onde começa a tabela do processo ativo. Cada processo guarda o seu no **TCB**. Na troca de processo, o SO copia o PTBR do novo para a MMU. Por isso, clicar no processo "atualiza a MMU".

**MMU.** É o hardware que faz a tradução a cada acesso: lê a tabela e monta o endereço físico. O processo nem percebe.

## 3. Exemplos com números

### P1 acessa a variável `a` (endereço lógico `0x0010` = 16)

1. `página = 16 ÷ 1024 = 0` e `deslocamento = 16`.
2. A entrada da tabela fica em `PTBR + página × 4 = 0x0000 + 0 = 0x0000`, no quadro 0. Por isso o quadro 0 pisca primeiro.
3. A entrada diz "página 0 → quadro 5".
4. `físico = 5 × 1024 + 16 = 5136 = 0x1410`. O quadro 5 pisca.

### P2 acessa a variável `c` (endereço lógico `0x09A0` = 2464)

1. `página = 2464 ÷ 1024 = 2` e `deslocamento = 416`.
2. A entrada fica em `0x0100 + 2 × 4 = 0x0108` (a tabela de P2 começa em `0x0100`).
3. A entrada diz "página 2 → quadro 12".
4. `físico = 12 × 1024 + 416 = 12704 = 0x31A0`.

Esse segundo exemplo mostra que cada processo tem a sua própria tabela, em outro offset do quadro 0.

## 4. Parte 2: memória virtual sob demanda

**Ideia central.** Nem todas as páginas cabem na RAM. 3 processos × 4 páginas dão 12 páginas, mas só há 8 quadros de usuário. Então só se carrega uma página quando ela é usada (**paginação sob demanda**).

**Estados de uma página:**

| Estado | Significado |
|---|---|
| NAL | Nunca usada; não está em lugar nenhum |
| RAM | Está em um quadro (4–11) |
| SWAP | Foi expulsa para o disco (blocos 16–23) |

**Page fault.** É a exceção que a MMU dispara quando a página não está na RAM (NAL ou SWAP). O SO assume, escolhe um quadro e o acesso é refeito. É o que o simulador faz com "a instrução é reexecutada".

**Fluxo:**

1. Acesso a uma página NAL → "Page Fault – página ainda não alocada".
2. Se há quadro livre, o aluno escolhe qual.
3. Se a RAM está cheia, é preciso escolher uma **vítima**. Ela vai para o swap e sua tabela passa a SWAP.
4. Mais tarde, acessar uma página que está no swap causa outro fault, e ela volta (**swap-in**).

**Política de substituição.** Escolher a vítima é uma decisão importante. FIFO tira a página mais antiga e LRU tira a menos usada recentemente. O simulador só sugere; a escolha é manual, como o enunciado pede.

**Proteção.** Os quadros 0–3 e 12–15 pertencem ao SO. Se um processo tentar mapear um deles, é uma **violação de proteção**. Dá para provocar clicando num quadro reservado durante a escolha.

## 5. Roteiro de demonstração (3 a 5 minutos)

1. **Parte 1:** clique em P1 e mostre o TCB e o PTBR. Clique na variável `a` e leia cada passo em voz alta, mostrando o quadro 0 e depois o quadro 5 piscando.
2. Troque para P2 e mostre que o PTBR e a tabela mudam.
3. **Parte 2:** acesse uma variável de P1 e mostre o fault NAL e a escolha do quadro.
4. Acesse mais variáveis até encher os 8 quadros e mostre o fault com a vítima indo para o swap.
5. Reacesse uma página que está no swap e mostre o swap-in.
6. Provoque a violação de proteção clicando num quadro reservado.

## 6. Como o código reflete isso

- `core/` é o "sistema operacional e o hardware", sem nada de tela.
- `components/` é só o desenho.
- `pages/` liga os dois.

A MMU (`core/mmu.ts`) nunca altera a memória, só traduz e devolve os passos. Quem altera é o SO (`core/pagingOs.ts`). Isso espelha a divisão real: o hardware traduz e o SO trata o fault.

## 7. Perguntas que podem aparecer

| Pergunta | Resposta |
|---|---|
| Por que o quadro 0 é acessado primeiro? | A tabela fica na memória e precisa ser lida antes do dado. Isso dá dois acessos à memória por tradução. |
| Isso não deixa o acesso lento? | Sim. O hardware real usa a **TLB**, um cache de traduções recentes. O simulador não a implementa, mas o livro explica. |
| O que acontece com a página da vítima? | Vai para o swap e a tabela dela passa a SWAP. |
| E se o swap encher? | Aqui nunca acontece (12 páginas, 8 quadros, 8 blocos), mas o código trata como erro. |
| O que o simulador não faz? | TLB, bit de página suja (dirty), páginas de tamanho variável e tabelas multinível. |

## 8. Escolhas minhas, não do enunciado

O enunciado não define alguns detalhes. Diga isso se perguntarem:

- Os deslocamentos das variáveis (por exemplo, `a` em `0x010`).
- A entrada da tabela de páginas ocupar 4 bytes.
- Na Parte 2, as tabelas continuarem no quadro 0.
- FIFO/LRU serem só sugestão.
