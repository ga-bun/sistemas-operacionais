// Objetivo: 
// Implementar, em linguagem C, uma simulação simplificada 
// do gerenciamento de tarefas de um Sistema Operacional
#include <stdio.h>

// Definindo a struct TCB
typedef struct {
    int id; // Identificador da tarefa
    int estado; // 0 = PRONTA, 1 = EXECUTANDO, 2 = FINALIZADA
    int pc; // Contador de programa, indicando a próxima instução a executar
    int registradores[4]; // Memória - registradores fictícios
    int instrucoes; // Quantidade de instruções que ainda precisam ser executadas

    char nome[20]; // Nome da tarefa
    int quantum; // Instruções por quantum
    int memoria; // Local fictício de memória onde parou
    int sp; // Stack Pointer fictício
} TCB; // Task Control Block - Estrutura que representa a tarefa para o SO

// Constantes para o estado
#define PRONTA 0
#define EXECUTANDO 1
#define FINALIZADA 2

int main() {
    TCB T1 = {
        .id = 1,
        .nome = "Tarefa 1",
        .estado = PRONTA,
        .pc = 0,
        .quantum = 2,
        .instrucoes = 6,
        .sp = 100
    };

    TCB T2 = {
        .id = 2,
        .nome = "Tarefa 2",
        .estado = PRONTA,
        .pc = 0,
        .quantum = 2,
        .instrucoes = 4,
        .sp = 200
    };

    TCB T3 = {
        .id = 3,
        .nome = "Tarefa 3",
        .estado = PRONTA,
        .pc = 0,
        .quantum = 2,
        .instrucoes = 8,
        .sp = 300
    };
    
    printf("ID: %d\n", T1.id);
    printf("Nome: %s\n", T1.nome);
    printf("Estado: %d\n", T1.estado);
    printf("PC: %d\n", T1.pc);
    printf("Quantum: %d\n", T1.quantum);
    printf("Instrucoes restantes: %d\n", T1.instrucoes);
    printf("Memoria: %d\n", T1.memoria);
    printf("SP: %d\n", T1.sp);
    for (int i = 0; i < 4; i++) {
        printf("Registrador[%d]: %d\n", i, T1.registradores[i]);
    }
    
    TCB fila[3] = {T1, T2, T2}; // Onde as tarefas serão armazenadas
}

