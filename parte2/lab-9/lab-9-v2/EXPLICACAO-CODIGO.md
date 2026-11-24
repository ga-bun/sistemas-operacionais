# Explicação do código – Simulador de Substituição de Páginas (lab-9)

Arquivos: [index.html](./index.html) (estrutura da tela + JavaScript com a lógica) e [style.css](./style.css) (aparência). O `index.html` carrega o CSS com `<link rel="stylesheet" href="style.css">`, então os dois precisam ficar na mesma pasta.

Referência: Maziero, *Sistemas Operacionais: Conceitos e Mecanismos*, cap. 17 (paginação em disco e algoritmos de substituição).

## 1. O problema que o código resolve

A RAM tem poucos quadros. Quando um processo acessa uma página que **não está** na memória, ocorre uma **falta de página** (page fault). Se todos os quadros estão ocupados, o SO precisa escolher uma página **vítima** para sair. Cada algoritmo tem um critério diferente:

| Algoritmo | Quem sai? |
|---|---|
| FIFO | A que está há mais tempo na memória |
| LRU | A que foi usada há mais tempo (menos recentemente) |
| OPT | A que só será usada mais longe no futuro (ótimo, teórico) |
| RANDOM | Uma sorteada |

O código simula cada algoritmo sobre uma **sequência de referências** (ex.: `7 0 1 2 0 3 0 4`) e conta as faltas. Quanto menos faltas, melhor.

## 2. Mapa do arquivo

O JavaScript está dividido em blocos, na ordem em que aparecem no arquivo:

| Bloco | Funções principais | Papel |
|---|---|---|
| Algoritmos | `fifo`, `lru`, `opt`, `random` e auxiliares | A lógica de substituição, sem tocar na tela |
| Entrada e utilitários | `lerEntrada`, `el`, `criarTabelaDeExecucao` | Lê e valida os campos e desenha tabelas |
| Modo automático | `executarAutomatico`, `mostrarResumo`, `mostrarTabelas` | Roda os quatro algoritmos e mostra o resultado |
| Modo manual | `iniciarManual`, `avancarManual`, `desenharManual` | FIFO passo a passo, com cliques |
| Gráfico | `gerarGrafico`, `desenharGrafico` | Faltas × número de quadros no `<canvas>` |
| Ligação dos botões | `addEventListener` no final | Conecta botões e funções |

A ideia é separar duas coisas: os **algoritmos** só calculam, e o resto só mostra. Por isso dá para entender e testar cada algoritmo sem olhar a interface.

## 3. Como a memória é representada

A memória é um vetor com um espaço por quadro. `null` significa quadro livre:

```js
function memoriaVazia(quadros) {
  return new Array(quadros).fill(null);   // com 3 quadros: [null, null, null]
}
```

Quando uma página substitui outra, ela ocupa **o mesmo quadro** da vítima, como nas tabelas do livro. Por isso os algoritmos trabalham com a posição no vetor.

## 4. O formato do resultado

Todo algoritmo recebe `(seq, quadros)` e devolve:

```js
{ faltas: 7, passos: [ ... ] }
```

Cada item de `passos` descreve o estado **depois de um acesso**:

```js
{ pagina: 2, falta: true, vitima: 7, memoria: [2, 0, 1] }
```

- `pagina`: a página acessada.
- `falta`: `true` se a página não estava na memória.
- `vitima`: a página que saiu, ou `null` (hit ou quadro livre).
- `memoria`: uma **cópia** do vetor naquele instante (`[...memoria]`). A cópia é necessária porque o vetor original continua mudando nos acessos seguintes.

As funções auxiliares `criarPasso` e `criarResultado` montam esses objetos.

## 5. Os algoritmos

Todos seguem o mesmo esqueleto para cada página `p` da sequência:

```
se p já está na memória       → hit (nada a substituir)
senão (falta):
    se existe quadro livre    → usa o quadro livre
    senão                     → escolhe a vítima (aqui os algoritmos diferem)
    coloca p no quadro escolhido
```

Só a linha "escolhe a vítima" muda de um algoritmo para outro.

### 5.1 FIFO (`fifo` e `fifoPasso`)

