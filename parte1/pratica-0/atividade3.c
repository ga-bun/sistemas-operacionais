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

Aluno cadastrarAluno(char *nome, char *email, int matricula, Curso curso) {
    Aluno aluno;
    
    aluno.curso = curso;
    aluno.matricula = matricula;
    strcpy(aluno.nome, nome);
    strcpy(aluno.email, email);

    return aluno;
}

Curso cadastrarCurso(char *nome, Universidade universidade) {
    Curso curso;
    
    strcpy(curso.nome, nome);
    curso.universidade = universidade;
    
    return curso;
}

Universidade cadastrarUniversidade(char *nome, char *cidade) {
    Universidade universidade;

    strcpy(universidade.nome, nome);
    strcpy(universidade.cidade, cidade);

    return universidade;
}

int main () {
    Universidade ufsc = cadastrarUniversidade("UFSC", "Florianópolis");
    Universidade ifscSJ = cadastrarUniversidade("IFSC", "São José");
    
    Curso fisicaUfsc = cadastrarCurso("Física", ufsc);
    Curso sistemasUfsc = cadastrarCurso("Sistemas da Informação", ufsc);

    Curso adsIfscSJ = cadastrarCurso("ADS", ifscSJ);
    Curso quimicaIfscSj = cadastrarCurso("Química", ifscSJ);

    // Cadastro com array de alunos
    Aluno alunos[4];

    alunos[0] = cadastrarAluno("Pedro", "pedro@grad.ufsc.br", 1234567890, fisicaUfsc);
    alunos[1] = cadastrarAluno("Marta", "marta@grad.ufsc.br", 1234567891, sistemasUfsc);
    alunos[2] = cadastrarAluno("Gabriel", "gabriel@aluno.ifsc.edu.br", 1234567892, adsIfscSJ);
    alunos[3] = cadastrarAluno("Flora", "flora@aluno.ifsc.edu.br", 1234567893, quimicaIfscSj);

    // Relembrando length do array: 
    // sizeof(numeros) -> tamanho em bytes
    // tamanho_em_bytes/sizeof(numeros[0]) -> quantidade de elementos: tamanho total em bytes dividido pelo tamanho em bytes do primeiro elemento
    int tamanhoAlunos = sizeof(alunos)/sizeof(alunos[0]);

    for(int i=0; i<tamanhoAlunos; i++){
        printf("Nome: %s\n", alunos[i].nome);
        printf("Email: %s\n", alunos[i].email);
        printf("Matricula: %d\n", alunos[i].matricula);
        printf("Curso: %s\n", alunos[i].curso.nome);
        printf("Universidade: %s\n", alunos[i].curso.universidade.nome);
        printf("Cidade universidade: %s\n", alunos[i].curso.universidade.cidade);
    }

    return 0;
}