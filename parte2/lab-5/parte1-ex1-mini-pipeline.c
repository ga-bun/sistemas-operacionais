// Exercício 1 – Mini Pipeline
// Implemente um programa equivalente ao comando:

// ls | wc -l

// O programa deve criar um pipe
// criar dois processos filhos
// redirecionar a saída do primeiro para o pipe
// redirecionar a entrada do segundo a partir do pipe
// executar os comandos com uma função da família exec

// O programa deve aguardar o término dos filhos utilizando uma função da família wait.
// Explique: o papel de pipe(), o uso de dup2() e a necessidade de fechar descritores.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() // Processo pai
{
    int fd[2];        // Descritores do pipe -> fd[0]: leitura; fd[1]: escrita
    pid_t pid1, pid2; // Processos filhos

    if (pipe(fd) == -1) // Tenta criar o canal de comunicação entre processos -> pipe
    {
        perror("pipe");
        return EXIT_FAILURE;
    }

    pid1 = fork(); // Cria fork 1 - filho 1

    if (pid1 == -1)
    {
        perror("fork");
        return EXIT_FAILURE;
    }

    if (pid1 == 0)
    {
        if (dup2(fd[1], STDOUT_FILENO) == -1) // dup2: Esse comando redireciona a saída padrão para o extremo de escrita do pipe (`fd[1]`).
        {
            perror("dup2");
            _exit(EXIT_FAILURE);
        }
        // Antes ls recebido escrevia na tela/terminal, agora escreve no pipe

        // - no processo pai, `pid1` recebe o PID do filho
        // - no processo filho, `pid1` recebe `0`

        // Fechar os dois lados do pipe (descritores) -> para o processo não ficar pendurado
        close(fd[0]);
        close(fd[1]);

        // execlp("nome do comando", "args[0]", final da lista de argumetnos com null)
        execlp("ls", "ls", (char *)NULL); // executa o ls em si, execlp executa um programa externo
        // A partir daqui o filho 1 agora executa o ls e o ls é redirecionado para o extremos do pipe - escrita

        perror("execlp ls");
        _exit(EXIT_FAILURE);
    }

    pid1 = fork(); // Cria fork 2 - filho 2

    if (pid1 == -1)
    {
        perror("fork");
        return EXIT_FAILURE;
    }

    if (pid1 == 0)
    {
        if (dup2(fd[0], STDOUT_FILENO) == -1) // dup2: Esse comando redireciona a saída padrão para o extremo de leitura do pipe (`fd[0]`)
        {
            perror("dup2");
            _exit(EXIT_FAILURE);
        }
        // Antes o wc -l lia do teclado agora vai ler do pipe
    
        // Fechar os dois lados do pipe (descritores) -> para o processo não ficar pendurado
        close(fd[0]);
        close(fd[1]);

        execlp("wc", "wc", "-l", (char *)NULL);
        perror("execlp wc");
        _exit(EXIT_FAILURE);
    }

    close(fd[0]);
    close(fd[1]);

    wait(NULL);
    wait(NULL);

    return EXIT_SUCCESS;
}