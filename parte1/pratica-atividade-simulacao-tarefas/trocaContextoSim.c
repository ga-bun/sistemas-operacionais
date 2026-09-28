// Objetivo: 
// Implementar, em linguagem C, uma simulação simplificada 
// do gerenciamento de tarefas de um Sistema Operacional
#include <stdio.h>

// Definindo a struct TCB
typedef struct {
    int id; // Identificador da tarefa
    char nome[20]; // Nome da tarefa
    int estado; // 0 = PRONTA, 1 = EXECUTANDO, 2 = FINALIZADA
    int pc; // Contador de programa, indicando a próxima instução a executar
    int quantum; // Instruções por quantum
    int instrucoes; // Quantidade de instruções que ainda precisam ser executadas
    int memoria; // Local fictício de memória onde parou
    int registradores[4]; // Memória - registradores fictícios
    int sp; // Stack Pointer fictício

} TCB; // Task Control Block - Estrutura que representa a tarefa para o SO

// Constantes para o estado
#define PRONTA 0
#define EXECUTANDO 1
#define FINALIZADA 2
#define TAMANHO_FILA 3
#define QUANTUM 2

TCB criarTarefa(int id, char *nome, int instrucoes, int sp) {
    TCB t;
    t.id = id;
    snprintf(t.nome, sizeof(t.nome), "%s", nome);
    t.estado = PRONTA;
    t.pc = 0;
    t.quantum = QUANTUM;
    t.instrucoes = instrucoes;
    t.memoria = 0;
    t.sp = sp;
    for (int i = 0; i < 4; i++) {
        t.registradores[i] = 0;
    }

    return t;
}

void imprimirEstadoFila(TCB fila[], int tamanhoFila) {
    printf("\nFila:\n");
    for(int i = 0; i < tamanhoFila; i++) {
        printf("ID: %d, nome: %s, Estado: %d, PC:%d, Instrucoes restantes: %d, SP: %d\n",
                fila[i].id, fila[i].nome, fila[i].estado, fila[i].pc, fila[i].instrucoes, fila[i].sp);
    }
}

void executarTarefa(TCB *tarefa) {
    int execucoes = 0;

    while(execucoes < tarefa->quantum && tarefa->instrucoes > 0){
        tarefa->pc++; 
        tarefa->instrucoes--;
        
        execucoes++;
    }
}

int main() {
    // Criando as tarefas
    TCB T1 = criarTarefa(1, "Tarefa 1", 6, 100);
    TCB T2 = criarTarefa(2, "Tarefa 2", 4, 200);
    TCB T3 = criarTarefa(3, "Tarefa 3", 8, 300);
    
    TCB fila[TAMANHO_FILA] = {T1, T2, T3}; // Onde as tarefas serão armazenadas

    printf("\n-------- ESTADO INICIAL DA FILA --------");
    imprimirEstadoFila(fila, TAMANHO_FILA);
    
    int tarefasFinalizadas = 0;
    int indice = 0;

    while(tarefasFinalizadas < TAMANHO_FILA) {
        // Verifica se a tarefa atual nao foi finalizada ainda
        if(fila[indice].estado != FINALIZADA) {
            fila[indice].estado = EXECUTANDO; // Muda o estado da tarefa para executando
        }

        executarTarefa(&fila[indice]); // Passando o endereço da tarefa
        
        imprimirEstadoFila(fila, TAMANHO_FILA);
        
        if (fila[indice].instrucoes == 0 && fila[indice].estado != FINALIZADA){
            fila[indice].estado = FINALIZADA; // Quando acaba as instruções, a tarefa tá finalizada
            tarefasFinalizadas++;
        } else if (fila[indice].estado != FINALIZADA) {
            fila[indice].estado = PRONTA; // Caso não esteja finalizada, volta a estar pronta
        }

        indice = (indice + 1) % TAMANHO_FILA; // Indice circular
    }

    printf("\n-------- ESTADO FINAL DA FILA --------");
    imprimirEstadoFila(fila, TAMANHO_FILA);
    
}

