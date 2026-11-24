/* ===================================================================
   Interface do simulador (usa as funções de algoritmos.js)
   =================================================================== */

const NOMES = { fifo: 'FIFO', lru: 'LRU', opt: 'OPT', random: 'RANDOM' };
const CORES = { fifo: '#d9480f', lru: '#1971c2', opt: '#2f9e44', random: '#7048e8' };
const RODADAS_RANDOM = 200; // execuções usadas para a média do RANDOM no gráfico

/* ---------- Entrada e utilitários de tela ---------- */

const $ = (id) => document.getElementById(id);

function el(tag, classe, texto) {
  const no = document.createElement(tag);
  if (classe) no.className = classe;
  if (texto !== undefined) no.textContent = String(texto);
  return no;
}

function lerEntrada() {
  const quadros = Number($('quadros').value);
  if (!Number.isInteger(quadros) || quadros < 1 || quadros > 50) {
    throw new Error('O número de quadros deve ser um inteiro entre 1 e 50.');
  }

  const partes = $('sequencia').value.split(/[\s,;]+/).filter(Boolean);
  if (partes.length === 0) {
    throw new Error('Informe a sequência de referências (ex.: 7 0 1 2 0 3 0 4).');
  }
  const seq = partes.map(Number);
  if (seq.some((n) => !Number.isInteger(n) || n < 0)) {
    throw new Error('A sequência deve conter apenas números inteiros não negativos.');
  }
  return { seq, quadros };
}

// Executa uma ação e mostra na tela qualquer erro de entrada.
function comTratamentoDeErro(acao) {
  return () => {
    $('erro').textContent = '';
    try {
      acao();
    } catch (erro) {
      $('erro').textContent = erro.message;
    }
  };
}

// Tabela no estilo do livro: uma linha por quadro, uma coluna por referência.
function criarTabelaDeExecucao(passos, quadros) {
  const tabela = el('table');

  const cabecalho = el('tr');
  cabecalho.append(el('th', '', 'Ref.'));
  passos.forEach((p) => cabecalho.append(el('th', '', p.pagina)));
  tabela.append(cabecalho);

  for (let j = 0; j < quadros; j++) {
    const linha = el('tr');
    linha.append(el('th', '', `Q${j}`));
    passos.forEach((p, i) => {
      const anterior = i > 0 ? passos[i - 1].memoria[j] : null;
      const mudou = p.falta && p.memoria[j] !== anterior;
      linha.append(el('td', mudou ? 'novo' : '', p.memoria[j] === null ? '' : p.memoria[j]));
    });
    tabela.append(linha);
  }

  const resultado = el('tr');
  resultado.append(el('th', '', 'Acesso'));
  passos.forEach((p) => resultado.append(el('td', p.falta ? 'falta' : 'hit', p.falta ? 'F' : 'hit')));
  tabela.append(resultado);

  return tabela;
}

/* ---------- Modo automático ---------- */

function executarAutomatico() {
  const { seq, quadros } = lerEntrada();
  const resultados = {};
  Object.keys(ALGORITMOS).forEach((nome) => {
    resultados[nome] = ALGORITMOS[nome](seq, quadros);
  });

  mostrarResumo(resultados, seq.length);
  mostrarTabelas(resultados, quadros);
  $('secAuto').hidden = false;
}

function mostrarResumo(resultados, totalAcessos) {
  const menor = Math.min(...Object.values(resultados).map((r) => r.faltas));

  const tabela = el('table');
  const cabecalho = el('tr');
  ['Algoritmo', 'Faltas de página', 'Hits', 'Taxa de faltas'].forEach((t) => cabecalho.append(el('th', '', t)));
  tabela.append(cabecalho);

  Object.keys(resultados).forEach((nome) => {
    const { faltas } = resultados[nome];
    const linha = el('tr', faltas === menor ? 'melhor' : '');
    linha.append(
      el('th', '', NOMES[nome]),
      el('td', '', faltas),
      el('td', '', totalAcessos - faltas),
      el('td', '', `${((faltas / totalAcessos) * 100).toFixed(1)}%`)
    );
    tabela.append(linha);
  });

  $('resumo').replaceChildren(
    el('p', 'suave', `${totalAcessos} acessos. Em verde: menor número de faltas. RANDOM muda a cada execução.`),
    tabela
  );
}

