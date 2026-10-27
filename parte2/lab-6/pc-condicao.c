#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 5
#define TOTAL 20

static int buffer[N];

static int entrada = 0;
static int saida = 0;

static int ocupadas = 0;
static int total_consumido = 0;

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t nao_cheio = PTHREAD_COND_INITIALIZER;
static pthread_cond_t nao_vazio = PTHREAD_COND_INITIALIZER;

void *produtor(void *arg)
{
    for (int i = 0; i < TOTAL; i++)
    {
        pthread_mutex_lock(&mutex);

        while (ocupadas == N)
        {
            pthread_cond_wait(&nao_cheio, &mutex);
        }

        buffer[entrada] = i;
        entrada = (entrada + 1) % N; 
        ocupadas++;                  

        printf("Produziu %d | ocupadas: %d\n", i, ocupadas);

        pthread_cond_signal(&nao_vazio);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void *consumidor(void *arg)
{
    while (total_consumido < TOTAL)
    {
        pthread_mutex_lock(&mutex);

        while (ocupadas == 0)
        {
            pthread_cond_wait(&nao_vazio, &mutex);
        }

        int itemBuffer = buffer[saida];
        saida = (saida + 1) % N;
        ocupadas--;
        total_consumido++;

        printf("Consumiu %d | ocupadas: %d\n", itemBuffer, ocupadas);

        pthread_cond_signal(&nao_cheio);
        pthread_mutex_unlock(&mutex);

    }

    return NULL;
}

int main()
{
    pthread_t produtora, consumidora;

    if (pthread_create(&produtora, NULL, produtor, NULL) != 0 || pthread_create(&consumidora, NULL, consumidor, NULL) != 0)
    {
        perror("pthread_create");
        return EXIT_FAILURE;
    }

    pthread_join(produtora, NULL);
    pthread_join(consumidora, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&nao_cheio);
    pthread_cond_destroy(&nao_vazio);

    // printf("Produzidos: %d | Consumidos: %d", TOTAL, total_consumido);

    return 0;
}