#include <stdio.h>
#include <unistd.h>

int main() {
    fork();
    printf("Oi!\n");
    return 0;
}