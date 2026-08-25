#include <unistd.h>
#include <sys/syscall.h>

int main() {
    char msg[] = "Escrita realizada via Syscall direta sem printf!\n";
    
    // syscall(c´odigo_da_syscall, file_descriptor, ponteiro_buffer, tamanho_bytes)
    // SYS_write = opcode 1 no Linux x86_64
    // 1 = file descriptor para sa´ıda padr~ao (stdout)
    syscall(SYS_write, 1, msg, sizeof(msg) - 1);
    
    return 0;
}