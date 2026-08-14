// Ver se tem instalado o gcc --version
// pra compilar -> gcc -Wall hello.c -o hello

// Escreva uma função para verificar se determinada String é igual a uma segunda String. 
// O acesso aos caracteres deve ser realizado com aritmética de ponteiros.

#include <stdio.h> // Importando biblioteca de entrada e saída
#include <string.h> // Biblioteca de string

// * significa que estou pegando o VALOR da variavel presente no endereço de memória passado
int compararStrings(char *string1, char *string2) {

}

int main () {

    char string1[100];
    char string2[100];

    printf("Digite a primeira string: ");
    fgets(string1, sizeof(string1), stdin);

    printf("Digite a segunda string: ");
    fgets(string2, sizeof(string2), stdin);

    // & significa que estou passando o endereço de memória da variavel (referencia)
    compararStrings(&string1, &string2);

    return 0;

}