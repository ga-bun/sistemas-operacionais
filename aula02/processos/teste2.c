#include <stdio.h>
#include <unistd.h>
int main() {
    int pid = fork();

    if (pid == -1) {
        // erro ao criar processo
        printf("Erro no fork\n");
    }
    else if (pid == 0) {
        // código executado pelo filho
        printf("Sou o FILHO\n");
        printf("PID do filho: %d\n", getpid());
    }
    else {
        // código executado pelo pai
        printf("Sou o PAI\n");
        printf("PID do filho: %d\n", pid);
        printf("PID do pai: %d\n", getpid());
    }
    printf("Ambos continuam daqui!\n");
    return 0;
}