## 2 Mutex e variáveis de condição

2.1 
O mutex protege o buffer e as variáveis de controle (`entrada`, `saida`, `ocupadas`) e é exigido por `pthread_cond_wait()`. As variáveis de condição permitem que a thread durma até que a condição mude (`nao_cheio` ou `nao_vazio`), em vez de ficar testando repetidamente.

2.2 `pthread_cond_wait()` libera o mutex e bloqueia a thread de forma atômica. Ao ser acordada, ela readquire o mutex antes de retornar. Por isso, a verificação da condição e a bloqueio não podem ser interrompidos por outra thread.

2.3 Com `while`, a condição é verificada novamente após cada despertar. Isso é necessário porque: (a) podem ocorrer despertares espúrios; (b) entre o `signal` e o momento em que a thread readquire o mutex, outra thread pode ter consumido ou ocupado o recurso. Com `if`, a thread prosseguiria sem que a condição estivesse verdadeira.

2.4 Não. `pthread_cond_signal()` apenas torna a thread sinalizada elegível para execução. Ela só retorna de `pthread_cond_wait()` depois de readquirir o mutex, que só fica disponível quando quem sinalizou o libera. O escalonador decide quando ela executa.

2.5 Não há espera ativa: a thread bloqueada não consome CPU enquanto aguarda. A implementação do Exercício 1 consome CPU continuamente nas tentativas repetidas. Em contrapartida, a versão com variáveis de condição tem o custo de chamadas de sinalização e de troca de contexto.

2.6 Não. O mutex continua necessário para tornar atômicas a verificação da condição e a alteração do estado, e para que `pthread_cond_wait()` funcione corretamente. Sem o mutex, haveria condições de corrida e perda de sinais (lost wakeup), ou seja, uma thread poderia dormir mesmo com a condição já satisfeita.

buffer de int