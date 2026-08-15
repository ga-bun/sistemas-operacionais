// Ver se tem instalado o gcc --version
// pra compilar -> gcc -Wall hello.c -o hello

#include <stdio.h> // Importando biblioteca de entrada e saída
#include <string.h> // Biblioteca de string

// Declarando a Struct (tipo a classe de Java/POO)
typedef struct {
    char nome[50];
    int idade;
    char rua[50];
    int numero;

} Pessoa;

int main() {
    // Instanciando a pessoa
    Pessoa p1; // struct Pessoa p1; como colocamos o typedef struct lá em cima, pode usar só Pessoa p1
    strcpy(p1.nome, "Martha");
    p1.idade = 47;
    p1.numero = 200;

    // Pra pegar o atributo preciso fazer exatamente o que está na terceira linha
    printf("Digite o nome da rua: ");
    fgets(p1.rua, sizeof(p1.rua), stdin);
    p1.rua[ strcspn(p1.rua, "\n")] = '\0'; // Remove o \n da string que fica do fgets

    return 0;

}