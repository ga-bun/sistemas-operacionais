// Exercício 2 – Leitura Bloqueante

// Implemente um programa em C que crie um pipe e um processo filho.
// O processo filho deverá executar read() para receber uma mensagem do pai. 
// O processo pai deverá aguardar 5 segundos e, em seguida, escrever a mensagem "IFSC São José" no pipe
// O filho deverá apresentar a mensagem recebida na saída padrão.
// Observe e explique: por que read() bloqueia e em que momento a operação é desbloqueada

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define MENSAGEM "IFSC São José"

int main() {
    int fd[2];
    pid_t pid;
    ssize_t n; // tipo inteiro com sinal -> usado para guardar as funções read()/write()
    char buffer[128];

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
        printf("[filho] chamando read() (bloquenate)\n");
        n = read(fd[0], buffer, sizeof(buffer) -1);
        if(n == -1){
            perror ("read");
            _exit(EXIT_FAILURE);
        }
        buffer[n] = '\0';
        printf("[filho] mensagem recebida: %s\n", buffer);
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