function mostrarTabelas(resultados, quadros) {
  const caixa = $('tabelas');
  caixa.replaceChildren();

  Object.keys(resultados).forEach((nome) => {
    const { passos, faltas } = resultados[nome];
    const rolagem = el('div', 'rolagem');
    rolagem.append(criarTabelaDeExecucao(passos, quadros));
    caixa.append(el('h3', '', `${NOMES[nome]} – ${faltas} faltas`), rolagem);
  });
}

/* ---------- Modo manual (FIFO) ---------- */

let manual = null; // { seq, quadros, estado, indice, passos }

function iniciarManual() {
  const { seq, quadros } = lerEntrada();
  manual = { seq, quadros, estado: criarEstadoFifo(quadros), indice: 0, passos: [] };
  $('secManual').hidden = false;
  desenharManual();
}

function avancarManual(posicaoClicada) {
  if (manual === null || manual.indice >= manual.seq.length) return;
  if (posicaoClicada !== manual.indice) return; // só a próxima página da sequência pode ser acessada

  manual.passos.push(fifoPasso(manual.estado, manual.seq[manual.indice]));
  manual.indice += 1;
  desenharManual();
}

function desenharManual() {
  desenharSequenciaManual();
  desenharEstadoManual();
  $('manualTabela').replaceChildren(
    manual.passos.length > 0 ? criarTabelaDeExecucao(manual.passos, manual.quadros) : el('p', 'suave', 'Nenhum acesso ainda.')
  );
  $('btnProximo').disabled = manual.indice >= manual.seq.length;
}

function desenharSequenciaManual() {
  const caixa = $('manualSequencia');
  caixa.replaceChildren();

  manual.seq.forEach((pagina, i) => {
    const botao = el('button', 'chip', pagina);
    if (i < manual.indice) {
      botao.classList.add('feito', manual.passos[i].falta ? 'falta' : 'hit');
    } else if (i === manual.indice) {
      botao.classList.add('atual');
    } else {
      botao.classList.add('futuro');
    }
    botao.disabled = i !== manual.indice;
    botao.addEventListener('click', () => avancarManual(i));
    caixa.append(botao);
  });
}

function desenharEstadoManual() {
  const caixa = $('manualEstado');
  caixa.replaceChildren();

  const total = manual.passos.length;
  const faltas = manual.passos.filter((p) => p.falta).length;
  const contadores = el('div', 'contadores');
  contadores.append(el('span', '', `Acessos: ${total}`), el('span', '', `Faltas: ${faltas}`), el('span', '', `Hits: ${total - faltas}`));
  caixa.append(contadores);

  if (total === 0) {
    caixa.append(el('div', 'mensagem', 'Clique na primeira página da sequência para começar.'));
    return;
  }

  const ultimo = manual.passos[total - 1];
  const anterior = total > 1 ? manual.passos[total - 2].memoria : memoriaVazia(manual.quadros);

  const linhaMemoria = el('div', 'linha-estado');
  linhaMemoria.append(el('span', 'rotulo', 'Memória (quadros):'));
  ultimo.memoria.forEach((pagina, j) => {
    const mudou = ultimo.falta && pagina !== anterior[j];
    const classe = pagina === null ? 'caixa vazia' : mudou ? 'caixa novo' : 'caixa';
    linhaMemoria.append(el('span', classe, pagina === null ? '—' : pagina));
  });

  const linhaFila = el('div', 'linha-estado');
  linhaFila.append(el('span', 'rotulo', 'Fila FIFO:'));
  if (ultimo.fila.length === 0) {
    linhaFila.append(el('span', 'caixa vazia', 'vazia'));
  } else {
    linhaFila.append(el('span', 'seta', 'sai ←'));
    ultimo.fila.forEach((pagina) => linhaFila.append(el('span', 'caixa', pagina)));
    linhaFila.append(el('span', 'seta', '← entra'));
  }

  caixa.append(el('div', `mensagem ${ultimo.falta ? 'falta' : 'hit'}`, mensagemDoPasso(ultimo)), linhaMemoria, linhaFila);

  if (manual.indice >= manual.seq.length) {
    caixa.append(el('div', 'mensagem', `Fim da sequência: ${faltas} faltas de página em ${total} acessos.`));
  }
}

