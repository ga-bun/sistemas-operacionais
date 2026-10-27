#define _POSIX_C_SOURCE 200809L

// Exercício 3 – Concorrência com Múltiplos Processos
// Implemente duas situações distintas.
// Parte A – Múltiplos transmissores e um receptor Implemente um programa com:
// • dois processos transmissores;
// • um processo receptor.
// Cada transmissor deve enviar mensagens identificadas com sua origem, por exemplo:
// 3 "TX1 - leitura 10"
// 4 "TX2 - leitura 15"
// Observe:
// • a ordem em que as mensagens chegam;
// • possíveis variações entre execuções;
// • se cada mensagem é recebida inteira ou pode ser misturada com outra.
// IFSC – CAMPUS SÃO JOSÉ 4
// Parte B – Um transmissor e múltiplos receptores Implemente um programa com:
// • um processo transmissor;
// • dois processos receptores.
// O transmissor deve enviar várias mensagens numeradas.
// Observe:
// • como as mensagens são distribuídas entre os receptores;
// • se uma mesma mensagem pode ser entregue duas vezes;
// • se os receptores competem entre si pelas mensagens disponíveis.
// Explique:
// • por que as mensagens são preservadas como unidades discretas;
// • por que não ocorre difusão automática das mensagens para todos os receptores;
// • a diferença entre concorrência entre transmissores e concorrência entre receptores.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <mqueue.h>

#define NOME_FILA "/fila_lab_ex3"
#define TAM_MSG   64
#define MAX_MSG   10
#define NUM_MSG_A 5
#define NUM_MSG_B 10

static void verificarFork(pid_t pid) {
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
}

static void transmissorA(const char *nome) {
    mqd_t fila;
    char msg[TAM_MSG];
    int i;

    fila = mq_open(NOME_FILA, O_WRONLY);
    if (fila == (mqd_t)-1) {
        perror("mq_open transmissor");
        _exit(EXIT_FAILURE);
    }

    for (i = 1; i <= NUM_MSG_A; i++) {
        snprintf(msg, sizeof(msg), "%s - leitura %d", nome, i * 5);
        if (mq_send(fila, msg, strlen(msg), 0) == -1) {
            perror("mq_send");
            _exit(EXIT_FAILURE);
        }
        printf("[%s] enviou: %s\n", nome, msg);
        usleep(100000);
    }

    mq_close(fila);
    _exit(EXIT_SUCCESS);
}

static void receptorA(mqd_t fila, int total) {
    char buffer[TAM_MSG];
    ssize_t n;
    int i;

    for (i = 1; i <= total; i++) {
        n = mq_receive(fila, buffer, TAM_MSG, NULL);
        if (n == -1) {
            perror("mq_receive");
            return;
        }
        buffer[n] = '\0';
        printf("[receptor] %d recebida inteira: %s\n", i, buffer);
    }
}

static void transmissorB(void) {
    mqd_t fila;
    char msg[TAM_MSG];
    int i;

    fila = mq_open(NOME_FILA, O_WRONLY);
    if (fila == (mqd_t)-1) {
        perror("mq_open transmissor");
        _exit(EXIT_FAILURE);
    }

    for (i = 1; i <= NUM_MSG_B; i++) {
        snprintf(msg, sizeof(msg), "Mensagem %d", i);
        if (mq_send(fila, msg, strlen(msg), 0) == -1) {
            perror("mq_send");
            _exit(EXIT_FAILURE);
        }
        printf("[transmissor] enviou: %s\n", msg);
        usleep(100000);
    }

    mq_close(fila);
    _exit(EXIT_SUCCESS);
}

static void receptorB(const char *nome) {
    mqd_t fila;
    char buffer[TAM_MSG];
    struct timespec limite;
    ssize_t n;
    int contador = 0;

    fila = mq_open(NOME_FILA, O_RDONLY);
    if (fila == (mqd_t)-1) {
        perror("mq_open receptor");
        _exit(EXIT_FAILURE);
    }

    for (;;) {
        clock_gettime(CLOCK_REALTIME, &limite);
        limite.tv_sec += 1;

        n = mq_timedreceive(fila, buffer, TAM_MSG, NULL, &limite);
        if (n == -1) {
            if (errno != ETIMEDOUT) {
                perror("mq_timedreceive");
            }
            break;
        }
        buffer[n] = '\0';
        contador++;
        printf("[%s] recebeu: %s\n", nome, buffer);
    }

    printf("[%s] sem mensagens por 1 segundo, total recebido = %d\n", nome, contador);
    mq_close(fila);
    _exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
    struct mq_attr attr;
    mqd_t fila;
    pid_t pid;

    setvbuf(stdout, NULL, _IONBF, 0);

    if (argc != 2 || (strcmp(argv[1], "A") != 0 && strcmp(argv[1], "B") != 0)) {
        fprintf(stderr, "uso: %s A|B\n", argv[0]);
        return EXIT_FAILURE;
    }

    memset(&attr, 0, sizeof(attr));
    attr.mq_maxmsg = MAX_MSG;
    attr.mq_msgsize = TAM_MSG;

    mq_unlink(NOME_FILA);
    fila = mq_open(NOME_FILA, O_CREAT | O_RDONLY, 0600, &attr);
    if (fila == (mqd_t)-1) {
        perror("mq_open");
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "A") == 0) {
        pid = fork();
        verificarFork(pid);
        if (pid == 0) {
            transmissorA("TX1");
        }
        pid = fork();
        verificarFork(pid);
        if (pid == 0) {
            transmissorA("TX2");
        }

        receptorA(fila, 2 * NUM_MSG_A);

        wait(NULL);
        wait(NULL);
    } else {
        pid = fork();
        verificarFork(pid);
        if (pid == 0) {
            transmissorB();
        }
        pid = fork();
        verificarFork(pid);
        if (pid == 0) {
            receptorB("R1");
        }
        pid = fork();
        verificarFork(pid);
        if (pid == 0) {
            receptorB("R2");
        }

        wait(NULL);
        wait(NULL);
        wait(NULL);
    }

    mq_close(fila);
    mq_unlink(NOME_FILA);

    return EXIT_SUCCESS;
}