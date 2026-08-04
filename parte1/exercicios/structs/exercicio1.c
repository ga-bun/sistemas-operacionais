// Ver se tem instalado o gcc --version
// pra compilar -> gcc -Wall hello.c -o hello

#include <stdio.h> // Importando biblioteca de entrada e saída
#include <string.h> // Biblioteca de string

// Declarando a Struct (tipo a classe de Java/POO)
typedef struct {
    char nome[50];
    char telefone[50];

} Contato;

// Manter lista de contatos -> armazenar em um arranjo (array/vetor), possibilitando que vários registros sejam mantidos ao mesmo tempo

int main() {

    char listaDeContatos[10];

    // Inserir contato
    printf("INSERIR CONTATO:");

    // Novo contato:
    Contato contato;
    printf("Nome: ");
    gets(contato.nome, sideof(contato.nome), stdin);
    contato.nome[ strspn(contato.nome, "\n")] = '\0' // Remove o \n da string que fica do fgets
    
    printf("Telefone: ");
    gets(contato.telefone, sideof(contato.telefone), stdin);
    contato.telefone[ strspn(contato.telefone, "\n")] = '\0' // Remove o \n da string que fica do fgets
    
    // Descobrindo a posição livre do vetor:
    int posicao = 0;
    
    // Tamanho do vetor:
    int tamanhoListaDeContatosEmBytes = syzeof(listaDeContatos);
    int tamanhoListaDeContatos = tamanhoListaDeContatosEmBytes/sizeof(listaDeContatos[0]);
    int tamanhoListaDeContatosReal = 0;
    
    // Tenta encontrar a próxima posição vazia do vetor
    while (posicao < tamanhoListaDeContatos && listaDeContatos[i] != '\O') {
        posicao++;
    };

    // Caso não encontre, dobra o tamanho do vetor
    if (posicao == tamanhoListaDeContatos){
        char listaDeContatosAux[tamanhoListaDeContatos*2];
        strcpy(listaDeContatosAux, listaDeContatos);
        // ToDo/Pergunta: substitui agora a lista de contatos por lista de contatos auxiliar, como fazer????
    };

    listaDeContatos[posicao] = contato;
    // Incrementa o tamanho dos elementos contidos no vetor...
    tamanhoListaDeContatosReal++;
    
    // Remover contato
    printf("REMOVER CONTATO:");
    char nomeRemover;
    printf("Nome: ");
    gets(nomeRemover, sideof(nomeRemover), stdin);

    // Poderia colocar esse procura posição contato como uma função no futuro:: ToDo
    while (posicao < tamanhoListaDeContatos && listaDeContatos[i] != nomeRemover){
        posicao++;
    }

    if (posicao < tamanhoListaDeContatos){
        listaDeContatos[posicao].nome = listaDeContatos[tamanhoListaDeContatosReal-1].nome;
        listaDeContatos[tamanhoListaDeContatosReal-1].nome = '\O'; // Pergunta: Posso fazer isso?
        listaDeContatos[posicao].telefone = listaDeContatos[tamanhoListaDeContatosReal-1].telefone;
        listaDeContatos[tamanhoListaDeContatosReal-1].telefone = '\O';

        tamanhoListaDeContatosReal--;
    }
    
    // Atualizar contato
    printf("ATUALIZAR CONTATO:");
        
    char nomeAtualizar;
    gets(nomeAtualizar, sideof(nomeAtualizar), stdin);
    
    // Procura contato e atualiza dados do contato.....
    while (posicao < tamanhoListaDeContatos && listaDeContatos[i] != nomeAtualizar){
        posicao++;
    }

    // Atualiza o nome e telefone do contato
    if (posicao < tamanhoListaDeContatos){
        printf("Novo nome: ");
        gets(listaDeContatos[i].nome, sideof(listaDeContatos[i].nome), stdin);
        printf("Novo telefone: ");
        gets(listaDeContatos[i].telefone, sideof(listaDeContatos[i].telefone), stdin);
    }

    // Listar contatos
    printf("LISTAR CONTATOS:");
    for (int i = 0; listaDeContatos[i] != '\O'; i++) {
        printf("Contato %d: %s, %s", i+1, listaDeContatos[i].nome, listaDeContatos.telefone);
    }

    // Buscar contato
    printf("BUSCAR CONTATO:"); // Por nome... ToDo: telefone
    char nomeBuscar;
    printf("Nome: ");
    gets(nomeBuscar, sideof(nomeBuscar), stdin);

    // Posso fazer essa pesquisa por posição também, que nem fiz anteriormente
    for(int i = 0; listaDeContatos[i] != '\O'; i++){
        if(listaDeContatos[i].nome == nomeBuscar) {
            printf("Nome: %s, Telefone: %s", listaDeContatos[i].nome, listaDeContatos.telefone);
            return;
        }
    }

    return 0;

}