// Ver se tem instalado o gcc --version
// pra compilar -> gcc -Wall hello.c -o hello

// Escreva uma função para receber dois argumentos (números inteiros) x e y, calcular o produto
// entre as duas variáveis e retornar o resultado na própria variável y.


#include <stdio.h> // Importando biblioteca de entrada e saída

int calcularProduto(int *x, int *y) {
    return *x * *y;
}

int main () {
    int x;
    int y;

    printf("Digite x: ");
    scanf("%d", &x);
    
    printf("Digite y: ");
    scanf("%d", &y);

    y = calcularProduto(&x,&y);
    printf("%d\n", y);
    
    return 0;
}