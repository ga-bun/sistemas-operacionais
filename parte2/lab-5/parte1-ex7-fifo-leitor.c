// Exercício 7 – Duas FIFOs e select() (atividade adicional)
// Crie duas FIFOs. Dois escritores independentes deverão produzir dados em intervalos
// diferentes; um processo leitor deverá acompanhar ambas usando select() com timeout.
// Explique: como identificar qual descritor está pronto, o significado de prontidão quando
// ocorre EOF e os cuidados necessários com a abertura e o fechamento das FIFOs.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/stat.h>

#define FIFO_A      "/tmp/fifo_lab_ex7_a"
#define FIFO_B      "/tmp/fifo_lab_ex7_b"
#define TIMEOUT_SEG 5

int main(void) {
    const char *caminhos[2] = {FIFO_A, FIFO_B};
    int fds[2];
    int ativo[2] = {1, 1};
    int restantes = 2;
    int i, maior, pronto;
    char buffer[128];
    ssize_t n;
    fd_set leitura;
    struct timeval tempo;

    setvbuf(stdout, NULL, _IONBF, 0);

    for (i = 0; i < 2; i++) {
        if (mkfifo(caminhos[i], 0666) == -1 && errno != EEXIST) {
            perror("mkfifo");
            return EXIT_FAILURE;
        }
        fds[i] = open(caminhos[i], O_RDONLY | O_NONBLOCK);
        if (fds[i] == -1) {
            perror("open");
            return EXIT_FAILURE;
        }
    }
    printf("[leitor] FIFOs abertas em modo nao bloqueante\n");

    while (restantes > 0) {
        FD_ZERO(&leitura);
        maior = -1;
        for (i = 0; i < 2; i++) {
            if (ativo[i]) {
                FD_SET(fds[i], &leitura);
                if (fds[i] > maior) {
                    maior = fds[i];
                }
            }
        }

        tempo.tv_sec = TIMEOUT_SEG;
        tempo.tv_usec = 0;

        pronto = select(maior + 1, &leitura, NULL, NULL, &tempo);
        if (pronto == -1) {
            perror("select");
            break;
        }
        if (pronto == 0) {
            printf("[leitor] aguardando dados...\n");
            continue;
        }

        for (i = 0; i < 2; i++) {
            if (!ativo[i] || !FD_ISSET(fds[i], &leitura)) {
                continue;
            }
            n = read(fds[i], buffer, sizeof(buffer) - 1);
            if (n > 0) {
                buffer[n] = '\0';
                printf("[leitor] %s: %s\n", caminhos[i], buffer);
            } else if (n == 0) {
                printf("[leitor] %s: EOF, escritor fechou a FIFO\n", caminhos[i]);
                close(fds[i]);
                ativo[i] = 0;
                restantes--;
            } else if (errno != EAGAIN) {
                perror("read");
                close(fds[i]);
                ativo[i] = 0;
                restantes--;
            }
        }
    }

    for (i = 0; i < 2; i++) {
        if (ativo[i]) {
            close(fds[i]);
        }
        unlink(caminhos[i]);
    }
    printf("[leitor] FIFOs removidas\n");

    return EXIT_SUCCESS;
}
