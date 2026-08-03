// Ver se tem instalado o gcc --version
// pra compilar -> gcc -Wall hello.c -o hello

#include <stdio.h> // Importando biblioteca de entrada e saída

// Função Principal/main
int main() {

    int coordenadas[2];
    while (1) {
        printf("Digite coordenadas: \n");
        printf("X: ");
        scanf("%d", &coordenadas[0]);
        printf("Y: ");
        scanf("%d", &coordenadas[1]);

        if (coordenadas[0] == 0 || coordenadas[1] == 0) {
            break;
        }

        // Verificando o quadrante
        // Quadrante 1: x > 0, y > 0
        // Quadrante 4: x > 0, y < 0
        if (coordenadas[0] > 0 ) {
            if (coordenadas[1] > 0) {
                printf("Quadrante 1\n");
            } else {
                printf("Quadrante 4\n");
            }
        }
        // Quadrante 2: x < 0, y > 0
        // Quadrante 3: x < 0, y < 0
        if (coordenadas [0] < 0 ) {
            if (coordenadas[1] > 0) {
                printf("Quadrante 2\n");
            } else {
                printf("Quadrante 3\n");
            }
        }
    }

    return 0; // Return 0 significa que o programa finalizou corretamente
}