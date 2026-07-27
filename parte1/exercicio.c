// Ver se tem instalado o gcc --version
// pra compilar -> gcc -Wall hello.c -o hello

#include <stdio.h> // Importando biblioteca de entrada e saída

// Função Principal/main
int main() {
    
    char quartos[10];
    char dadosLocatario[2];
    int numeroDoQuarto;

    for (int i = 0; i < 4; i++){
        printf("Digite o seu nome: ");
        fgets(dadosLocatario[1], sizeof(dadosLocatario[1]), stdin);
        printf("Digite o seu email: ");
        fgets(dadosLocatario[2], sizeof(dadosLocatario[2]), stdin);
        printf("Qual quarto você gostaria de alugar? ");
        scanf("%d", &numeroDoQuarto);

        if(quartos[numeroDoQuarto] != '\O') {
            quartos[numeroDoQuarto] = dadosLocatario;
        }
    }

    for (int i = 0; i < 10; i++){
        if (quartos[i]!='\O'){
            printf("%d: %s", i, quartos[i]);
        }
    }
    
    return 0; // Return 0 significa que o programa finalizou corretamente

}