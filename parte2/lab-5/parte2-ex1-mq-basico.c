#define _POSIX_C_SOURCE 200809L

// Exercício 1 – Uso Básico da Fila: Bloqueante, Não Bloqueante e Timeout na
// Recepção
// Implemente dois processos:
// • um processo transmissor;
// • um processo receptor.
// O transmissor deve enviar uma mensagem textual simples, como por exemplo:
// 2 "Temperatura=29.5"
// O receptor deve abrir a mesma fila e receber essa mensagem, exibindo-a na tela.
// Este exercício deve ser dividido em três partes:
// 1. Recepção bloqueante: o receptor chama mq_receive() antes do transmissor enviar
// a mensagem.
// 2. Recepção não bloqueante: o receptor abre a fila em modo não bloqueante e tenta
// receber uma mensagem quando a fila está vazia.
// 3. Recepção com timeout: o receptor utiliza mq_timedreceive() com um tempo limite,
// enquanto o transmissor envia a mensagem somente após esse tempo expirar.
// Explique:
// • o papel de mq_open();
// • a diferença entre abrir a fila para escrita e para leitura;
// • por que a recepção pode bloquear;
// • o que muda no modo não bloqueante;
// • o significado de um retorno por expiração de tempo;
// • a diferença entre:
// – bloqueio indefinido;
// – retorno imediato;
// – espera com tempo limite

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <mqueue.h>

#define NOME_FILA "/fila_lab_ex1"
#define TAM_MSG   64
#define MAX_MSG   5
#define MENSAGEM  "Temperatura=29.5"

static void transmissor(void) {
    mqd_t fila;

    fila = mq_open(NOME_FILA, O_WRONLY);
    if (fila == (mqd_t)-1) {
        perror("mq_open transmissor");
        _exit(EXIT_FAILURE);
    }

    printf("[transmissor] aguardando 3 segundos antes de enviar\n");
    sleep(3);

    if (mq_send(fila, MENSAGEM, strlen(MENSAGEM), 0) == -1) {
        perror("mq_send");
        _exit(EXIT_FAILURE);
    }
    printf("[transmissor] mensagem enviada: %s\n", MENSAGEM);

    mq_close(fila);
    _exit(EXIT_SUCCESS);
}

static void exibirRecebida(char *buffer, ssize_t n) {
    buffer[n] = '\0';
    printf("[receptor] mensagem recebida: %s\n", buffer);
}

int main(int argc, char *argv[]) {
    const char *modo;
    struct mq_attr attr;
    struct mq_attr nb;
    struct timespec limite;
    mqd_t fila;
    pid_t pid;
    char buffer[TAM_MSG];
    ssize_t n;

    setvbuf(stdout, NULL, _IONBF, 0);

    if (argc != 2) {
        fprintf(stderr, "uso: %s bloqueante|nao-bloqueante|timeout\n", argv[0]);
        return EXIT_FAILURE;
    }
    modo = argv[1];

    memset(&attr, 0, sizeof(attr));
    attr.mq_maxmsg = MAX_MSG;
    attr.mq_msgsize = TAM_MSG;

    mq_unlink(NOME_FILA);
    fila = mq_open(NOME_FILA, O_CREAT | O_RDONLY, 0600, &attr);
    if (fila == (mqd_t)-1) {
        perror("mq_open receptor");
        return EXIT_FAILURE;
    }

    pid = fork();
    if (pid == -1) {
        perror("fork");
        return EXIT_FAILURE;
    }
    if (pid == 0) {
        transmissor();
    }

    if (strcmp(modo, "bloqueante") == 0) {
        printf("[receptor] chamando mq_receive (bloqueante)\n");
        n = mq_receive(fila, buffer, TAM_MSG, NULL);
        if (n == -1) {
            perror("mq_receive");
        } else {
            exibirRecebida(buffer, n);
        }
    } else if (strcmp(modo, "nao-bloqueante") == 0) {
        memset(&nb, 0, sizeof(nb));
        nb.mq_flags = O_NONBLOCK;
        if (mq_setattr(fila, &nb, NULL) == -1) {
            perror("mq_setattr");
            return EXIT_FAILURE;
        }
        printf("[receptor] fila em modo nao bloqueante\n");

        while ((n = mq_receive(fila, buffer, TAM_MSG, NULL)) == -1 && errno == EAGAIN) {
            printf("[receptor] fila vazia (EAGAIN), nova tentativa em 1 segundo\n");
            sleep(1);
        }
        if (n == -1) {
            perror("mq_receive");
        } else {
            exibirRecebida(buffer, n);
        }
    } else if (strcmp(modo, "timeout") == 0) {
        clock_gettime(CLOCK_REALTIME, &limite);
        limite.tv_sec += 2;
        printf("[receptor] chamando mq_timedreceive com limite de 2 segundos\n");

        n = mq_timedreceive(fila, buffer, TAM_MSG, NULL, &limite);
        if (n == -1 && errno == ETIMEDOUT) {
            printf("[receptor] tempo limite expirou sem mensagem\n");
            n = mq_receive(fila, buffer, TAM_MSG, NULL);
        }
        if (n == -1) {
            perror("mq_receive");
        } else {
            exibirRecebida(buffer, n);
        }
    } else {
        fprintf(stderr, "modo invalido: %s\n", modo);
        mq_close(fila);
        mq_unlink(NOME_FILA);
        return EXIT_FAILURE;
    }

    waitpid(pid, NULL, 0);
    mq_close(fila);
    mq_unlink(NOME_FILA);

    return EXIT_SUCCESS;
}