function mensagemDoPasso(passo) {
  if (!passo.falta) {
    return `Página ${passo.pagina}: HIT – já está na memória (a fila não muda).`;
  }
  if (passo.vitima === null) {
    return `Página ${passo.pagina}: FALTA – há quadro livre, a página é carregada e entra no fim da fila.`;
  }
  return `Página ${passo.pagina}: FALTA – memória cheia; sai a mais antiga (${passo.vitima}) e entra a ${passo.pagina}.`;
}

/* ---------- Gráfico: faltas de página × número de quadros ---------- */

let ultimoGrafico = null; // { series, maxQuadros }, usado para redesenhar ao redimensionar

// RANDOM varia a cada execução; no gráfico usa-se a média de várias rodadas.
function mediaRandom(seq, quadros) {
  let soma = 0;
  for (let r = 0; r < RODADAS_RANDOM; r++) soma += random(seq, quadros).faltas;
  return soma / RODADAS_RANDOM;
}

function calcularSeries(seq, maxQuadros) {
  const series = { fifo: [], lru: [], opt: [], random: [] };
  for (let q = 1; q <= maxQuadros; q++) {
    series.fifo.push(fifo(seq, q).faltas);
    series.lru.push(lru(seq, q).faltas);
    series.opt.push(opt(seq, q).faltas);
    series.random.push(mediaRandom(seq, q));
  }
  return series;
}

function gerarGrafico() {
  const { seq } = lerEntrada();
  const distintas = new Set(seq).size;
  // Com tantos quadros quanto páginas distintas, só há faltas obrigatórias.
  const maxQuadros = Math.min(Math.max(distintas, 2), 40);
  const series = calcularSeries(seq, maxQuadros);

  $('secGrafico').hidden = false;
  ultimoGrafico = { series, maxQuadros };
  desenharGrafico($('grafico'), series, maxQuadros);

  mostrarAvisoBelady(series.fifo);
  mostrarDadosDoGrafico(series, maxQuadros);
}

function desenharGrafico(canvas, series, maxQuadros) {
  const dpr = window.devicePixelRatio || 1;
  const largura = canvas.clientWidth;
  const altura = 400;
  canvas.width = largura * dpr;
  canvas.height = altura * dpr;

  const ctx = canvas.getContext('2d');
  ctx.scale(dpr, dpr);
  ctx.fillStyle = '#fff';
  ctx.fillRect(0, 0, largura, altura);

  const margem = { esq: 60, dir: 25, topo: 50, base: 55 };
  const w = largura - margem.esq - margem.dir;
  const h = altura - margem.topo - margem.base;

  const maiorValor = Math.max(1, Math.ceil(Math.max(...Object.values(series).flat())));
  const passoY = Math.max(1, Math.ceil(maiorValor / 10));
  const topoY = Math.ceil(maiorValor / passoY) * passoY;

  const px = (q) => (maxQuadros === 1 ? margem.esq + w / 2 : margem.esq + ((q - 1) / (maxQuadros - 1)) * w);
  const py = (v) => margem.topo + h - (v / topoY) * h;

  // Grade e rótulos do eixo Y
  ctx.font = '12px system-ui, sans-serif';
  ctx.textAlign = 'right';
  ctx.textBaseline = 'middle';
  for (let v = 0; v <= topoY; v += passoY) {
    ctx.strokeStyle = '#e9ecef';
    ctx.beginPath();
    ctx.moveTo(margem.esq, py(v));
    ctx.lineTo(margem.esq + w, py(v));
    ctx.stroke();
    ctx.fillStyle = '#495057';
    ctx.fillText(String(v), margem.esq - 8, py(v));
  }

  // Rótulos do eixo X
  ctx.textAlign = 'center';
  ctx.textBaseline = 'top';
  const passoX = Math.ceil(maxQuadros / 20);
  for (let q = 1; q <= maxQuadros; q++) {
    if ((q - 1) % passoX !== 0) continue;
    ctx.fillStyle = '#495057';
    ctx.fillText(String(q), px(q), margem.topo + h + 8);
  }

  // Eixos
  ctx.strokeStyle = '#343a40';
  ctx.lineWidth = 1.5;
  ctx.beginPath();
  ctx.moveTo(margem.esq, margem.topo);
  ctx.lineTo(margem.esq, margem.topo + h);
  ctx.lineTo(margem.esq + w, margem.topo + h);
  ctx.stroke();

  ctx.fillStyle = '#212529';
  ctx.font = '13px system-ui, sans-serif';
  ctx.fillText('Número de quadros', margem.esq + w / 2, altura - 22);
  ctx.save();
  ctx.translate(16, margem.topo + h / 2);
  ctx.rotate(-Math.PI / 2);
  ctx.fillText('Faltas de página', 0, 0);
  ctx.restore();

  // Linhas dos algoritmos
  Object.keys(series).forEach((nome) => {
    ctx.strokeStyle = CORES[nome];
    ctx.fillStyle = CORES[nome];
    ctx.lineWidth = 2.5;
    ctx.setLineDash(nome === 'random' ? [6, 4] : []);

    ctx.beginPath();
    series[nome].forEach((valor, k) => {
      if (k === 0) ctx.moveTo(px(k + 1), py(valor));
      else ctx.lineTo(px(k + 1), py(valor));
    });
    ctx.stroke();

    ctx.setLineDash([]);
    series[nome].forEach((valor, k) => {
      ctx.beginPath();
      ctx.arc(px(k + 1), py(valor), 3.5, 0, Math.PI * 2);
      ctx.fill();
    });
  });

  // Legenda
  ctx.textAlign = 'left';
  ctx.textBaseline = 'middle';
  let x = margem.esq;
  Object.keys(series).forEach((nome) => {
    ctx.fillStyle = CORES[nome];
    ctx.fillRect(x, 16, 22, 4);
    ctx.fillStyle = '#212529';
    ctx.fillText(NOMES[nome], x + 28, 18);
    x += 110;
  });

  $('btnBaixar').href = canvas.toDataURL('image/png');
}

