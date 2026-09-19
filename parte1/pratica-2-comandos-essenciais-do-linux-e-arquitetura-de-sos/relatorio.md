# Pratica - Comandos essenciais do Linux e arquitetura de SOs

Gabriele de Carvalho

## Questão 22

### 22. Obtenha informações do sistema:

(a) O comando uname -a mostra informações sobre o kernel e o sistema operacional.
        
![uname -a](./01-atividade-questao-22/a-uname-a.png)

Qual é a versão do kernel em execução?

- **Versão 7, mais especificamente 7.0.0-31-generic**

(b) O comando uptime exibe há quanto tempo o sistema está ativo e a carga média.

![uptime](./01-atividade-questao-22/b-uptime.png)

Há quanto tempo o sistema está ligado?
- **1 minuto e 1 segundo**

(c) O comando dmesg | tail mostra as últimas mensagens do kernel, úteis para depuração.

![dmesg | tail](./01-atividade-questao-22/c-dmesg-tail.png)

Que tipo de mensagens aparecem no log?

- **Mensagens de audit. Indicando acessos negados pelo apparmor. A operação é de abrir arquivos relacionados ao etc/vulkan. Aparece o process Id também (pid) e o fsuid que corresponde à identidade do usuário**

(d) O comando vmstat exibe estatísticas de memória, processos e CPU.

![vmstat](./01-atividade-questao-22/d-vmstat.png)

Qual é a quantidade de memória livre?

- **2686080**

(e) O comando iostat mostra estatísticas de entrada/saída de disco.

![iostat](./01-atividade-questao-22/e-iostat.png)

Qual dispositivo apresenta maior atividade de I/O?

- **sda com maiores kb de leitura e escrita**

(f) O comando sar -u 1 3 coleta métricas de desempenho da CPU a cada segundo, por 3

![sar -u 1 3](./01-atividade-questao-22/f-sar-u-1-3.png)

Qual foi o percentual médio de uso da CPU?

- **6,32%**

(g) O comando who mostra usuários logados.

![who](./01-atividade-questao-22/g-who.png)

Quantos usuários estão logados?
- **A saída do comando who não ofereceu nenhum retorno. Mas temos o usuário aluno e também o root (veja saida do comando w)**

(h) O comando w mostra usuários e processos em execução.

![w](./01-atividade-questao-22/h-w.png)

Qual usuário consome mais recursos?
- **O usuário root, o JCPU é maior. JCPU é o tempo consumido por todos os processos não somente o que está rodando agora, não inclui processos terminados em bg, mas inclui processos em bg rodando agora**

(i) O comando id exibe UID, GID e grupos do usuário atual.

![id](./01-atividade-questao-22/i-id.png)

Qual é o UID do seu usuário?

- **0**

(j) O comando lscpu mostra informações da CPU (núcleos, threads, arquitetura).

![lscpu](./01-atividade-questao-22/j-lscpu.png)

Quantos núcleos físicos e lógicos existem?

- **4 núcleos físicos (CPU) e 4 núcleos lógicos (core per socket)**

(k) O comando cat /proc/cpuinfo mostra detalhes de cada núcleo.

![cat /proc/cpuinfo](./01-atividade-questao-22/k-cat-proc-cpuinfo-1.png)
![cat /proc/cpuinfo](./01-atividade-questao-22/k-cat-proc-cpuinfo-2.png)

Qual é o modelo da CPU?

- **AMD Ryzen 3 3200G with Radeon Vega Graphics**

(l) O comando nproc retorna a quantidade de núcleos disponíveis.

![nproc](./01-atividade-questao-22/l-nproc.png)

Quantos núcleos o sistema reconhece?

- **4**

(m) O comando free -h mostra memória total, usada e livre.

![free -h](./01-atividade-questao-22/m-free-h.png)

Qual é a quantidade de memória livre?

- **free: 215 MiB**

(n) O comando cat /proc/meminfo mostra informações detalhadas da RAM.

![cat /proc/meminfo](./01-atividade-questao-22/n-cat-proc-meminfo.png)

Qual é o valor de MemAvailable?

