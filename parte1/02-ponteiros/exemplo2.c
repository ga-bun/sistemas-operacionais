// Ver se tem instalado o gcc --version
// pra compilar -> gcc -Wall hello.c -o hello

#include <stdio.h> // Importando biblioteca de entrada e saída
#include <string.h> // Biblioteca de string

// Comparar conteúdo e comparar endereço de memória
int main () {
    double *a, *b, c, d;

    // Apontando
    a = &d;
    b = &c;

    c = 3.14;
    d = 7.14;

    // Comparando conteúdo
    if ( *a < *b ) {
        printf("Valor apontado por a (d) é menor que o valor apontado por b (c).\n");
    } else {
        printf("Valor apontado por a (d) não é menor que o valor apontado por b (c).\n");
    }

    // Comparando endereço
    if ( a < b ) {
        printf("Endereço de armazenado em a (endereço de d) é menor que o endereço armazenado em b (endereço de c).\n");
    } else {
        printf("Endereço de armazenado em a (endereço de d) não é menor que o endereço armazenado em b (endereço de c).\n");
    }

    return 0;
}