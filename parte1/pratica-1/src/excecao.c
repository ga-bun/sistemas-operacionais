#include <stdio.h>

int main() {
    int *p = NULL; // Ponteiro aponta para o endere¸co zero (protegido)
    printf("Tentando escrever em endereco de memoria proibido (NULL)...\n");
    *p = 999; // Provoca exce¸c~ao de hardware gerada pela MMU
    return 0;
}