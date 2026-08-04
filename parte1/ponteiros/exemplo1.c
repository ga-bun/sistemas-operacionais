// Ver se tem instalado o gcc --version
// pra compilar -> gcc -Wall hello.c -o hello

#include <stdio.h> // Importando biblioteca de entrada e saída
#include <string.h> // Biblioteca de string

int main () {

    int numero = 10;
    int *ptr;

    // Ponteiro aponte para o endereço de memória da variável numero
    ptr = &numero;
    printf("Valor do numero: %d\n", numero);
    printf("Endereço do numero: %p \n", numero);
    printf("Valor do ponteiro (endereço): %p \n", ptr);
    printf("Conteúdo apontado por ptr: %d \n", *ptr);
    printf("Conteúdo do ponteiro: %d\n", ptr);

    *ptr = 20;

    printf("Novo valor de numero: %d\n", numero);
    return 0;
}