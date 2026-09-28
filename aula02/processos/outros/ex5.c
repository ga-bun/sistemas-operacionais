#include <stdio.h>
#include <unistd.h>

// Cada processo tem sua cópia própria da variável x. Alterações feitas no filho não afetam o pai e vice-versa.

int main() {
    int x = 10;
    pid_t pid = fork();

    if (pid == 0) {
        // Processo filho
        x++;
        printf("No processo %5d x vale %d\n", getpid(), x);
    } else {
        // Processo pai
        x += 5;
        printf("No processo %5d x vale %d\n", getpid(), x);
    }

    return 0;
}
