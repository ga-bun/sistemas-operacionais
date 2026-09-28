#include <stdio.h>
#include <unistd.h>

int main() {
    int x = 10;

    if (fork() == 0) {
        x = 20;
        printf("Filho: %d\n", x);
    } else {
        printf("Pai: %d\n", x);
    }

    return 0;
}