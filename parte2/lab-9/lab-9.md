Curso Tecnologia em ADS
Sistemas Operacionais
Miniprojeto: Implementação de um Simulador de Substituição de
Páginas em HTML e JavaScript
Professor: Eraldo Silveira e Silva
1 Objetivos
Este trabalho tem como objetivo desenvolver um simulador de algoritmos de substitui-
ção de páginas utilizando HTML e JavaScript. O simulador deverá permitir:
• A entrada da quantidade de quadros da memória física (mínimo 1).
• A entrada da sequência de referências à páginas (ex.: 7 0 1 2 0 3 0 4).
• A execução dos quatro algoritmos clássicos de substituição:
– FIFO (First-In First-Out)
– LRU (Least Recently Used)
– OPT (Ótimo / Belády)
– RANDOM
• Um modo manual para demonstrar passo a passo a evolução do algoritmo FIFO.
• Um modo automático que calcula o total de falhas de página para todos os algoritmos.
Modo gerador de gráfico: mostra um gráfico similar ao apresentado por Maziero (Maziero,
2018).
• A organização dos algoritmos em funções JavaScript separadas.
Além da implementação, cada equipe deverá preparar slides explicando o funcionamento
de cada algoritmo.
2 Descrição Geral do Simulador
O simulador deverá ser implementado em um único arquivo HTML contendo:
• Um campo para entrada do número de quadros.
• Um campo para entrada da sequência de páginas.
• Botões para:
– Executar algoritmos automaticamente;
– Ativar o modo manual para o algoritmo FIFO.
• Uma área para exibição dos resultados.
IFSC – CAMPUS SÃO JOSÉ 1
3 Especificação dos Algoritmos
Cada algoritmo deverá ser implementado em uma função JavaScript específica, seguindo
os protótipos:
function fifo(seq, quadros) { ... }
function lru(seq, quadros) { ... }
function opt(seq, quadros) { ... }
function random(seq, quadros) { ... }
A seguir, um resumo conceitual dos algoritmos.
3.1 FIFO
O algoritmo FIFO substitui a página que está há mais tempo na memória, baseado no
princípio de fila (First-In First-Out).
• Mantém uma fila de páginas carregadas.
• Quando ocorre uma falta:
– Se há espaço livre, insere no fim da fila.
– Caso contrário, remove a mais antiga e insere a nova.
3.2 LRU
Substitui a página que foi menos recentemente usada. O algoritmo deve:
• Verificar o histórico da sequência até o ponto atual.
• Identificar o último uso de cada página na memória.
• Substituir a página cujo tempo de uso passado é o mais distante.
3.3 OPT (Ótimo)
Substitui a página cujo próximo uso ocorrerá mais tarde no futuro.
• Percorre a sequência à frente para prever qual página será menos necessária no futuro.
• É um algoritmo teórico: produz o menor possível número de falhas.
3.4 RANDOM
Escolhe de forma randômica.
IFSC – CAMPUS SÃO JOSÉ 2
4 Modo Manual (FIFO)
O simulador deverá permitir ao aluno clicar cada página da sequência e visualizar, passo a
passo:
• O estado da memória após cada acesso.
• Indicação de hit ou falta.
• A fila FIFO sendo atualizada.
Este modo será usado em sala de aula para explicar visualmente a substituição por FIFO.
5 Modo Automático
O simulador deverá implementar:
• Execução completa dos quatro algoritmos com a sequência fornecida.
• Exibição do número total de faltas de página para:
– FIFO
– LRU
– OPT
– RANDOM
6 Entrega
A equipe deverá entregar:
1. O arquivo index.html com todo o simulador funcionando.
2. Um vídeo curto demonstrando o modo manual.
3. Um conjunto de slides explicando cada algoritmo:
• Funcionamento básico;
• Exemplos;
• Comparação entre algoritmos;
• Resultados do simulador.
4. Identificação da equipe e repositório Git utilizado.
7 Modo Gerador de Gráfico
Um gráfico similar ao apresentado em Maziero (Maziero, 2018) deve ser gerado a partir da
simualação. Apresentar o gráfico nos slides.
IFSC – CAMPUS SÃO JOSÉ 3
8 Observações
• O código deve estar organizado, com funções separadas.
• Comentários são recomendados para clareza.
• A equipe pode usar CSS para melhorar a aparência do simulador.
IFSC – CAMPUS SÃO JOSÉ 4