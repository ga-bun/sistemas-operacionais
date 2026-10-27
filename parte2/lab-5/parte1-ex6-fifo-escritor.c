// Exercício 6 – FIFO Nomeada (atividade adicional)
// Crie uma FIFO usando mkfifo(). Implemente dois programas independentes: um escritor e um leitor, que se comuniquem por meio do nome da FIFO no sistema de arquivos.
// Observe e explique: o comportamento de open() quando não existe outra extremidade
// aberta, a diferença entre a entrada da FIFO no sistema de arquivos e os dados armazenados
// no núcleo, e o efeito de unlink().

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define NOME_FIFO "/tmp/fifo_lab_ex6"

int main(void) {
    char msg[64];
    int fd;
    int i;

    setvbuf(stdout, NULL, _IONBF, 0);

    if (mkfifo(NOME_FIFO, 0666) == -1 && errno != EEXIST) {
        perror("mkfifo");
        return EXIT_FAILURE;
    }
    printf("[escritor] FIFO criada ou existente: %s\n", NOME_FIFO);

    printf("[escritor] open() para escrita, bloqueia ate existir leitor\n");
    fd = open(NOME_FIFO, O_WRONLY);
    if (fd == -1) {
        perror("open");
        return EXIT_FAILURE;
    }
    printf("[escritor] FIFO aberta para escrita\n");

    for (i = 1; i <= 5; i++) {
        snprintf(msg, sizeof(msg), "mensagem %d", i);
        if (write(fd, msg, strlen(msg)) == -1) {
            perror("write");
            break;
        }
        printf("[escritor] enviada: %s\n", msg);
        sleep(1);
    }

    close(fd);
    printf("[escritor] FIFO fechada\n");

    return EXIT_SUCCESS;
}
