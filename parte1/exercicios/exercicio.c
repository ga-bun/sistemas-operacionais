// Ver se tem instalado o gcc --version
// pra compilar -> gcc -Wall hello.c -o hello

#include <stdio.h> // Importando biblioteca de entrada e saída


// Função Principal/main
int main() {
    
    char quartos[10][2][100]; // Lista de quartos: 10 quartos, cada quarto tem 2 strings (nome e email)
    // Exemplo: quartos[i][0] = nome do locatário, quartos[i][1] = email do locatário
    int ocupado[10] = {0}; // Lista de quartos ocupados: 10 linhas -> 10 quartos; 0 = livre; 1 = ocupado; todos começam livres

    // Serão 3 quartos para locação:
    for (int i = 0; i < 4; i++){
        printf("Número do quarto a ser locado (0-9): ");
        int numeroDoQuarto;
        scanf("%d", &numeroDoQuarto);
        
        // Adicionar um verificador de quarto ocupado?
        
        printf("Digite o seu nome do locatário: ");
        fgets(quartos[numeroDoQuarto][0], sizeof(quartos[numeroDoQuarto][0]), stdin);
        
        printf("Digite o seu email do locatário: ");
        fgets(quartos[numeroDoQuarto][1], sizeof(quartos[numeroDoQuarto][1]), stdin);

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