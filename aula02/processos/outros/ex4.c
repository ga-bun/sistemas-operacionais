#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>


// o pai espera o filho terminar de rodar o comando date antes de continuar.
int main() {
    pid_t pid = fork();

    if (pid == 0) {
        char *args[] = {"/bin/date", NULL};
        execve("/bin/date", args, NULL);
        perror("execve falhou");
    } else {
        wait(NULL); // pai espera o filho terminar
        printf("Pai: o filho terminou de executar o programa.\n");
    }

    return 0;
}
