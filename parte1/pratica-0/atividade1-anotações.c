// Ver se tem instalado o gcc --version
// pra compilar -> gcc -Wall hello.c -o hello

// Escreva uma função para verificar se determinada String é igual a uma segunda String. 
// O acesso aos caracteres deve ser realizado com aritmética de ponteiros.

#include <stdio.h> // Importando biblioteca de entrada e saída
#include <string.h> // Biblioteca de string

// Passando o ponteiro das duas strings -> só recebe o endereço de memória do primeiro char e não a string inteira
int compararStrings(char *prtString1, char *prtString2) {
    printf("Conteúdo de str:\n");
    printf("String1: %c, String2: %c\n", *prtString1, *prtString2);
    printf("Endereço de memória das strings:\n");
    printf("String1: %p, String2: %p\n", prtString1, prtString2);
    
    return 0;
}

int main () {

    char string1[100];
    char string2[100];
    char *prtString1 = &string1[0]; // Ponteiro para o primeiro caractere da string1
    char *prtString2 = &string2[0]; // Ponteiro para o primeiro caractere da string2

    printf("Digite a primeira string: ");
    fgets(string1, sizeof(string1), stdin);
    string1[strcspn(string1, "\n")] = '\0'; // Remove o \n da string que fica do fgets

    printf("Digite a segunda string: ");
    fgets(string2, sizeof(string2), stdin);
    string2[strcspn(string2, "\n")] = '\0'; // Remove o \n da string que fica do fgets

    printf("\n");

    printf("Conteúdo das strings:\n");
    printf("String1: %s, String2: %s\n", string1, string2);

    printf("Conteúdo apontado pelos ponteiros -> conteúdo do primeiro elemento das strings:\n");
    printf("String1: %c, String2: %c\n", *prtString1, *prtString2);
    
    printf("\n");

    printf("Endereço de memória das strings:\n");
    printf("String1: %p, String2: %p\n", &string1, &string2);

    printf("Conteúdo dos ponteiros -> endereço de memória das strings\n");
    printf("String1: %p, String2: %p\n", prtString1, prtString2);

    printf("\n");

    compararStrings(string1, string2);

    return 0;
}