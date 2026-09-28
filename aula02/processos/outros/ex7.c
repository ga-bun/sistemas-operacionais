#include <stdio.h>
#include <unistd.h>

//  árvore de 3 processos: o pai, filho 1 e filho 2.


int main() {
    printf("Processo raiz PID=%d\n", getpid());

    pid_t pid1 = fork();
    if (pid1 == 0) {
        printf("Filho 1 PID=%d\n", getpid());
    } else {
        pid_t pid2 = fork();
        if (pid2 == 0) {
            printf("Filho 2 PID=%d\n", getpid());
        } else {
            printf("Pai PID=%d criou dois filhos: %d e %d\n", getpid(), pid1, pid2);
        }
    }

    return 0;
}
