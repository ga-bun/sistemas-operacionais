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
    // Criando um arranjo com 3 pessoas
    Pessoa pessoas[3] = {
        {"João",22,"Dos Anjos",200},
        {"Maria",24,"Lago do Cisnes",400},
        {"Fabio",26,"Faria Lima",450}
    };

    for (int i = 0; i < 3; i++) {
        printf("Nome:  %s\n ", pessoas[i].nome);
        printf("Idade:  %s\n ", pessoas[i].idade);
        printf("Rua:  %s, %s\n  ", pessoas[i].rua, pessoas[i].numero); 
    }

    return 0;

}