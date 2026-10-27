// Exercício 4 – Fim de Arquivo (EOF)
// Implemente um programa em que o escritor envia dados e fecha o pipe; o leitor deverá ler
// até detectar EOF.
// Explique: quando read() retorna zero e o papel do fechamento de todas as extremidades
// de escrita

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
    const char *mensagens[] = {"Mensagem 1", "Mensagem 2", "Mensagem 3"};
    int fd[2];
    char buffer[128];
    ssize_t n;
    pid_t pid;
    int i;

    setvbuf(stdout, NULL, _IONBF, 0);

    if (pipe(fd) == -1) {
        perror("pipe");
        return EXIT_FAILURE;
    }

    pid = fork();
    if (pid == -1) {
        perror("fork");
        return EXIT_FAILURE;
    }

    if(pid == 0){
        close(fd[0]);
        for (i = 0; i < 3; i++){
            if(write(fd[1], mensagens[i], strlen(mensagens[i])) == -1){
                perror("write");
                _exit(EXIT_FAILURE);
            }
            printf("[escritor] enviada: %s\n", mensagens[i]);
            sleep(1);
        }
        close(fd[1]);
        printf("[escritor] extremidade de escrita fechada\n");
        _exit(EXIT_SUCCESS);
    }

    close(fd[1]);
    while((n = read(fd[0], buffer, sizeof(buffer) - 1 )) >0){
        buffer[n] = '\0';
        printf("[leitor] recebida: %s\n", buffer);
    }

    if(n == 0){
        printf("[leitor] read() retornou 0: EOF\n");
    } else {
        perror("read");
    }

    close(fd[0]);
    wait(NULL);

    return EXIT_SUCCESS;
}