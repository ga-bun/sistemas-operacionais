#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifndef NUM_LEITORES
#define NUM_LEITORES 10
#endif

#define ITERACOES 5

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static int valor_compartilhado = 0;

void *leitor(void *arg)
{
    int leitor_id = *(int *)arg;

    for (int i = 0; i < ITERACOES; ++i)
    {
        pthread_mutex_lock(&mutex);
        printf("[Leitor %d] entrou na região crítica; valor atual = %d\n",
               leitor_id, valor_compartilhado);
        usleep(50000);
        printf("[Leitor %d] saiu da região crítica; valor lido = %d\n",
               leitor_id, valor_compartilhado);
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

void *escritor(void *arg)
{
    (void)arg;

    for (int i = 0; i < ITERACOES; ++i)
    {
        pthread_mutex_lock(&mutex);
        printf("[Escritor] entrou na região crítica; valor antigo = %d\n",
               valor_compartilhado);
        usleep(50000);
        valor_compartilhado++;
        printf("[Escritor] escreveu novo valor = %d\n", valor_compartilhado);
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

int main(int argc, char **argv)
{
    int leitores = NUM_LEITORES;
    if (argc > 1)
    {
        leitores = atoi(argv[1]);
    }

    pthread_t leitor_tid[NUM_LEITORES];
    pthread_t escritor_tid;
    int ids[NUM_LEITORES];

    if (leitores > NUM_LEITORES)
    {
        fprintf(stderr, "Número de leitores excede o limite configurado (%d).\n",
                NUM_LEITORES);
        return EXIT_FAILURE;
    }

    for (int i = 0; i < leitores; ++i)
    {
        ids[i] = i;
        if (pthread_create(&leitor_tid[i], NULL, leitor, &ids[i]) != 0)
        {
            perror("pthread_create leitor");
            return EXIT_FAILURE;
        }
    }

    if (pthread_create(&escritor_tid, NULL, escritor, NULL) != 0)
    {
        perror("pthread_create escritor");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < leitores; ++i)
    {
        pthread_join(leitor_tid[i], NULL);
    }

    pthread_join(escritor_tid, NULL);
    pthread_mutex_destroy(&mutex);

    printf("Execução concluída. Valor final compartilhado: %d\n", valor_compartilhado);
    return EXIT_SUCCESS;
}
