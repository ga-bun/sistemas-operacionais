// Scanner, input, console.log, etc etc

#include <stdio.h> // Importando biblioteca de entrada e saída

// Função principal
int main(){

    int numero;
    // float preco;
    // char letra;

    // Entrada de dados
    printf("Digite um numero inteiro: ");
    scanf("%d", &numero); // & -> Acessando o endereço de memória dessa variável

    // scanf("%f", &preco);
    // scanf("%c", &letra);
    
    printf("O número que você digitou foi: %d\n", numero);

    return 0;

}