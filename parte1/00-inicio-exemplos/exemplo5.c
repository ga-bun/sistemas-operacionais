// Ver se tem instalado o gcc --version
// pra compilar -> gcc -Wall hello.c -o hello

#include <stdio.h> // Importando biblioteca de entrada e saída
#include <string.h> // Biblioteca de string

int main(){
    
    // Strings
    char nome1[30] = "Microsoft";
    char nome2[30] = "Samsung";
    char nome3[30];

    // Fazendo uma cópia
    strcpy(nome3, nome1);
    printf("String copiada: %s\n", nome3);
    
    // Comparação entre strings, 0 == true e 1 == false
    if (strcmp(nome1, nome2) == 0){
        printf("Iguais\n");
    } else {
        printf("Diferentes\n");
    }

    return 0;
}