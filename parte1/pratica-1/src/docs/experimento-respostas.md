# Experimento 1
1. O utilitário strace do UNIX permite observar a sequência de chamadas de sistema efetuadas por uma aplicação. Em um terminal, execute strace date para descobrir quais os arquivos abertos pela execução do utilitário date (que indica a data e hora correntes). Por que o utilitário date precisa fazer chamadas de sistema?

- Sendo o Date um programa utilitário, pertencente à camada do Usuário/aplicações, ele não tem permissão para gerenciar o relógio do hardware. Quem faz isso é o núcleo do SO. Para ter acesso à essa informação, o Date precisa fazer um "pedido" ao núcleo por meio de algumas funções. Exemplo: execve(), brk(), read(), mmap(), close() que pode ser observadas em 
![Imagem com detalhes do Strace parte 1 strace date](./strace-and-ltrace/strace-1.png)

2. O utilitário ltrace do UNIX permite observar a sequência de chamadas de biblioteca efetuadas por uma aplicação. Em um terminal, execute ltrace date para descobrir as funções de biblioteca chamadas pela execução do utilitário date (que indica a data e hora correntes). Pode ser observada alguma relação entre as chamadas de biblioteca e as chamadas de sistema observadas no item anterior?

- Executando o comando não é possível ver muito bem a relação, visto que o comando só trouxe a data e a hora e um indicador de que o processo foi terminado, e não as chamadas das bibliotecas. 

- ![Imagem com o retorno da chamada ltrace](./strace-and-ltrace/ltrace.png)

--
# Experimento 2
Explique com suas palavras a diferença entre o que o comando ltrace intercepta e o que o strace intercepta. Por que o printf() precisa disparar a syscall write()?

- trace system calls and signals
http://www.man7.org/linux/man-pages/man1

- A library call tracer
https://www.man7.org/linux/man-pages/man1/ltrace.1.html

--
# Experimento 3
Pesquise, qual componente de hardware detecta a tentativa de acesso indevido à memória e
como o Kernel reage ao receber essa exceção

--
# Experimento 5
Observe o Terminal A: Note o salto instantâneo nas colunas in (interrupções).
• Qual era o valor aproximado da coluna in (interrupções/seg) com o sistema em re-
pouso?
• Para quanto esse valor subiu durante a execução do loop de E/S?

--
# Experimento 6

ToDo:
cat /proc/interrupts | grep -E "CPU0|SATA|nvme|timer|LOC"
cat /proc/ioports | head -n 25
cat /proc/dma
ou ver uma alternativa para rodar esses no docker

--
# Perguntas finais
1. Quais são as duas principais funções de um sistema operacional?
2. Instruções relacionadas ao acesso a dispositivos de E/S são tipicamente instruções privilegia-
das, isto é, podem ser executadas em modo núcleo, mas não em modo usuário. Dê uma razão
de por que essas instruções são privilegiadas.
3. Qual é a diferença entre modo núcleo e modo usuário? Explique como ter dois modos distintos
ajuda no projeto de um sistema operacional.
4. Quais das instruções a seguir devem ser deixadas somente em modo núcleo?
(a) Desabilitar todas as interrupções.
(b) Ler o relógio da hora do dia.
(c) Configurar o relógio da hora do dia.
(d) Mudar o mapa de memória
5. Quando um programa de usuário faz uma chamada de sistema para ler ou escrever um arquivo
de disco, ele fornece uma indicação de qual arquivo ele quer, um pon- teiro para o buffer de
dados e o contador. O controle é então transferido para o sistema operacional, que chama
o driver apropriado. Suponha que o driver começa o dis- co e termina quando ocorre uma
interrupção. No caso da leitura do disco, obviamente quem chamou terá de ser bloqueado
(pois não há dados para ele). E quanto a escrever para o disco? Quem chamou precisa ser
bloque- ado esperando o término da transferência de disco?