// Ver se tem instalado o gcc --version
// pra compilar -> gcc -Wall hello.c -o hello

#include <stdio.h> // Importando biblioteca de entrada e saída

// Função Principal/main
int main() {
    
    int numeros[10];
    int decimoNumero;
    
    // Adicionando 9 numeros no vetor
    for (int i = 0; i < 9; i++){
        printf("Digite um número: ");
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


    for (int i = 0; i < 10; i++){
        printf("%d\n", numeros[i]);
    }

    return 0; // Return 0 significa que o programa finalizou corretamente

}