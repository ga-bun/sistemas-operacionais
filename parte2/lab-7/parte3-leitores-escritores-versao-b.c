#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define NUM_LEITORES 10
#define NUM_LEITURAS 20
#define NUM_ESCRITAS 2

static pthread_mutex_t mutex_leitores = PTHREAD_MUTEX_INITIALIZER;
static sem_t acesso;
static int leitores_ativos = 0;
static int valor_compartilhado = 0;

static double diff_ms(const struct timespec *start, const struct timespec *end)
{
    long seconds = end->tv_sec - start->tv_sec;
    long nanoseconds = end->tv_nsec - start->tv_nsec;
    return (seconds * 1000.0) + (nanoseconds / 1000000.0);
}

void *leitor(void *arg)
{
    int reader_id = *(int *)arg;

    for (int i = 0; i < NUM_LEITURAS; ++i)
    {
        pthread_mutex_lock(&mutex_leitores);
        leitores_ativos++;
        if (leitores_ativos == 1)
        {
            sem_wait(&acesso);
        }
        pthread_mutex_unlock(&mutex_leitores);

        printf("[Leitor %d] entrou, leitores ativos=%d, valor=%d\n",
               reader_id, leitores_ativos, valor_compartilhado);
        usleep(120000);
        printf("[Leitor %d] leu valor=%d\n", reader_id, valor_compartilhado);

        pthread_mutex_lock(&mutex_leitores);
        leitores_ativos--;
        if (leitores_ativos == 0)
        {
            sem_post(&acesso);
        }
        pthread_mutex_unlock(&mutex_leitores);

        usleep(30000);
    }

    return NULL;
}

void *escritor(void *arg)
{
    (void)arg;

    struct timespec inicio, fim;

    for (int i = 0; i < NUM_ESCRITAS; ++i)
    {
        clock_gettime(CLOCK_MONOTONIC, &inicio);
        printf("[Escritor] solicitou acesso em %ld.%09ld s\n",
               (long)inicio.tv_sec, inicio.tv_nsec);

        sem_wait(&acesso);
        clock_gettime(CLOCK_MONOTONIC, &fim);

        printf("[Escritor] entrou após %.3f ms; valor anterior = %d\n",
               diff_ms(&inicio, &fim), valor_compartilhado);
        usleep(50000);
        valor_compartilhado += 1;
        printf("[Escritor] escreveu novo valor = %d\n", valor_compartilhado);

        sem_post(&acesso);
    }

    return NULL;
}

int main(void)
{
    pthread_t leitores_tid[NUM_LEITORES];
    pthread_t escritor_tid;
    int ids[NUM_LEITORES];

    sem_init(&acesso, 0, 1);

    for (int i = 0; i < NUM_LEITORES; ++i)
    {
        ids[i] = i;
        if (pthread_create(&leitores_tid[i], NULL, leitor, &ids[i]) != 0)
        {
            perror("pthread_create leitor");
            return EXIT_FAILURE;
        }
    }

    usleep(200000);

    if (pthread_create(&escritor_tid, NULL, escritor, NULL) != 0)
    {
        perror("pthread_create escritor");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < NUM_LEITORES; ++i)
    {
        pthread_join(leitores_tid[i], NULL);
    }

    pthread_join(escritor_tid, NULL);

    sem_destroy(&acesso);
    pthread_mutex_destroy(&mutex_leitores);

    printf("Versão B concluída. Valor final: %d\n", valor_compartilhado);
    return EXIT_SUCCESS;
}
