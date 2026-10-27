// Exercício 3 – Leitura Não Bloqueante

// Modifique o programa do Exercício 2 para que o processo filho realize a leitura do pipe em
// modo não bloqueante, utilizando fcntl() e a opção O_NONBLOCK.
// O processo pai deverá aguardar 5 segundos antes de escrever a mensagem "IFSC São José" no pipe.
// O filho deverá tentar ler o pipe periodicamente, com intervalo de 1 segundo entre as tentativas, até receber a mensagem.
// O programa deverá apresentar uma indicação sempre que uma tentativa de leitura não
// encontrar dados disponíveis.
// Explique: como distinguir a ausência temporária de dados, o fim do pipe (EOF) e um erro
// de leitura.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

#define MENSAGEM "IFSC São José"

int main() {
    int fd[2];
    pid_t pid;
    ssize_t n;
    char buffer[128];
    int flags;
    int tentativa = 0;

    setvbuf(stdout, NULL, _IONBF, 0);

    if(pipe(fd) == -1){
        perror("pipe");
        return EXIT_FAILURE;
    }
    
    pid = fork();
    if(pid == -1){
        perror("fork");
        return EXIT_FAILURE;
    }

    if (pid == 0) {
        close(fd[1]);

        flags =fcntl(fd[0], F_GETFL);
        if(flags == -1 || fcntl(fd[0], F_SETFL, flags | O_NONBLOCK) == -1){
            perror("fcntl");
            _exit(EXIT_FAILURE);
        }

        for(;;) {
            tentativa++;
            n = read(fd[0], buffer, sizeof(buffer) -1);
            if(n > 0) {
                buffer[n] = '\0';
                printf("[filho] tentativa %d: mensagem recebida: %s\n", tentativa, buffer);
                break;
            }
            if(n == 0){
                printf("[filho] tentativa %d: EOF, nenhum escritor ativo\n", tentativa);
                break;
            }
            if(errno == EAGAIN){
                printf("[filho] tentativa %d:sem dados disponíveis, nova tentativa em  segundo\n", tentativa);
                sleep(1);
                continue;
            }
            perror("read");
            break;
        }
        close(fd[0]);
        _exit(EXIT_SUCCESS);
    }

    close(fd[0]);
    printf("[pai] aguardando 5 segundos\n");
    sleep(5);
    if(write(fd[1], MENSAGEM, strlen(MENSAGEM)) == -1){
        perror("write");
    }
    printf("[pai] mensagem escrita no pipe\n");
    close(fd[1]);
    wait(NULL);

    return EXIT_SUCCESS;
}