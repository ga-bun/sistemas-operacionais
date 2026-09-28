#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

// O wait() faz o processo pai bloquear até o filho terminar, garantindo sincronização.

int main() {
    pid_t pid = fork();

    if (pid == 0) {
        printf("Filho executando (PID: %d)\n", getpid());
        sleep(2); // simula trabalho
        printf("Filho terminou!\n");
    } else {
        printf("Pai esperando o filho...\n");
        wait(NULL); // espera o filho terminar
        printf("Pai detectou que o filho terminou.\n");
    }

    return 0;
}
