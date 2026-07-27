// Ver se tem instalado o gcc --version
// pra compilar -> gcc -Wall hello.c -o hello

#include <stdio.h> // Importando biblioteca de entrada e saída

// Função Principal/main
int main() {

    printf("Olá mundo! \n"); // Pra não dar azar

    // Tipos de dados primitivos -> se alguma variável não for usada o vscode vai reclamar
    int idade = 20;
    float altura = 1.75;
    double pi = 3.1415;
    char letra = 'M'; // String é um vetor de char

    printf("Minha idade é %d\n", idade);
    printf("Minha altura é %.2f e o pi é %.4f\n", altura, pi);
    printf("Qual a letra: %c\n", letra);
    
    return 0; // Return 0 significa que o programa finalizou corretamente

}