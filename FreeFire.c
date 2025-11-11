#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// Código da Ilha – Edição Free Fire
// Nível: Mestre
// Este programa simula o gerenciamento avançado de uma mochila com componentes coletados durante a fuga de uma ilha.
// Ele introduz ordenação com critérios e busca binária para otimizar a gestão dos recursos.

#define MAX_ITENS 10
#define MAX_STRING 50

// Struct Item:
// Representa um componente com nome, tipo, quantidade e prioridade (1 a 5).
// A prioridade indica a importância do item na montagem do plano de fuga.
typedef struct {
    char nome[MAX_STRING];
    char tipo[MAX_STRING];
    int quantidade;
    int prioridade;
} Item;

// Enum CriterioOrdenacao:
// Define os critérios possíveis para a ordenação dos itens (nome, tipo ou prioridade).
typedef enum {
    NOME,
    TIPO,
    PRIORIDADE
} CriterioOrdenacao;

// Vetor mochila:
// Armazena até 10 itens coletados.
Item mochila[MAX_ITENS];

// Variáveis de controle: numItens (quantidade atual), comparacoes (análise de desempenho), ordenadaPorNome (para controle da busca binária).
int numItens = 0;
long long comparacoes = 0;
bool ordenadaPorNome = false;

void limparTela();
void limparBuffer();
void pressioneEnterParaContinuar();
void exibirMenu();
void inserirItem();
void removerItem();
void listarItens();
void menuDeOrdenacao();
void insertionSort(CriterioOrdenacao criterio);
void buscaBinariaPorNome();