- **1805328kB**

(o) O comando lsblk lista discos e partições.

![lsblk](./01-atividade-questao-22/o-lsblk.png)

Quais partições estão montadas?

- **Partições reais montadas só o sda2 -> em /**
- **Os loops que qparecem não são partições físicas, mas estão montados em snap/...; aqui seriam 13**

(p) O comando df -h mostra uso de espaço em disco por partição.

![df -h](./01-atividade-questao-22/p-df-h.png)

Qual partição está mais cheia?
- **/dev/sda2, com 33% de uso**

---

## Questionário Arquitetura


1. Monte uma tabela com os benefícios e deficiências mais relevantes das principais arquiteturas de sistemas operacionais

| Arquitetura | Benefícios | Deficiências |
|---|---|---|
| **Sistemas monolíticos** | Desempenho devido à comunicação direta entre componentes do núcleo | Problemas de robustez do sistema, um erro pode propagar para todo o núcleo; Problemas com desenvolvimento: os componentes podem ser interdependentes, dificulta manutenção/evolução |
| **Sistemas micronúcleo** | Modularidade, serviços independentes; flexibilidade, serviços podem ser carregados/desativados individualmente; robustez, caso um falhe, só ele falha | Como todo aspecto de alto nível é implementado fora do núcleo, a comunicação por mensagem pode gerar um problema de desempenho, pois cada chamada de sistema tem custo elevado |
| **Sistemas em camadas** | Mais organizada, os níveis tem privilégios diferentes,  | Pode ter problemas de desempenho, devido ao empilhamento das camadas; A separação das funções nessas camadas às vezes não é tão simples, tem funcionalidades que podem estar ou devem estar em várias delas (ex: gestão de entrada/saida) |
| **Sistemas híbridos** | Traz de volta ao núcleo componentes mais críticos, ainda mantém certa modularidade (micronucleo/camadas) mas garante mais desempenho (monolito) | Aumenta a complexidade, não é tão claro a definição dos componentes como é em camadas/micronúcleo |
| **Máquinas virtuais (avançado)** | Isolamento, permite execução de código independente de plataforma, baixo custo | Sobrecarga do sistema físico que comporta essas máquinas virtuais |
| **Contêineres (avançado)** | Benefícios parecidos com os da máquina virtual, isolamento, aproveita bem o hardware porque separa em domínios (espaço de usuário) | Sobrecarga do sistema físico também, complexidade a mais em termos de gerência |
| **Sistemas exonúcleo** | Acesso controlado aos recursos do hardware, cada aplicação implementa o que for mais adequado, isso pode favorecer o desempenho da aplicação específica | Não há abstração, os componentes fora do núcleo devem aplicar tudo (LibOs), sobrecarga de desempenho para o sistema (chamadas de sistema para o núcleo), complexidade |
| **Sistemas uninúcleo (unikernel)** | O núcleo, bibliotecas e aplicação compilados e ligados enre si, aumenta muito desempenho. Só entra no código final, os componentes necessários, sistema compacto  | Qualquer mudança é necessário recompilação. A separação entre os componentes não existe, então não pode ser usada em sistemas que precisam de aplicações independentes |

---
2. Sobre as afirmações a seguir, relativas às diversas arquiteturas de sistemas operacionais, indique quais são incorretas, justificando sua resposta:

- (a) Uma máquina virtual de sistema é construída para suportar uma aplicação escrita em uma linguagem de programação específica, como Java. 

    **Incorreta, isso seria uma maquina virtual de aplicação (Hipervisor de aplicação), o de sistema (Hipervisor de sistema) suporta a execução de um sistema operacional convidado + aplicações**

- (b) Um hipervisor convidado executa sobre um sistema operacional hospedeiro.
    
    **Correto**

- (c) Em um sistema operacional micronúcleo, os diversos componentes do sistema são construídos como módulos interconectados executando dentro do núcleo.
    
    **Incorreto: isso seria monolítico. No micronúcleo, só os serviços essenciais ficam no núcleo, removendo toda os serviços de alto nível, deixando esses no espaço de usuário**

