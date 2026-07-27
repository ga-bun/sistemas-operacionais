// Ver se tem instalado o gcc --version
// pra compilar -> gcc -Wall hello.c -o hello

#include <stdio.h> // Importando biblioteca de entrada e saída

// Definindo uma constante
#define num_tarefas 100
#define MENSAGEM "Bem-vindo"

// Função Principal/main
int main() {

    // Vetores
    // int tamanho_do_vetor = 5;
    // int numeros[tamanho_do_vetor]; // alocando na memória um vetor de 5 posições
    int identificadores[] = {1,2,3,4,5}; // aqui eu deixo o compilador identificar qual o tamanho
    
    // Pegando um elemento específico do vetor
    printf("%d\n", identificadores[1]);
    
    char nome[] = "IFSC";
    for (int i = 0; nome[i] != '\O'; i++);
        printf("%c\n", nome [i]);

    return 0; // Return 0 significa que o programa finalizou corretamente

}