// faltei: ex prof
#include <stdio.h> 
#include <string.h>
#include <stdlib.h>


int main () {
    char *p = (char*) malloc (100);

    if (p == NULL) {
        printf("Erro de alocação \n");
        return 1;
    }

    for (int i  = 0; i < 100; i++) {
        p[i] = 'A';
    }

    // expandindo array
    p = (char*) realloc(p, 200);
    if (p == NULL) {
        printf("Erro de alocação \n");
        return 1;
    }

    for (int i  = 100; i < 200; i++) {
        p[i] = 'W';
    }
    printf("Primeiro byte:  %c\n", p[0]);
    printf("byte 150 :  %c\n", p[150]);
    free(p);
    return 0;




}
