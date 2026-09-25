// Objetivo: 
// Implementar, em linguagem C, uma simulação simplificada 
// do gerenciamento de tarefas de um Sistema Operacional
#include <stdio.h>
#include <string.h>

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

TCB criarTarefa(int id, char *nome, int instrucoes, int sp) {
    TCB t;
    t.id = id;
    snprintf(t.nome, sizeof(t.nome), "%s", nome);
    t.estado = PRONTA;
    t.pc = 0;
    t.quantum = 2;
    t.instrucoes = instrucoes;
    t.memoria = 0;
    t.sp = sp;
    for (int i = 0; i < 4; i++) {
        t.registradores[i] = 0;
    }

    return t;
}

int main() {
    // Criando as tarefas
    TCB T1 = criarTarefa(1, "Tarefa 1", 6, 100);
    TCB T2 = criarTarefa(2, "Tarefa 2", 4, 200);
    TCB T3 = criarTarefa(3, "Tarefa 3", 8, 300);
    
    TCB fila[3] = {T1, T2, T3}; // Onde as tarefas serão armazenadas

    printf("\nFila:\n");
    for(int i = 0; i < 3; i++) {
        printf("ID: %d, nome: %s, Estado: %d, PC:%d, Instrucoes restantes: %d, Memoria: %d, SP: %d\n",
                fila[i].id, fila[i].nome, fila[i].estado, fila[i].pc, fila[i].instrucoes, fila[i].memoria, fila[i].sp);
    }
    
    int tarefasFinalizadas = 0;
    int indice = 0;

    // TODO: melhorar, tá se tornando meio macarronico
    while(tarefasFinalizadas < 3) {
        // Verifica se a tarefa atual nao foi finalizada ainda
        if(fila[indice].estado != FINALIZADA) {
            fila[indice].estado = EXECUTANDO; // Muda o estado da tarefa para executando
            
            // Executando de fato a tarefa
            int execucoes = 0;
            while(execucoes < fila[indice].quantum && fila[indice].instrucoes > 0){
                // Incrementa os registradores? como fazer isso?
                fila[indice].memoria++;
                fila[indice].sp++;
                fila[indice].pc++; 
                
                fila[indice].instrucoes--;
                
                execucoes++;
            }
        }
        
        if (fila[indice].instrucoes == 0 && fila[indice].estado != FINALIZADA){
            fila[indice].estado = FINALIZADA; // Quando acaba as instruções, a tarefa tá finalizada
            tarefasFinalizadas++;
        } else if (fila[indice].estado != FINALIZADA) {
            fila[indice].estado = PRONTA; // Caso não esteja finalizada, volta a estar pronta
        }

        indice = (indice + 1) % 3; // Indice circular
    }

    printf("\nFila:\n");
    for(int i = 0; i < 3; i++) {
        printf("ID: %d, nome: %s, Estado: %d, PC:%d, Instrucoes restantes: %d, Memoria: %d, SP: %d\n",
                fila[i].id, fila[i].nome, fila[i].estado, fila[i].pc, fila[i].instrucoes, fila[i].memoria, fila[i].sp);
    }

// Considere um quantum de 2 instruções.
// Quando o quantum terminar:
    // 1 A tarefa atual deixa a CPU;
    // 2 Seu contexto permanece armazenado em sua TCB;
    // 3 Seu estado passa para PRONTA;
    // 4 A pr´oxima tarefa da fila ´e selecionada;
    // 5 O contexto da nova tarefa ´e carregado;
    // 6 Seu estado passa para EXECUTANDO;
    // 7 A execu¸c˜ao continua a partir de seu PC.
// Regra fundamental
// Uma tarefa n˜ao come¸ca novamente do in´ıcio.
// Ela continua a partir do contexto armazenado em sua TCB.


// Execução:
// Troca de contexto Salvando T1: PC = 2 Carregando T2: PC = 0
// Executando T2 PC: 0 -> 1 -> 2
// Troca de contexto Salvando T2: PC = 2 Carregando T3: PC = 0
// Quando T1 voltar a executar:
// T1 continua em PC = 2
}

