// Ver se tem instalado o gcc --version
// pra compilar -> gcc -Wall hello.c -o hello

#include <stdio.h> // Importando biblioteca de entrada e saída

// Definindo uma constante
#define num_tarefas 100
#define MENSAGEM "Bem-vindo"

// Função Principal/main
int main() {

    // fgets
    // char curso[] = "Computação";

    char nome[100];

    printf("Digite o seu nome: ");
    fgets(nome, sizeof(nome), stdin); // sizeof retorna em bytes

    printf("%s", nome);

    int numeros[] = {1,2,3,4,5};
    int tamanho_em_bytes = sizeof(numeros);
    printf("%d\n", tamanho_em_bytes);
    int tamanho_vetor = tamanho_em_bytes/sizeof(numeros[0]);

    printf("Comprimento do vetor numeros = %d\n", tamanho_vetor);

    return 0; // Return 0 significa que o programa finalizou corretamente

}