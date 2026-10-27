// Exercício 5 – Monitoramento de Pipes com select()
// Implemente um programa em C que crie dois pipes e três processos filhos: dois escritores
// e um receptor.
// O primeiro escritor deverá enviar cinco mensagens pelo primeiro pipe, com intervalo de 3
// segundos entre os envios. O segundo escritor deverá enviar cinco mensagens pelo segundo
// pipe, com intervalo de 8 segundos entre os envios.
// Cada mensagem deverá identificar o processo que a enviou.
// O processo receptor deverá utilizar select() para monitorar simultaneamente os dois
// pipes. Sempre que receber dados, deverá apresentar a mensagem recebida e identificar o
// escritor.
// Caso nenhuma mensagem seja recebida durante 5 segundos, o receptor deverá apresentar
// a mensagem "Aguardando mensagens..." e continuar monitorando os pipes.
// Após o término de cada escritor, o receptor deverá detectar EOF e deixar de monitorar o
// respectivo pipe. O processo pai deverá aguardar o término dos três filhos.
// Explique: como select() identifica os descritores prontos para leitura, o funcionamento
// do timeout e o tratamento de EOF.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/wait.h>

#define NUM_MENSAGENS 5
#define TIMEOUT_SEG   5

static void escritor(int fd, int id, int intervalo) {
    char msg[128];
    int i;

    for (i = 1; i <= NUM_MENSAGENS; i++) {
        sleep(intervalo);
        snprintf(msg, sizeof(msg), "Escritor %d (PID %d) - mensagem %d", id, getpid(), i);
        if (write(fd, msg, strlen(msg)) == -1) {
            perror("write");
            _exit(EXIT_FAILURE);
        }
        printf("[escritor %d] enviada: %s\n", id, msg);
    }

    close(fd);
    _exit(EXIT_SUCCESS);
}

static void receptor(int fds[2]) {
    int ativo[2] = {1, 1};
    int restantes = 2;
    char buffer[256];
    ssize_t n;
    int i, maior, pronto;
    fd_set leitura;
    struct timeval tempo;

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
            _exit(EXIT_FAILURE);
        }
        if (pronto == 0) {
            printf("[receptor] Aguardando mensagens...\n");
            continue;
        }

        for (i = 0; i < 2; i++) {
            if (!ativo[i] || !FD_ISSET(fds[i], &leitura)) {
                continue;
            }
            n = read(fds[i], buffer, sizeof(buffer) - 1);
            if (n > 0) {
                buffer[n] = '\0';
                printf("[receptor] pipe %d recebeu: %s\n", i + 1, buffer);
            } else if (n == 0) {
                printf("[receptor] pipe %d: EOF detectado, deixando de monitorar\n", i + 1);
                close(fds[i]);
                ativo[i] = 0;
                restantes--;
            } else {
                perror("read");
                _exit(EXIT_FAILURE);
            }
        }
    }

    _exit(EXIT_SUCCESS);
}

int main(void) {
    int p1[2], p2[2];
    int fds[2];
    pid_t w1, w2, rx;

    setvbuf(stdout, NULL, _IONBF, 0);

    if (pipe(p1) == -1 || pipe(p2) == -1) {
        perror("pipe");
        return EXIT_FAILURE;
    }

    w1 = fork();
    if (w1 == -1) {
        perror("fork");
        return EXIT_FAILURE;
    }
    if (w1 == 0) {
        close(p1[0]);
        close(p2[0]);
        close(p2[1]);
        escritor(p1[1], 1, 3);
    }

    w2 = fork();
    if (w2 == -1) {
        perror("fork");
        return EXIT_FAILURE;
    }
    if (w2 == 0) {
        close(p1[0]);
        close(p1[1]);
        close(p2[0]);
        escritor(p2[1], 2, 8);
    }

    rx = fork();
    if (rx == -1) {
        perror("fork");
        return EXIT_FAILURE;
    }
    if (rx == 0) {
        close(p1[1]);
        close(p2[1]);
        fds[0] = p1[0];
        fds[1] = p2[0];
        receptor(fds);
    }

    close(p1[0]);
    close(p1[1]);
    close(p2[0]);
    close(p2[1]);

    waitpid(w1, NULL, 0);
    waitpid(w2, NULL, 0);
    waitpid(rx, NULL, 0);
    printf("[pai] todos os filhos finalizados\n");

    return EXIT_SUCCESS;
}