- (d) Núcleos monolíticos são muito utilizados devido à sua robustez e facilidade de manutenção. 
    
    **Incorreto: utilizados principalmente pelo desempenho devido à comunicação direta entre os componentes**

- (e) Em um sistema operacional micronúcleo, as chamadas de sistema são implementadas através de trocas de mensagens. 

    **Correto**

---

3. Pesquisa que tipo de arquitetura é mais apropriada para os seguintes sistemas (justifique o porque):

- (a) Sistemas Críticos (ex: Aeronáutica)

    **O exemplo da aeronáutica, é preciso de grande desempenho (resposta rápida) e modularizaçao, então nesse caso, a melhor arquitetura seria micronúcleo**

- (b) Servidores

    **A melhor opção seria o monolítico (no caso do Ubuntu server, monolítico modular) ou híbrido (Windows server), porque o foco é desempenho no gerenciamento de recursos/usuários e disponibilidade de informação, mas também segurança da informação**

- (c) Sistemas Distribuídos

    **Micronúcleo. Aqui o foco é a troca de mensagens entre computadores, compartilhamente de recursos entre essas máquinas, escalabilidade, heterogeneidade. Por isso o micronúcleo é o mais adequado, ele garante justamente a independencia dos serviços**

- (d) Sistemas Embarcados

    **Micronúcleo. Pegando como exemplo o QNX, utilizado também em sistemas críticos, o foco é resposta rápida (tempo real), multitarefa/multiusuário, estabilidade e segurança, mas também precisa ser um sistema que utiliza pouco espaço**

- (e) Sistemas de Tempo Real

    **Micronúcleo: Tem muito em comum com os sistemas embarcados e sistemas críticos, pois os dois usam muito de sistemas de Tempo Real (exemplo QNX que usei antes), aqui o foco é essa resposta em tempo real e a maioria dos serviços de tempo real/threads são implementados diretamente no núcleo, às vezes é dispensável serviços fora dele, eliminando a grande desvantagem desse sistema.**

- (f) Sistemas Móveis

    **Híbrido: Os sistemas móveis focam muito em equilíbrio/flexibilidade (bateria, segurança, conectividade, etc). Pegando como exemplo o sistema XNU, desenvolvido pela apple, ele contém características de arquitetura monolítica e micronúcleo**

---

## Atividade Minix

O sistema operacional Minix 3 usa uma arquitetura micronúcleo. Ele pode ser obtido gratuitamente no site http://www.minix3.org. Instale-o em uma máquina virtual e explore seus processos, usando os comandos top e ps.

Acesso o sistema via login e senha: root

Identifique os principais processos em execução, usando a documentação do site e o comando man.

Faça uma lista explicando o que cada um dos serviços faz

---

- **ps**: imprime na tela o status os processos ativos. Se usar o ps -e (ou -ax) é possível ver todos os processos dos terminais e fora deles

    ![ps](./03-atividade-minix/ps.png)

    ![ps-e](./03-atividade-minix/ps-e.png)


- **top**: imprime na tela de forma dinâmica (em tempo real). Indica um resumo de informações do sistema, lista de processos/threads sendo gerenciadas pelo núcleo

    ![top](./03-atividade-minix/top.png)

É possível ver que há 37 processos, somente em execução (running) e 36 dormindo (sleeping). O comando também mostra um relato da memória e detalhes do processador que está em basicamente ocioso (idle), 98%.

- **kernel**: sempre em primeiro, com o PID -1. É aqui que acontece as operações básicas do sistema

- **init**: inicio de todos os processos, é o primeiro programa que o SO inicia

- **tty**: retorna o nome do terminal do usuário

- **memory**: gerenciamento de memória

- **procfs**: informações sobre processos (/proc). Dá pra ver os processos (diretórios) em ls -la /proc

- **random**: fornece linhas aleatórias de arquivos ou números aleatórios.

- Não encotrados no man: **vfs, mfs**, podem estão relacionados com filesystem; **vm** provavel que estejam relacionados ao gerenciamento da memória virtual; **pm**; **rs**; **sched**; **pfs**; **vbox**