// faltei: ex prof

#include <stdio.h> 
#include <string.h>
#include <stdlib.h>

typedef struct Node
{
    int valor;
    struct  Node *prox;   
} Node;


Node* criaNo(int v) {
    Node *novo =  (Node*) malloc(sizeof(Node));
    novo-> valor = v;
    novo-> prox = NULL;
    return novo;
}

int main() {
    Node *n1 = criaNo(10);
    Node *n2 = criaNo(100);
    n1->prox = n2;
    printf("Primeiro:  %d,   Segundo:  %d\n"  , n1->valor, n1->prox->valor);
    free(n1);
    free(n2);
    return 0;
}


