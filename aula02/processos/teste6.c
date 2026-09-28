#include <stdio.h>
#include <unistd.h>

int main() {
    fork();

    for(int i = 0; i < 5; i++) {
        printf("Executando...\n");
        sleep(1);
    }

    return 0;
}