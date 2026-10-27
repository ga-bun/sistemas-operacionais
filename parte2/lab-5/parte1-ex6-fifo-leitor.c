// Exercício 6 – FIFO Nomeada (atividade adicional)
// Crie uma FIFO usando mkfifo(). Implemente dois programas independentes: um escritor e um leitor, que se comuniquem por meio do nome da FIFO no sistema de arquivos.
// Observe e explique: o comportamento de open() quando não existe outra extremidade
// aberta, a diferença entre a entrada da FIFO no sistema de arquivos e os dados armazenados
// no núcleo, e o efeito de unlink().

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define NOME_FIFO "/tmp/fifo_lab_ex6"

int main(void) {
    char buffer[128];
    ssize_t n;
    int fd;

    setvbuf(stdout, NULL, _IONBF, 0);

    if (mkfifo(NOME_FIFO, 0666) == -1 && errno != EEXIST) {
        perror("mkfifo");
        return EXIT_FAILURE;
    }
    printf("[leitor] FIFO criada ou existente: %s\n", NOME_FIFO);

    printf("[leitor] open() para leitura, bloqueia ate existir escritor\n");
    fd = open(NOME_FIFO, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return EXIT_FAILURE;
    }
    printf("[leitor] FIFO aberta para leitura\n");

    while ((n = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[n] = '\0';
        printf("[leitor] recebida: %s\n", buffer);
    }

    if (n == 0) {
        printf("[leitor] EOF: nenhum escritor com a FIFO aberta\n");
    } else {
        perror("read");
    }

    close(fd);

    if (unlink(NOME_FIFO) == -1) {
        perror("unlink");
        return EXIT_FAILURE;
    }
    printf("[leitor] entrada removida do sistema de arquivos\n");

    return EXIT_SUCCESS;
}
