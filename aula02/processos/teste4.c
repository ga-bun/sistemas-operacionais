#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int pid = fork();

    if (pid == 0) {
        // filho
        printf("Filho: executando...\n");
        sleep(2);
        printf("Filho: terminou\n");
    } else {
        // pai
        printf("Pai: esperando o filho\n");
        wait(NULL);
        printf("Pai: filho terminou\n");
    }

    return 0;
}