int main() {
    // Menu principal com opções:
    // 1. Adicionar um item
    // 2. Remover um item
    // 3. Listar todos os itens
    // 4. Ordenar os itens por critério (nome, tipo, prioridade)
    // 5. Realizar busca binária por nome
    // 0. Sair

    int opcao;

    do {
        limparTela();
        exibirMenu();
        
        scanf("%d", &opcao);
        limparBuffer();

        // A estrutura switch trata cada opção chamando a função correspondente.
        switch (opcao) {
            case 1:
                inserirItem();
                break;
            case 2:
                removerItem();
                break;
            case 3:
                listarItens();
                break;
            case 4:
                menuDeOrdenacao();
                break;
            case 5:
                buscaBinariaPorNome();
                break;
            case 0:
                printf("Escapando da ilha... \n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }

        if (opcao != 0) {
            pressioneEnterParaContinuar();
        }

    } while (opcao != 0);

    // A ordenação e busca binária exigem que os dados estejam bem organizados.
    return 0;
}

// limparTela():
// Simula a limpeza da tela imprimindo várias linhas em branco.
void limparTela() {
    for(int i = 0; i < 50; i++) {
        printf("\n");
    }
}

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void pressioneEnterParaContinuar() {
    printf("\nPressione ENTER para continuar...");
    getchar();
}

// exibirMenu():
// Apresenta o menu principal ao jogador, com destaque para status da ordenação.
void exibirMenu() {
    printf("==========================================\n");
    printf(" DESAFIO CODIGO DA ILHA - FREE FIRE \n");
    printf("==========================================\n");
    printf("Status da Mochila: %d / %d itens\n", numItens, MAX_ITENS);
    
    if (ordenadaPorNome) {
        printf("Ordenacao: [OK - Por Nome]\n");
    } else {
        printf("Ordenacao: [Nao ordenada por nome]\n");
    }
    printf("------------------------------------------\n");
    printf("1. Adicionar item\n");
    printf("2. Remover item\n");
    printf("3. Listar itens\n");
    printf("4. Ordenar mochila...\n");
    printf("5. Buscar item (Busca Binaria por Nome)\n");
    printf("0. Sair\n");
    printf("------------------------------------------\n");
    printf("Escolha sua acao: ");
}

// inserirItem():
// Adiciona um novo componente à mochila se houver espaço.
// Solicita nome, tipo, quantidade e prioridade.
// Após inserir, marca a mochila como "não ordenada por nome".
void inserirItem() {
    if (numItens >= MAX_ITENS) {
        printf("Erro: Mochila cheia! Nao e possivel adicionar mais itens.\n");
        return;
    }

    printf("\n--- Adicionando Novo Item ---\n");
    Item novoItem;

    printf("Nome do item (ex: Colete): ");
    fgets(novoItem.nome, MAX_STRING, stdin);
    novoItem.nome[strcspn(novoItem.nome, "\n")] = 0; 

    printf("Tipo do item (ex: Protecao): ");
    fgets(novoItem.tipo, MAX_STRING, stdin);
    novoItem.tipo[strcspn(novoItem.tipo, "\n")] = 0; 

    printf("Quantidade: ");
    scanf("%d", &novoItem.quantidade);
    limparBuffer();

    printf("Prioridade (1-Baixa, 5-Alta): ");
    scanf("%d", &novoItem.prioridade);
    limparBuffer();

    mochila[numItens] = novoItem;
    numItens++;

    ordenadaPorNome = false; 
    
    printf("Sucesso: Item '%s' adicionado a mochila.\n", novoItem.nome);
}

// removerItem():
// Permite remover um componente da mochila pelo nome.
// Se encontrado, reorganiza o vetor para preencher a lacuna.
void removerItem() {
    if (numItens == 0) {
        printf("Mochila ja esta vazia.\n");
        return;
    }

    char nomeBusca[MAX_STRING];
    printf("\n--- Removendo Item ---\n");
    printf("Digite o NOME do item a remover: ");
    fgets(nomeBusca, MAX_STRING, stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = 0;

    int indiceEncontrado = -1;

    for (int i = 0; i < numItens; i++) {
        if (strcmp(mochila[i].nome, nomeBusca) == 0) {
            indiceEncontrado = i;
            break;
        }
    }

    if (indiceEncontrado == -1) {
        printf("Erro: Item '%s' nao encontrado na mochila.\n", nomeBusca);
    } else {
        for (int i = indiceEncontrado; i < numItens - 1; i++) {
            mochila[i] = mochila[i + 1];
        }
        numItens--;
        
        ordenadaPorNome = false; 

        printf("Sucesso: Item '%s' removido da mochila.\n", nomeBusca);
    }
}

// listarItens():
// Exibe uma tabela formatada com todos os componentes presentes na mochila.
void listarItens() {
    printf("\n--- Itens na Mochila (%d/%d) ---\n", numItens, MAX_ITENS);
    
    if (numItens == 0) {
        printf("A mochila esta vazia.\n");
        return;
    }

    printf("------------------------------------------------------------------\n");
    printf("%-25s | %-20s | %-5s | %-10s\n", "NOME", "TIPO", "QTD", "PRIORIDADE");
    printf("------------------------------------------------------------------\n");

    for (int i = 0; i < numItens; i++) {
        printf("%-25s | %-20s | %-5d | %-10d\n", 
               mochila[i].nome, 
               mochila[i].tipo, 
               mochila[i].quantidade, 
               mochila[i].prioridade);
    }
    printf("------------------------------------------------------------------\n");
}

// menuDeOrdenacao():
// Permite ao jogador escolher como deseja ordenar os itens.
// Utiliza a função insertionSort() com o critério selecionado.
// Exibe a quantidade de comparações feitas (análise de desempenho).
void menuDeOrdenacao() {
    if (numItens < 2) {
        printf("Nao e necessario ordenar. (Menos de 2 itens)\n");
        return;
    }

    int criterio;
    printf("\n--- Ordenar Mochila ---\n");
    printf("Escolha o criterio de ordenacao:\n");
    printf("1. Por Nome (A-Z)\n");
    printf("2. Por Tipo (A-Z)\n");
    printf("3. Por Prioridade (5-1, Decrescente)\n");
    printf("Opcao: ");
    
    scanf("%d", &criterio);
    limparBuffer();

    comparacoes = 0;

    switch (criterio) {
        case 1:
            insertionSort(NOME);
            printf("Mochila ordenada por NOME.\n");
            break;
        case 2:
            insertionSort(TIPO);
            printf("Mochila ordenada por TIPO.\n");
            break;
        case 3:
            insertionSort(PRIORIDADE);
            printf("Mochila ordenada por PRIORIDADE.\n");
            break;
        default:
            printf("Criterio invalido.\n");
            return;
    }

    printf("Ordenacao concluida. Total de comparacoes: %lld\n", comparacoes);
    listarItens();
}

// insertionSort():
// Implementação do algoritmo de ordenação por inserção.
// Funciona com diferentes critérios de ordenação:
// - Por nome (ordem alfabética)
// - Por tipo (ordem alfabética)
// - Por prioridade (da mais alta para a mais baixa)
void insertionSort(CriterioOrdenacao criterio) {
    ordenadaPorNome = (criterio == NOME);

    for (int i = 1; i < numItens; i++) {
        Item chave = mochila[i];
        int j = i - 1;
        bool deveTrocar = false;

        if (j >= 0) {
            comparacoes++;
            switch(criterio) {
                case NOME: 
                    deveTrocar = (strcmp(mochila[j].nome, chave.nome) > 0); 
                    break;
                case TIPO: 
                    deveTrocar = (strcmp(mochila[j].tipo, chave.tipo) > 0); 
                    break;
                case PRIORIDADE: 
                    deveTrocar = (mochila[j].prioridade < chave.prioridade); 
                    break;
            }
        }

        while (j >= 0 && deveTrocar) {
            mochila[j + 1] = mochila[j];
            j--;

            if (j >= 0) {
                comparacoes++;
                switch(criterio) {
                    case NOME: 
                        deveTrocar = (strcmp(mochila[j].nome, chave.nome) > 0); 
                        break;
                    case TIPO: 
                        deveTrocar = (strcmp(mochila[j].tipo, chave.tipo) > 0); 
                        break;
                    case PRIORIDADE: 
                        deveTrocar = (mochila[j].prioridade < chave.prioridade); 
                        break;
                }
            } else {
                deveTrocar = false;
            }
        }
        mochila[j + 1] = chave;
    }
}

// buscaBinariaPorNome():
// Realiza busca binária por nome, desde que a mochila esteja ordenada por nome.
// Se encontrar, exibe os dados do item buscado.
// Caso contrário, informa que não encontrou o item.
void buscaBinariaPorNome() {
    if (!ordenadaPorNome) {
        printf("\n=======================================================\n");
        printf("!! ATENCAO !!\n");
        printf("A Busca Binaria so funciona se a mochila estiver\n");
        printf("ORDENADA POR NOME. Use a Opcao 4 primeiro.\n");
        printf("=======================================================\n");
        return;
    }

    if (numItens == 0) {
        printf("Mochila esta vazia. Nada para buscar.\n");
        return;
    }

    char nomeBusca[MAX_STRING];
    printf("\n--- Busca Binaria por Nome ---\n");
    printf("Digite o NOME do item a buscar: ");
    fgets(nomeBusca, MAX_STRING, stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = 0; 

    int inicio = 0;
    int fim = numItens - 1;
    bool encontrado = false;

    while (inicio <= fim) {
        int meio = inicio + (fim - inicio) / 2;
        
        int cmp = strcmp(mochila[meio].nome, nomeBusca);

        if (cmp == 0) {
            printf("\n--- Item Encontrado! ---\n");
            printf("Nome:       %s\n", mochila[meio].nome);
            printf("Tipo:       %s\n", mochila[meio].tipo);
            printf("Quantidade: %d\n", mochila[meio].quantidade);
            printf("Prioridade: %d\n", mochila[meio].prioridade);
            encontrado = true;
            break;
        } else if (cmp < 0) {
            inicio = meio + 1;
        } else {
            fim = meio - 1;
        }
    }

    if (!encontrado) {
        printf("\nItem '%s' nao foi encontrado na mochila.\n", nomeBusca);
    }
}