#include <stdio.h>
#include <unistd.h>

// Fork() cria um novo processo. O retorno é 0 no filho e o PID do filho no pai.

int main() {
    printf("Processo original (PID: %d)\n", getpid());

    pid_t pid = fork();

    if (pid == 0) {
        // Código do processo filho
        printf("Sou o processo filho (PID: %d)\n", getpid());
    } else {
        // Código do processo pai
        printf("Sou o processo pai (PID: %d), criei o filho %d\n", getpid(), pid);
    }

    return 0;
}
