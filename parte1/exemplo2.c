// Ver se tem instalado o gcc --version
// pra compilar -> gcc -Wall hello.c -o hello

#include <stdio.h> // Importando biblioteca de entrada e saída

// Definindo uma constante
#define num_tarefas 100
#define MENSAGEM "Bem-vindo"

// Função Principal/main
int main() {
    printf("%s\n", MENSAGEM);

    int idade = 15;

    // If, else, switch é a mesma coisa do que de java
    if (idade >= 18) {
        printF("Maior de idade");
    }
    
    return 0; // Return 0 significa que o programa finalizou corretamente

}