Mantém uma **fila** com a ordem de chegada. A vítima é o primeiro da fila:

```js
vitima = fila.shift();                 // remove a mais antiga
posicao = memoria.indexOf(vitima);     // descobre em qual quadro ela estava
memoria[posicao] = pagina;
fila.push(pagina);                     // a nova entra no fim
```

Um detalhe importante: a lógica de **um acesso** fica em `fifoPasso(estado, pagina)`. A função `fifo()` só repete `fifoPasso` para a sequência inteira, e o **modo manual** chama a mesma `fifoPasso` a cada clique. Assim os dois modos nunca divergem.

### 5.2 LRU (`lru`)

Guarda, em `ultimoUso`, a posição da sequência em que cada página foi usada pela última vez. **Hit também atualiza** esse valor (a linha `ultimoUso[pagina] = i`). A vítima é a página de menor `ultimoUso`:

```js
posicao = indiceDoMenor(memoria, (p) => ultimoUso[p]);
```

### 5.3 OPT (`opt`)

Olha o **futuro** da sequência. `proximoUso` devolve a posição do próximo acesso a uma página, ou `Infinity` se ela nunca mais for usada:

```js
posicao = indiceDoMaior(memoria, (p) => proximoUso(seq, i, p));
```

A vítima é a que tem o maior valor, ou seja, a que demora mais a ser usada. Se várias nunca mais serão usadas, sai a primeira que aparece na memória. O OPT é teórico, porque um SO real não conhece o futuro. Ele serve de **limite inferior** para comparar os outros.

### 5.4 RANDOM (`random`)

Sorteia a vítima: `Math.floor(Math.random() * quadros)`. O resultado muda a cada execução.

### 5.5 Auxiliares `indiceDoMenor` e `indiceDoMaior`

Recebem o vetor da memória e uma função `chave`, e devolvem a posição do elemento com a menor (ou maior) chave. Isso evita repetir o mesmo laço no LRU e no OPT. A função passada como argumento (`(p) => ultimoUso[p]`) é o que diferencia os dois.

## 6. Exemplo passo a passo

Sequência `7 0 1 2 0 3 0 4` com 3 quadros. `F` = falta.

**FIFO**

| Ref. | 7 | 0 | 1 | 2 | 0 | 3 | 0 | 4 |
|---|---|---|---|---|---|---|---|---|
| Q0 | 7 | 7 | 7 | **2** | 2 | 2 | 2 | **4** |
| Q1 | | 0 | 0 | 0 | 0 | **3** | 3 | 3 |
| Q2 | | | 1 | 1 | 1 | 1 | **0** | 0 |
| Acesso | F | F | F | F | hit | F | F | F |

Total: **7 faltas**. Na falta da página 2, a fila é `7, 0, 1`, então sai o 7. Na da página 3, a fila é `0, 1, 2`, então sai o 0. Na do segundo 0, a fila é `1, 2, 3`, então sai o 1.

**LRU** nesta mesma sequência tem **6 faltas**. Quando entra a página 3, o 0 foi usado há pouco (hit no passo anterior) e o 1 está parado desde o início, então sai o 1. É por isso que o LRU costuma superar o FIFO: ele protege páginas que continuam em uso.

## 7. Entrada e utilitários

- **`lerEntrada()`** lê os campos, valida (quadros entre 1 e 50, sequência só com inteiros não negativos) e devolve `{ seq, quadros }`. Se algo estiver errado, lança um `Error` com a mensagem.
- **`comTratamentoDeErro(acao)`** envolve uma ação num `try/catch` e mostra a mensagem de erro na tela. Todo botão passa por ela.
- **`el(tag, classe, texto)`** cria um elemento HTML. Usa `textContent`, que trata o conteúdo como texto puro.
- **`criarTabelaDeExecucao(passos, quadros)`** monta a tabela do estilo do livro. Uma célula é destacada (amarelo) quando o quadro mudou numa falta, comparando o passo atual com o anterior.

## 8. Modo automático

`executarAutomatico()` lê a entrada e chama os quatro algoritmos num laço:

```js
Object.keys(ALGORITMOS).forEach((nome) => {
  resultados[nome] = ALGORITMOS[nome](seq, quadros);
});
```

