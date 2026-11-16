# RELATÓRIO: Laboratório de Problemas Clássicos - Parte 1

Gabriele de Carvalho 

## 1. Produtores e consumidores: solução clássica

### a) Qual é a função de cada um dos três mecanismos de sincronização?

- **Vagas (Semáforo):** 
    Serve para indicar quantas posições livres existem; ele é iniciado com o valor da capacidade do buffer. Quando o produtor insere algo, decrementa de vagas e quando o consumidor retira, incremnta vagas

- **Itens (Semáforo):**  
    Serve para indicar quantos itens ainda estão disponíveis; É iniciado em zero, pois nada foi produzido ainda. Quando o produtor insere algo, o item é incrementado e quando o consumidor retira, decrementa a quantidade de intens

- **Mutex:** 
    Serve para garantir que só uma thread altere o buffer de cada vez

### b) O que acontece quando o buffer fica cheio? E quando fica vazio?

- Buffer cheio: **vagas** fica igual a zero, os produtores não podem mais inserir nada (sem_wait(&vagas) e só serão liberados se um um consumidor retirar um item e liberar uma vaga
- Buffer vazio: **itens** fica igual a zero, os consumidores não conseguem retirar nada (sem_wait(&itens)) e só são liberados se um produtor insirerir um item e sinalizar que está disponível

### c) Por que somente o mutex não é suficiente para implementar convenientemente a espera por vagas e itens?

O mutex oferece exclusão mútua, ou seja, ele bloqueia acesso simultâneo à região crítica. Por não possuir contadores (vagas/itens). Quando uma thread encontra o buffer cheio/vazio, tem que liberar o mutex e retornar para o início do fluxo para conseguir bloquear o mutex novamente. Nisso, há desperdício de CPU, pode haver inanição ou até bloqueios


### d) O uso do mutex impede que produtores e consumidores realizem trabalho fora da região crítica simultaneamente? Justifique.

Não, não impede. O mutex protege só o trecho entre pthread_mutex_lock(&mutex) e pthread_mutex_unlock(&mutex), ou seja, em relação ao acesso ao buffer compartilhado. Tudo que é gerado/consumido (ou outras atividades) fora disso (região crítica) podem acontecer ao mesmo tempo, caso o CPU, núcleos do CPU ofereçam essa possibilidade


## 2. Leitores e escritores: solução simplista

### a) A implementação garante a integridade da região compartilhada?

Sim, garante, não tem concorrência à região crítica devido à exclusão mútua estrita (só uma thread acessa por vez) que o mutex impõe ali sobre a leitura e escrita do buffer compartilhado

### b) Dois leitores conseguem ler simultaneamente? Por quê?
Não, como dito na letra a, o mutex é exclusivo daquele que o bloqueou (pthread_mutex_lock(&mutex)). Outro leitor precisaria esperar que o leitor/escritor libere o mutex (pthread_mutex_unlock(&mutex))


### c) Por que essa solução é considerada simplista, mesmo sendo correta quanto à exclusão mútua?

Porque trata leitura e escrita da mesma forma, como se elas interferissem uma nas outras, mas operações de leitura só consultam os dados, não os alteram. Isso faz com que haja uma perda de desempenho ou operações podem até ser bloqueadas, pois pode haver situações em que o leitor demora muito tempo para realizar a operação, em ambos os casos a leitura/escrita poderia estar sendo feitas em paralelo.

### d) Que oportunidade de concorrência poderia ser explorada por uma solução mais elaborada?

Leitura simultânea: vários leitores podem acessar a área compartilhada ao mesmo tempo. Escrita exclusiva: exclusão mútua quando a thread escritora está modificando os dados.

## 3. Leitores e escritores: preferência aos leitores

### a) Por que o escritor pode permanecer bloqueado mesmo que leitores individuais terminem suas operações?

O escritor consegue pegar o acesso só quando o último leitor ativo (leitores_ativos == 0) executa a operação sem_post(&sem_acesso)/libera. Se caso outros leitores entrem antes que esse contador de leitores ativos chegue a zero, o acesso compartilhado se mantém para esses leitoresm ou seja, o semáforo continua bloqueado.

### b) A chegada de novos leitores é impedida quando há um escritor aguardando?

Não, o protocolo de preferência aos leitores, se tiver leitores ativos, os novos que entram incrementam o contador e entram direto na região compartilhada, sem precisar passar por sem_wait(&sem_acesso).

### c) Um tempo de espera muito longo demonstra, por si só, inanição indefinida? Explique a diferença entre evidência experimental e possibilidade teórica de starvation.

Não, só demonstra que teve atraso na execução. A inanição é uma possibilidade teórica do algoritmo, teoricamente, se chega muitos leitores (continuamente e infinitamente, sem pausas), o algoritmo não garante um limite de espera para o escritor, sendo assim, ele fica bloqueado de forma indefinida, já na evidência experimental, os programas reais são postos em teste e eles não funcionam infinitamente, então, eventualmente o escritor vai ser executado.

### d) Proponha uma alteração no protocolo que impeça a entrada indefinida de novos leitores quando houver um escritor aguardando.

- Adição de um semáforo que controla a ordem de chegada das tarefas (sem_entrada), inicializado em 1. 
- Tanto escritores quanto leitores precisam passar por esse semáforo (sem_wait(&sem_entrada)) antes de solicitar acesso à área compartilhada (sem_wait(&sem_acesso)).
- Quando um escritor chega, ele bloqueia sem_entrada, fechando a passagem para novos leitores; apenas os leitores que já estavam dentro continuam. O último leitor a sair libera a área compartilhada (sem_acesso) para o escritor entrar. Ao terminar de escrever, o próprio escritor libera a área compartilhada e o semáforo sem_entrada.
- O leitor, quando chega, adquire sem_entrada antes de verificar/bloquear a área compartilhada e libera sem_entrada logo após concluir seu protocolo de entrada. Se um escritor estiver aguardando em sem_entrada, todos os novos leitores ficam retidos atrás dele na fila, garantindo atendimento justo em ordem de chegada e a espera limitada."


## Registro de execução