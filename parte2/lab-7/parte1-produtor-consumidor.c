#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define PRODUCERS 3
#define CONSUMERS 3
#define BUFFER_CAPACITY 5
#define ITEMS_PER_PRODUCER 20
#define TOTAL_ITEMS (PRODUCERS * ITEMS_PER_PRODUCER)

typedef struct {
    int producer_id;
    int seq;
    int value;
} Item;

static Item buffer[BUFFER_CAPACITY];
static int head = 0;
static int tail = 0;
static int ocupadas = 0;

static sem_t vagas;
static sem_t itens;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *produtor(void *arg)
{
    int producer_id = *(int *)arg;

    for (int seq = 0; seq < ITEMS_PER_PRODUCER; ++seq)
    {
        int value = producer_id * 100 + seq;

        sem_wait(&vagas);
        pthread_mutex_lock(&mutex);

        buffer[tail] = (Item){.producer_id = producer_id, .seq = seq, .value = value};
        tail = (tail + 1) % BUFFER_CAPACITY;
        ocupadas++;

        printf("[Produtor %d] inseriu item %d (seq=%d) | ocupadas=%d\n",
               producer_id, value, seq, ocupadas);

        pthread_mutex_unlock(&mutex);
        sem_post(&itens);
    }

    return NULL;
}

void *consumidor(void *arg)
{
    int consumer_id = *(int *)arg;

    for (int i = 0; i < ITEMS_PER_PRODUCER; ++i)
    {
        sem_wait(&itens);
        pthread_mutex_lock(&mutex);

        Item item = buffer[head];
        head = (head + 1) % BUFFER_CAPACITY;
        ocupadas--;

        printf("[Consumidor %d] consumiu produtor=%d item=%d | ocupadas=%d\n",
               consumer_id, item.producer_id, item.value, ocupadas);

        pthread_mutex_unlock(&mutex);
        sem_post(&vagas);
    }

    return NULL;
}

int main(void)
{
    pthread_t p[PRODUCERS];
    pthread_t c[CONSUMERS];
    int producer_ids[PRODUCERS];
    int consumer_ids[CONSUMERS];

    sem_init(&vagas, 0, BUFFER_CAPACITY);
    sem_init(&itens, 0, 0);

    for (int i = 0; i < PRODUCERS; ++i)
    {
        producer_ids[i] = i;
        if (pthread_create(&p[i], NULL, produtor, &producer_ids[i]) != 0)
        {
            perror("pthread_create produtor");
            return EXIT_FAILURE;
        }
    }

    for (int i = 0; i < CONSUMERS; ++i)
    {
        consumer_ids[i] = i;
        if (pthread_create(&c[i], NULL, consumidor, &consumer_ids[i]) != 0)
        {
            perror("pthread_create consumidor");
            return EXIT_FAILURE;
        }
    }

    for (int i = 0; i < PRODUCERS; ++i)
    {
        pthread_join(p[i], NULL);
    }

    for (int i = 0; i < CONSUMERS; ++i)
    {
        pthread_join(c[i], NULL);
    }

    sem_destroy(&vagas);
    sem_destroy(&itens);
    pthread_mutex_destroy(&mutex);

    printf("Total de itens produzidos: %d\n", TOTAL_ITEMS);
    printf("Todos os itens foram consumidos com sucesso.\n");

    return EXIT_SUCCESS;
}
