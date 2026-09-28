#include <stdio.h>
#include <unistd.h>

// O execve() substitui o processo filho pelo programa ls -l. Se funcionar, o código após execve() não é executado.

int main() {
    pid_t pid = fork();

    if (pid == 0) {
        // Processo filho substitui sua imagem por outro programa
        char *args[] = {"/bin/ls", "-l", NULL};
        execve("/bin/ls", args, NULL);
        perror("execve falhou"); // só aparece se der erro
    } else {
        printf("Pai continua executando (PID: %d)\n", getpid());
    }

    return 0;
}
