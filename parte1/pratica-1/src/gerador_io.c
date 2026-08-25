#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    printf("Gerando carga de E/S e Syscalls... Pressione Ctrl+C para parar.\n");
    int fd = open("/dev/null", O_WRONLY);
    char buffer[1024];
    while(1) {
    // Executa chamadas repetidas de escrita para for¸car tra¸c~ao no Kernel
    write(fd, buffer, sizeof(buffer));
    }
    return 0;
}