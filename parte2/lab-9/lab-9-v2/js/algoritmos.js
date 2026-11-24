/* ===================================================================
   Este arquivo só tem lógica: não mexe na tela.
   Cada função recebe a sequência de referências e o número de quadros
   e devolve { faltas, passos }. Cada passo registra o estado da memória
   depois de um acesso: { pagina, falta, vitima, memoria }.
   =================================================================== */

function memoriaVazia(quadros) {
  return new Array(quadros).fill(null);
}

function criarPasso(pagina, falta, vitima, memoria) {
  return { pagina, falta, vitima, memoria: [...memoria] };
}

function criarResultado(passos) {
  return { passos, faltas: passos.filter((p) => p.falta).length };
}

// Posição (na memória) da página com o menor valor de "chave".
function indiceDoMenor(memoria, chave) {
  let melhor = 0;
  for (let j = 1; j < memoria.length; j++) {
    if (chave(memoria[j]) < chave(memoria[melhor])) melhor = j;
  }
  return melhor;
}

// Posição (na memória) da página com o maior valor de "chave".
function indiceDoMaior(memoria, chave) {
  let melhor = 0;
  for (let j = 1; j < memoria.length; j++) {
    if (chave(memoria[j]) > chave(memoria[melhor])) melhor = j;
  }
  return melhor;
}

/* ---------- FIFO: sai a página que está há mais tempo na memória ---------- */

function criarEstadoFifo(quadros) {
  return { memoria: memoriaVazia(quadros), fila: [] };
}

// Processa UM acesso. É usada pelo fifo() e também pelo modo manual.
function fifoPasso(estado, pagina) {
  const { memoria, fila } = estado;
  let falta = false;
  let vitima = null;

  if (!memoria.includes(pagina)) {
    falta = true;
    let posicao = memoria.indexOf(null); // quadro livre?
    if (posicao === -1) {
      vitima = fila.shift();             // a mais antiga sai da fila
      posicao = memoria.indexOf(vitima);
    }
    memoria[posicao] = pagina;
    fila.push(pagina);                   // a nova entra no fim da fila
  }

  const passo = criarPasso(pagina, falta, vitima, memoria);
  passo.fila = [...fila];
  return passo;
}

function fifo(seq, quadros) {
  const estado = criarEstadoFifo(quadros);
  return criarResultado(seq.map((pagina) => fifoPasso(estado, pagina)));
}

/* ---------- LRU: sai a página usada há mais tempo ---------- */

function lru(seq, quadros) {
  const memoria = memoriaVazia(quadros);
  const ultimoUso = {}; // página -> posição da sequência do último acesso
  const passos = [];

  seq.forEach((pagina, i) => {
    let falta = false;
    let vitima = null;

    if (!memoria.includes(pagina)) {
      falta = true;
      let posicao = memoria.indexOf(null);
      if (posicao === -1) {
        posicao = indiceDoMenor(memoria, (p) => ultimoUso[p]); // último uso mais distante
        vitima = memoria[posicao];
      }
      memoria[posicao] = pagina;
    }

    ultimoUso[pagina] = i; // hit ou falta, a página acabou de ser usada
    passos.push(criarPasso(pagina, falta, vitima, memoria));
  });

  return criarResultado(passos);
}

/* ---------- OPT (Ótimo / Belády): sai a página cujo próximo uso é o mais distante ---------- */

// Posição do próximo acesso a "pagina" depois de "depoisDe"; Infinity se nunca mais for usada.
function proximoUso(seq, depoisDe, pagina) {
  for (let k = depoisDe + 1; k < seq.length; k++) {
    if (seq[k] === pagina) return k;
  }
  return Infinity;
}

function opt(seq, quadros) {
  const memoria = memoriaVazia(quadros);
  const passos = [];

  seq.forEach((pagina, i) => {
    let falta = false;
    let vitima = null;

    if (!memoria.includes(pagina)) {
      falta = true;
      let posicao = memoria.indexOf(null);
      if (posicao === -1) {
        posicao = indiceDoMaior(memoria, (p) => proximoUso(seq, i, p)); // olha o futuro
        vitima = memoria[posicao];
      }
      memoria[posicao] = pagina;
    }

    passos.push(criarPasso(pagina, falta, vitima, memoria));
  });

  return criarResultado(passos);
}

/* ---------- RANDOM: sai uma página sorteada ---------- */

function random(seq, quadros) {
  const memoria = memoriaVazia(quadros);
  const passos = [];

  seq.forEach((pagina) => {
    let falta = false;
    let vitima = null;

    if (!memoria.includes(pagina)) {
      falta = true;
      let posicao = memoria.indexOf(null);
      if (posicao === -1) {
        posicao = Math.floor(Math.random() * quadros);
        vitima = memoria[posicao];
      }
      memoria[posicao] = pagina;
    }

    passos.push(criarPasso(pagina, falta, vitima, memoria));
  });

  return criarResultado(passos);
}

// Liga o nome de cada algoritmo à sua função.
const ALGORITMOS = { fifo, lru, opt, random };
