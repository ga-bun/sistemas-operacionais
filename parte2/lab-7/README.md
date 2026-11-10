# Laboratório 7 - Problemas clássicos de sincronização

## Compilação

```bash
cd parte2/lab-7

gcc -Wall -Wextra -pthread parte1-produtor-consumidor.c -o parte1-produtor-consumidor
gcc -Wall -Wextra -pthread parte2-leitores-escritores-simplista.c -o parte2-leitores-escritores-simplista
gcc -Wall -Wextra -pthread parte3-leitores-escritores-versao-a.c -o parte3-leitores-escritores-versao-a
gcc -Wall -Wextra -pthread parte3-leitores-escritores-versao-b.c -o parte3-leitores-escritores-versao-b
```

## Execução

```bash
./parte1-produtor-consumidor
./parte2-leitores-escritores-simplista
./parte3-leitores-escritores-versao-a
./parte3-leitores-escritores-versao-b
```

## Relatório breve

### Parte 1 - Produtor/Consumidor
- O semáforo `vagas` controla espaço livre no buffer.
- O semáforo `itens` controla itens disponíveis para leitura.
- O mutex protege as operações sobre o buffer e as variáveis `head`, `tail` e `ocupadas`.
- O buffer nunca é sobrescrito antes do consumo porque `sem_wait(&vagas)` bloqueia o produtor quando o buffer está cheio e `sem_wait(&itens)` bloqueia o consumidor quando está vazio.

### Parte 2 - Leitores/Escritores simplista
- O mutex garante exclusão mútua sobre a região crítica.
- Como o mutex é mantido durante toda a leitura e toda a escrita, os leitores não leem simultaneamente.
- A solução é simples e correta, mas pouco eficiente: a concorrência é praticamente anulada, já que a leitura demorada bloqueia todos os demais acessos.

### Parte 3 - Preferência aos leitores
- O mutex protege a contagem de leitores ativos.
- O semáforo binário `acesso` garante exclusividade para o escritor.
- O primeiro leitor adquire o semáforo para impedir a entrada do escritor; o último leitor o libera.
- Na versão B, leitores entram repetidamente e permanecem lendo por mais tempo, o que aumenta o tempo de espera do escritor e evidencia o efeito da preferência aos leitores.

## Observações

- A solução da parte 1 garante integridade do buffer e mantém progresso sem deadlock.
- A parte 2 é uma solução didática, adequada para demonstrar a exclusão mútua, mas não explora concorrência real.
- A parte 3 ilustra o comportamento clássico de leitores com preferência aos leitores e a possibilidade experimental de starvation do escritor quando há influxo contínuo de leitores.
