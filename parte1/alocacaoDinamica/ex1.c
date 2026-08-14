// faltei: ex prof

#include <stdio.h> 
#include <string.h>
#include <stdlib.h>

int main() {


    int n = 10;
    int *p;
    p = (int*) malloc (n * sizeof(int));    
    if (p == NULL) {
        printf("Erro de alocação\n");
        return 1;
    }
    *p = 30;
    printf("Valor %d\n",  *p);
    free(p);
    return 0;







}