`ALGORITMOS` é um objeto que liga o nome à função (`{ fifo, lru, opt, random }`). Depois, `mostrarResumo` monta a tabela de totais (destacando o menor número de faltas) e `mostrarTabelas` desenha uma tabela por algoritmo.

## 9. Modo manual (FIFO)

O estado fica na variável `manual`:

```js
manual = { seq, quadros, estado, indice, passos };
```

- `estado` é o `{ memoria, fila }` do FIFO.
- `indice` é a próxima página da sequência a ser acessada.
- `passos` é o histórico dos acessos já feitos.

Fluxo:

1. `iniciarManual()` cria o estado zerado.
2. `desenharSequenciaManual()` desenha um botão por página. Só o botão da posição `indice` fica habilitado (azul). Os já feitos ficam verdes (hit) ou vermelhos (falta).
3. Ao clicar, `avancarManual(i)` confere se `i === indice`, chama `fifoPasso`, guarda o resultado em `passos` e aumenta `indice`.
4. `desenharManual()` redesenha tudo: contadores, memória, fila, mensagem e histórico.

A tela é sempre redesenhada **a partir do estado** (`manual`), em vez de ir remendando o que já estava na tela. Isso evita inconsistências. `mensagemDoPasso` gera o texto explicando se foi hit, falta com quadro livre ou falta com vítima.

## 10. Gráfico

Mostra como as faltas variam conforme o **número de quadros**, o que compara os algoritmos de uma vez só.

1. `gerarGrafico()` define o eixo X: de 1 quadro até o número de páginas distintas da sequência (máximo 40). Com tantos quadros quanto páginas distintas, só sobram as faltas obrigatórias.
2. `calcularSeries()` roda os algoritmos para cada quantidade de quadros e guarda os totais em quatro séries.
3. No RANDOM, `mediaRandom()` faz a média de 200 execuções, porque uma só seria muito instável.
4. `desenharGrafico()` desenha no `<canvas>`: grade, eixos, linhas, marcadores e legenda. A conversão de valor para pixel é feita pelas funções `px` (eixo X) e `py` (eixo Y).
5. O canvas é transformado em imagem (`toDataURL`) para o botão "Baixar PNG".
6. `mostrarAvisoBelady()` compara pontos consecutivos da série do FIFO. Se com mais quadros houver **mais** faltas, mostra o aviso da **anomalia de Belády**.

## 11. Perguntas que podem aparecer

| Pergunta | Resposta |
|---|---|
| Por que o OPT não é usado de verdade? | Precisa conhecer o futuro. Serve só como referência do melhor resultado possível. |
| Mais quadros sempre significa menos faltas? | Em LRU e OPT sim. No FIFO nem sempre: é a anomalia de Belády. O exemplo `1 2 3 4 1 2 5 1 2 3 4 5` dá 9 faltas com 3 quadros e 10 com 4. |
| Por que `fifoPasso` existe separada? | Para o modo manual e o automático usarem exatamente a mesma lógica. |
| Por que a memória é copiada em cada passo? | O vetor muda a cada acesso. Sem a cópia, todos os passos mostrariam o estado final. |
| Por que RANDOM dá resultados diferentes? | Porque sorteia a vítima. No gráfico usa-se a média de várias execuções. |
| Qual a diferença entre hit em LRU e em FIFO? | No LRU o hit atualiza o último uso da página. No FIFO o hit não muda nada, porque só importa a ordem de chegada. |

## 12. Como o código reflete o enunciado

- Quatro funções separadas, com os protótipos pedidos: `fifo`, `lru`, `opt`, `random`.
- Modo manual FIFO com estado da memória, indicação de hit ou falta e fila atualizada.
- Modo automático com o total de faltas dos quatro algoritmos.
- Gráfico de faltas × quadros.
- Funções separadas e comentários, com a aparência em `style.css`.

> **Atenção:** o enunciado pede o simulador "em um único arquivo HTML". Com o CSS separado, a entrega passa a ter dois arquivos (`index.html` e `style.css`). Se o professor exigir arquivo único, basta colar o conteúdo do `style.css` dentro de uma tag `<style>` no `<head>`.
