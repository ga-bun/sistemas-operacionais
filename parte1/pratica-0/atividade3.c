// Atividade 3
// Imagine que você está desenvolvendo um sistema acadêmico para organizar informações de uma universidade. Nesse sistema, cada aluno está matriculado em um curso, 
// e cada curso pertence a uma universidade.

// O objetivo é representar essa relação hierárquica em linguagem C, utilizando structs aninhadas:
// - O aluno deve guardar seus dados pessoais e o curso que ele faz. OK
// - O curso deve armazenar suas informações e também a universidade à qual pertence. OK
// - A universidade deve conter seus próprios dados, como nome e cidade. OK

// No final, o programa deve permitir cadastrar um array de alunos, preenchendo os dados diretamente no código, 
// e exibir na tela todas as informações, mostrando claramente que:
// - O aluno faz um curso.
// - O curso pertence a uma universidade.

// ------------------------------------------
// Ver se tem instalado o gcc --version
// pra compilar -> gcc -Wall hello.c -o hello
// ------------------------------------------

#include <stdio.h> // Importando biblioteca de entrada e saída
#include <string.h> // Biblioteca de string

typedef struct {
    char nome[100];
    char cidade[100];
} Universidade;

typedef struct {
    char nome[50];
    Universidade universidade;
} Curso;

typedef struct {
    char nome[100];
    char email[100];
    int matricula;
    Curso curso;
} Aluno;

int main () {

    return 0;
}