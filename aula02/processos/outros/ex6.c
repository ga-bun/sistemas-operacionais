#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

/** 
 * wait(0) é equivalente a wait(NULL) em muitos sistemas, mas quando usamos wait(&status) (ou wait(0) em algumas implementações), 
 * o pai captura o código de saída do filho.
 * 
 *  Se o filho terminar com exit(0), o status será 0.
 *  Se terminar com outro valor, o pai pode verificar.
 * 
 *  */ 

int main() {
    int x = 30;
    pid_t pid = fork();

    if (pid == 0) {
        x++;
        printf("Filho (PID: %d) x = %d\n", getpid(), x);
    } else {
        int status;
        wait(&status); // equivalente a wait(0)
        printf("Pai (PID: %d) recebeu status %d\n", getpid(), status);
        x += 5;
        printf("Pai (PID: %d) x = %d\n", getpid(), x);
    }

    return 0;
}

/** 
 * 
wait(NULL) → apenas espera o filho terminar, ignora o valor de saída.
wait(0) ou wait(&status) → espera o filho terminar e coleta o código de saída (útil para saber se o filho terminou com sucesso ou erro).
 * 

 *  */ 