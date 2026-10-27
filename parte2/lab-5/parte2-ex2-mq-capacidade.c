#define _POSIX_C_SOURCE 200809L
// Exercício 2 – Capacidade da Fila, Tamanho da Mensagem e Timeout no Envio
// Crie uma fila com capacidade pequena, por exemplo 3 mensagens, utilizando struct
// mq_attr.
// Defina também explicitamente o tamanho máximo de cada mensagem.
// Este exercício deve explorar:
// a) Capacidade máxima da fila: um transmissor deve enviar mensagens continuamente,
// enquanto o receptor demora para consumi-las.
// b) Envio bloqueante: observe o momento em que mq_send() bloqueia porque a fila está
// cheia.
// c) Envio com timeout: substitua o envio por mq_timedsend() e observe o que ocorre
// quando não há espaço disponível antes do tempo limite.
// d) Tamanho da mensagem: tente enviar uma mensagem maior que o tamanho máximo
// configurado para a fila.
// Explique:
// • o papel de mq_maxmsg;
// • o papel de mq_msgsize;
// • o que acontece quando a fila enche;
// • o que causa o desbloqueio do transmissor;
// • o que muda entre:
// – envio bloqueante;
// – envio com tempo limite;
// – falha por mensagem grande demais.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <mqueue.h>

#define NOME_FILA "/fila_lab_ex2"
#define TAM_MSG   32
#define MAX_MSG   3
#define NUM_MSG   10

static void transmissor(const char *modo) {
    mqd_t fila;
    char msg[TAM_MSG];
    char grande[100];
    struct timespec limite;
    int i;

    fila = mq_open(NOME_FILA, O_WRONLY);
    if (fila == (mqd_t)-1) {
        perror("mq_open transmissor");
        _exit(EXIT_FAILURE);
    }

    if (strcmp(modo, "capacidade") == 0) {
        for (i = 1; i <= NUM_MSG; i++) {
            snprintf(msg, sizeof(msg), "msg %d", i);
            printf("[transmissor] enviando %s\n", msg);
            if (mq_send(fila, msg, strlen(msg), 0) == -1) {
                perror("mq_send");
                _exit(EXIT_FAILURE);
            }
            printf("[transmissor] %s enviada\n", msg);
        }
    } else if (strcmp(modo, "timeout") == 0) {
        for (i = 1; i <= NUM_MSG; i++) {
            snprintf(msg, sizeof(msg), "msg %d", i);
            clock_gettime(CLOCK_REALTIME, &limite);
            limite.tv_sec += 1;
            printf("[transmissor] enviando %s com limite de 1 segundo\n", msg);
            if (mq_timedsend(fila, msg, strlen(msg), 0, &limite) == -1) {
                if (errno == ETIMEDOUT) {
                    printf("[transmissor] %s: tempo limite expirou, fila cheia\n", msg);
                } else {
                    perror("mq_timedsend");
                    _exit(EXIT_FAILURE);
                }
            } else {
                printf("[transmissor] %s enviada\n", msg);
            }
        }
    } else {
        memset(grande, 'A', sizeof(grande) - 1);
        grande[sizeof(grande) - 1] = '\0';
        printf("[transmissor] enviando mensagem de %zu bytes (mq_msgsize = %d)\n",
               strlen(grande), TAM_MSG);
        if (mq_send(fila, grande, strlen(grande), 0) == -1) {
            perror("mq_send (mensagem maior que mq_msgsize)");
        }
        snprintf(msg, sizeof(msg), "msg pequena");
        if (mq_send(fila, msg, strlen(msg), 0) == -1) {
            perror("mq_send");
        } else {
            printf("[transmissor] %s enviada\n", msg);
        }
    }

    mq_close(fila);
    _exit(EXIT_SUCCESS);
}

static void receptor(mqd_t fila, pid_t pid) {
    struct mq_attr attr;
    char buffer[TAM_MSG];
    ssize_t n;
    int status;

    for (;;) {
        sleep(2);

        if (mq_getattr(fila, &attr) == -1) {
            perror("mq_getattr");
            return;
        }
        printf("[receptor] mensagens na fila: %ld de %ld\n", attr.mq_curmsgs, attr.mq_maxmsg);

        if (attr.mq_curmsgs > 0) {
            n = mq_receive(fila, buffer, TAM_MSG, NULL);
            if (n == -1) {
                perror("mq_receive");
                return;
            }
            buffer[n] = '\0';
            printf("[receptor] recebida: %s\n", buffer);
        } else if (waitpid(pid, &status, WNOHANG) == pid) {
            return;
        }
    }
}

int main(int argc, char *argv[]) {
    struct mq_attr attr;
    mqd_t fila;
    pid_t pid;

    setvbuf(stdout, NULL, _IONBF, 0);

    if (argc != 2 ||
        (strcmp(argv[1], "capacidade") != 0 &&
         strcmp(argv[1], "timeout") != 0 &&
         strcmp(argv[1], "tamanho") != 0)) {
        fprintf(stderr, "uso: %s capacidade|timeout|tamanho\n", argv[0]);
        return EXIT_FAILURE;
    }

    memset(&attr, 0, sizeof(attr));
    attr.mq_maxmsg = MAX_MSG;
    attr.mq_msgsize = TAM_MSG;

    mq_unlink(NOME_FILA);
    fila = mq_open(NOME_FILA, O_CREAT | O_RDONLY, 0600, &attr);
    if (fila == (mqd_t)-1) {
        perror("mq_open receptor");
        return EXIT_FAILURE;
    }

    if (mq_getattr(fila, &attr) == 0) {
        printf("[pai] mq_maxmsg = %ld, mq_msgsize = %ld\n", attr.mq_maxmsg, attr.mq_msgsize);
    }

    pid = fork();
    if (pid == -1) {
        perror("fork");
        return EXIT_FAILURE;
    }
    if (pid == 0) {
        transmissor(argv[1]);
    }

    receptor(fila, pid);

    mq_close(fila);
    mq_unlink(NOME_FILA);

    return EXIT_SUCCESS;
}
