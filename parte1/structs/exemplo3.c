// Ver se tem instalado o gcc --version
// pra compilar -> gcc -Wall hello.c -o hello

#include <stdio.h> // Importando biblioteca de entrada e saída
#include <string.h> // Biblioteca de string

typedef struct {
    char nome[50];
    char especialidade[50];
} Medico;

typedef struct {
    char nome[50];
    int idade;
} Paciente;

// decomposição (igual java), aninhamento
typedef struct {
    Medico medico;
    Paciente paciente;
    char data[50];
    char descricao[];
} Consulta;

// * é o ponteiro: ele pega o endereço de memória da primeira posição do vetor
Consulta criarConsulta(Medico m, Paciente p, char *data, char *descricao) {

    Consulta c;
    c.medico = m;
    c.paciente = p;
    strcpy(c.descricao, descricao)
    strcpy(c.data, data)

    return c;
}