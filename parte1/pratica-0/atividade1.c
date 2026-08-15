// Ver se tem instalado o gcc --version
// pra compilar -> gcc -Wall hello.c -o hello

// Escreva uma função para verificar se determinada String é igual a uma segunda String. 
// O acesso aos caracteres deve ser realizado com aritmética de ponteiros.

#include <stdio.h> // Importando biblioteca de entrada e saída
#include <string.h> // Biblioteca de string

// Passando o ponteiro das duas strings -> só recebe o endereço de memória do primeiro char e não a string inteira
int compararStrings(char *prtString1, char *prtString2) {
    // Verifica cada caracter das strings se são iguais:
    while(*prtString1 != '\0' && *prtString2 != '\0') {
        
        if(*prtString1 != *prtString2) {
            return 1; // Strings são diferentes
        }
        
        // Incrementa em 1 o conteúdo do ponteiro, ou seja, prox caractere da string
        prtString1++;
        prtString2++;
    }

    return 0; // Strings são iguais
}

int main () {

    char string1[100];
    char string2[100];

    printf("Digite a primeira string: ");
    fgets(string1, sizeof(string1), stdin);
    string1[strcspn(string1, "\n")] = '\0'; // Remove o \n da string que fica do fgets

    printf("Digite a segunda string: ");
    fgets(string2, sizeof(string2), stdin);
    string2[strcspn(string2, "\n")] = '\0'; // Remove o \n da string que fica do fgets

    if(compararStrings(string1, string2) == 1) {
        printf("As strings são diferentes.\n");
    } else {
        printf("As strings são iguais.\n");
    }

    return 0;
}