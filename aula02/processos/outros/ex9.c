#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>


/** 
 * 
O processo raiz cria três filhos.

Cada filho é substituído por um programa diferente (date, whoami, pwd).

O pai usa wait(NULL) três vezes para garantir que todos os filhos terminem antes de encerrar.

Isso forma uma árvore de processos onde cada ramo executa um comando distinto.
 *  */ 

int main() {
    printf("Processo raiz PID=%d\n", getpid());

    pid_t pid1 = fork();
    if (pid1 == 0) {
        // Filho 1 executa "date"
        char *args[] = {"/bin/date", NULL};
        execve("/bin/date", args, NULL);
        perror("execve date falhou");
        return 1;
    }

    pid_t pid2 = fork();
    if (pid2 == 0) {
        // Filho 2 executa "whoami"
        char *args[] = {"/usr/bin/whoami", NULL};
        execve("/usr/bin/whoami", args, NULL);
        perror("execve whoami falhou");
        return 1;
    }

    pid_t pid3 = fork();
    if (pid3 == 0) {
        // Filho 3 executa "pwd"
        char *args[] = {"/bin/pwd", NULL};
        execve("/bin/pwd", args, NULL);
        perror("execve pwd falhou");
        return 1;
    }

    // Pai espera todos os filhos
    wait(NULL);
    wait(NULL);
    wait(NULL);

    printf("Pai PID=%d detectou que todos os filhos terminaram.\n", getpid());
    return 0;
}
