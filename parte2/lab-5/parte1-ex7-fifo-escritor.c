// Exercício 7 – Duas FIFOs e select() (atividade adicional)
// Crie duas FIFOs. Dois escritores independentes deverão produzir dados em intervalos
// diferentes; um processo leitor deverá acompanhar ambas usando select() com timeout.
// Explique: como identificar qual descritor está pronto, o significado de prontidão quando
// ocorre EOF e os cuidados necessários com a abertura e o fechamento das FIFOs.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    char msg[128];
    int intervalo;
    int quantidade;
    int fd;
    int i;

    if (argc != 5) {
        fprintf(stderr, "uso: %s <fifo> <intervalo_seg> <quantidade> <nome>\n", argv[0]);
        return EXIT_FAILURE;
    }

    setvbuf(stdout, NULL, _IONBF, 0);

    intervalo = atoi(argv[2]);
    quantidade = atoi(argv[3]);

    fd = open(argv[1], O_WRONLY);
    if (fd == -1) {
        perror("open");
        return EXIT_FAILURE;
    }

    for (i = 1; i <= quantidade; i++) {
        sleep(intervalo);
        snprintf(msg, sizeof(msg), "%s (PID %d) mensagem %d", argv[4], getpid(), i);
        if (write(fd, msg, strlen(msg)) == -1) {
            perror("write");
            break;
        }
        printf("[%s] enviada: %s\n", argv[4], msg);
    }

    close(fd);
    printf("[%s] FIFO fechada\n", argv[4]);

    return EXIT_SUCCESS;
}
