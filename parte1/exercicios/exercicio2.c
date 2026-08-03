// Ver se tem instalado o gcc --version
// pra compilar -> gcc -Wall hello.c -o hello

#include <stdio.h> // Importando biblioteca de entrada e saída

// Função Principal/main
int main() {
    
    int numeros[10];
    int decimoNumero;
    
    // Adicionando 9 numeros no vetor
    printf("Digite 9 números:\n");
    for (int i = 0; i < 9; i++){
        printf("Número %d: ", i + 1);
        scanf("%d", &numeros[i]);
    }

    // Lógica de ordenamento do vetor
    for (int i = 0; i < 9; i++){
        for (int j = i + 1; j < 9; j++){
            if (numeros[i] > numeros[j]){   
                int aux = numeros[i];
                numeros[i] = numeros[j];
                numeros[j] = aux;
            }
        }
    }

    // Adicionando o 10º numero já ordenado no vetor
    printf("Digite o último número: ");
    scanf("%d", &decimoNumero);

    // Adicionando o 10º número na posição correta

    // Compara um a um
    int posicao = 0; // Inicia comparação no primeiro elemento do vetor
    // Enquanto a posição for menor que 9 e o numero da posição for menor que o decimo numero
    while (posicao < 9 && numeros[posicao] < decimoNumero){
        posicao++; // incrementa 1 posição
    }

    // Desloca os elementos do vetor para a direita
    for (int i = 9; i > posicao; i--){
        numeros[i] = numeros[i - 1];
    }

    // Adiciona o décimo número na posição correta
    numeros[posicao] = decimoNumero;

    // Imprime o vetor ordenado
    for (int i = 0; i < 10; i++){
        printf("%d\n", numeros[i]);
    }

    return 0; // Return 0 significa que o programa finalizou corretamente

}