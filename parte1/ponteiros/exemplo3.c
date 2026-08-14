// faltei: ex prof

#include <stdio.h> 
#include <string.h>

typedef struct {
    char nome[50];
    int idade;
} Pessoa;

void alterarPessoa(Pessoa p) {
    p.idade = 99; // altera só a copia
}

void alterarPessoaReferencia(Pessoa *p) {
    p->idade = 99; // altera o original
}

int main() {
    Pessoa pessoa = {"João", 30};

    alterarPessoa(pessoa);
    printf("Idade :  %d\n", pessoa.idade);

    alterarPessoaReferencia(&pessoa);
    printf("Idade :  %d\n", pessoa.idade);

    return 0;


}