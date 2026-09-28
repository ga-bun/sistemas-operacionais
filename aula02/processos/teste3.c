#include <stdio.h>
#include <unistd.h>

int main() {
    printf("Processo raiz PID=%d\n", getpid());

    // Criar alguns filhos
    for (int i = 0; i < 3; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            // Cada filho entra em loop infinito
            while (1) {
                // Apenas para não consumir 100% CPU, dorme um pouco
                sleep(5);
            }
        }
    }

    // O pai também entra em loop infinito
    while (1) {
        sleep(5);
    }

    return 0;
}