// Anomalia de Belády: com mais quadros, o FIFO teve mais faltas.
function mostrarAvisoBelady(seriesFifo) {
  const caixa = $('avisoBelady');
  caixa.replaceChildren();

  for (let q = 2; q <= seriesFifo.length; q++) {
    if (seriesFifo[q - 1] > seriesFifo[q - 2]) {
      caixa.append(el('div', 'aviso',
        `Anomalia de Belády: o FIFO teve ${seriesFifo[q - 2]} faltas com ${q - 1} quadros e ${seriesFifo[q - 1]} faltas com ${q} quadros.`));
      return;
    }
  }
}

function mostrarDadosDoGrafico(series, maxQuadros) {
  const tabela = el('table');
  const cabecalho = el('tr');
  cabecalho.append(el('th', '', 'Quadros'));
  Object.keys(series).forEach((nome) => cabecalho.append(el('th', '', NOMES[nome])));
  tabela.append(cabecalho);

  for (let q = 1; q <= maxQuadros; q++) {
    const linha = el('tr');
    linha.append(el('th', '', q));
    Object.keys(series).forEach((nome) => {
      const valor = series[nome][q - 1];
      linha.append(el('td', '', Number.isInteger(valor) ? valor : valor.toFixed(1)));
    });
    tabela.append(linha);
  }
  $('dadosGrafico').replaceChildren(tabela);
}

/* ---------- Ligação dos botões ---------- */

$('btnAuto').addEventListener('click', comTratamentoDeErro(executarAutomatico));
$('btnManual').addEventListener('click', comTratamentoDeErro(iniciarManual));
$('btnGrafico').addEventListener('click', comTratamentoDeErro(gerarGrafico));
$('btnProximo').addEventListener('click', () => manual && avancarManual(manual.indice));
$('btnReiniciar').addEventListener('click', comTratamentoDeErro(iniciarManual));

$('btnExemplo').addEventListener('click', () => {
  $('quadros').value = 3;
  $('sequencia').value = '7 0 1 2 0 3 0 4 2 3 0 3 2 1 2 0 1 7 0 1';
});

// Com 3 quadros o FIFO tem 9 faltas; com 4 quadros, 10.
$('btnBelady').addEventListener('click', () => {
  $('quadros').value = 3;
  $('sequencia').value = '1 2 3 4 1 2 5 1 2 3 4 5';
});

window.addEventListener('resize', () => {
  if (ultimoGrafico && !$('secGrafico').hidden) {
    desenharGrafico($('grafico'), ultimoGrafico.series, ultimoGrafico.maxQuadros);
  }
});
