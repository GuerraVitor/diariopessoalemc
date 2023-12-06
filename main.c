#include "listavitor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ENTRY_LENGTH 1000
#define TAMANHO_MAX_SENHA 50
#define MAX_TAMANHO 200

const char *nome_diretorio = "ArquivosUsuario";

// Estrutura de dados auxiliar
typedef struct {
    DATA data;
    char entry[MAX_ENTRY_LENGTH];
} DATAFORNECIDA;

// Função que grava o texto no arquivo
void gravarTextoNoArquivo(char* texto, const char* nome_diretorio, DATA data) {
    FILE* arquivo;
    char nomeArquivo[MAX_TAMANHO];

    // Criando o nome do arquivo usando os valores da estrutura de data
    sprintf(nomeArquivo, "%d_%d_%d.txt", data.dia, data.mes, data.ano);

    // Abrindo o arquivo para escrita
    char caminhoArquivo[MAX_TAMANHO + strlen(nome_diretorio) + 2]; // +2 para o '/' e o caractere nulo
    sprintf(caminhoArquivo, "%s/%s", nome_diretorio, nomeArquivo);
    arquivo = fopen(caminhoArquivo, "w");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    // Gravando o texto no arquivo
    fputs(texto, arquivo);

    // Fechando o arquivo
    fclose(arquivo);
}

// Função que adciona o texto no final arquivo
void adcionaTextoNoArquivo(char* texto, const char* nome_diretorio, DATA data) {
    FILE* arquivo;
    char nomeArquivo[MAX_TAMANHO];

    // Criando o nome do arquivo usando os valores da estrutura de data
    sprintf(nomeArquivo, "%d_%d_%d.txt", data.dia, data.mes, data.ano);

    // Abrindo o arquivo para escrita no final
    char caminhoArquivo[MAX_TAMANHO + strlen(nome_diretorio) + 2]; // +2 para o '/' e o caractere nulo
    sprintf(caminhoArquivo, "%s/%s", nome_diretorio, nomeArquivo);
    arquivo = fopen(caminhoArquivo, "a");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    // Gravando o texto no arquivo
    fputs(texto, arquivo);

    // Fechando o arquivo
    fclose(arquivo);
}

PONT buscaBinaria(LISTA* lista, DATAFORNECIDA dataScaneada) {
    PONT inicio = lista->inicio;
    PONT fim = NULL;
    PONT meio;

    while (inicio != fim) {
        int distancia = 0;
        meio = inicio;

        // Calcula a distância entre os ponteiros
        while (meio != fim && distancia < 2) {
            meio = meio->prox;
            distancia++;
        }

        // Verifica se a data do meio é a procurada
        if (meio->reg.DATA.dia == dataScaneada.data.dia &&
            meio->reg.DATA.mes == dataScaneada.data.mes &&
            meio->reg.DATA.ano == dataScaneada.data.ano) {
            return meio;
        } else if (meio->reg.DATA.ano < dataScaneada.data.ano ||
                   (meio->reg.DATA.ano == dataScaneada.data.ano && meio->reg.DATA.mes < dataScaneada.data.mes) ||
                   (meio->reg.DATA.ano == dataScaneada.data.ano && meio->reg.DATA.mes == dataScaneada.data.mes && meio->reg.DATA.dia < dataScaneada.data.dia)) {
            fim = meio;
        } else {
            inicio = meio->prox;
        }
    }

    return NULL;
}

//funcao que exclui elemento especifico da lista
void removerElemento(LISTA* lista, DATAFORNECIDA dataScaneada) {
    PONT atual = lista->inicio;
    PONT anterior = NULL;

    while (atual != NULL) {
        if (atual->reg.DATA.dia == dataScaneada.data.dia &&
            atual->reg.DATA.mes == dataScaneada.data.mes &&
            atual->reg.DATA.ano == dataScaneada.data.ano) {

            if (anterior == NULL) {
                lista->inicio = atual->prox; // O elemento a ser removido é o primeiro da lista
            } else {
                anterior->prox = atual->prox; // Elemento a ser removido está no meio ou no final da lista
            }

            free(atual); // Libera memória do nó removido
            return;
        }

        anterior = atual;
        atual = atual->prox;
    }

    printf("\nElemento não encontrado na lista.\n");
}

void adicionarEntrada(LISTA* lista) {
    DATAFORNECIDA dataScaneada;
    printf("Digite a data (DD MM AAAA): ");

    // Verificando se existe entrada com esta data
    scanf("%d %d %d", &dataScaneada.data.dia, &dataScaneada.data.mes, &dataScaneada.data.ano);

    if (buscarData(lista, dataScaneada.data.dia, dataScaneada.data.mes, dataScaneada.data.ano)) {
        printf("\ndata já ocupada.\n");
    } 
    else {
        inserirElemento(lista, dataScaneada.data); // Insere data na lista
        ordenarLista(lista); // Ordena a lista 
        printf("Digite a entrada: ");
        getchar(); // Limpar o buffer do teclado
        fgets(dataScaneada.entry, MAX_ENTRY_LENGTH, stdin);
        gravarTextoNoArquivo(dataScaneada.entry, nome_diretorio, dataScaneada.data);

        printf("\nEntrada adicionada com sucesso!\n");
    }
}


void editarEntrada(LISTA* lista){
    DATAFORNECIDA dataScaneada;
    printf("Digite a data (DD MM AAAA): ");

    // Verificando se existe entrada com esta data
    scanf("%d %d %d", &dataScaneada.data.dia, &dataScaneada.data.mes, &dataScaneada.data.ano);

    if (buscarData(lista, dataScaneada.data.dia, dataScaneada.data.mes, dataScaneada.data.ano)){
        printf("Digite a entrada (sera adcionado no final do arquivo): ");
        getchar();
        fgets(dataScaneada.entry, MAX_ENTRY_LENGTH, stdin);
        adcionaTextoNoArquivo(dataScaneada.entry, nome_diretorio, dataScaneada.data);

        printf("\nEntrada adcionado ao registro anterior com sucesso!\n");
    }   
    else {
        printf("\nNao existe arquivo com a data fornecida\n");
    }
}

void LerEntradaPorData(LISTA* lista) {
    DATAFORNECIDA dataScaneada;
    printf("Digite a data (DD MM AAAA): ");
  
    scanf("%d %d %d", &dataScaneada.data.dia, &dataScaneada.data.mes, &dataScaneada.data.ano);

    if (buscarData(lista, dataScaneada.data.dia, dataScaneada.data.mes, dataScaneada.data.ano)) {
        FILE* arquivoBuscado;
        char caminhoArquivoBuscado[MAX_TAMANHO + strlen(nome_diretorio) + 15];
        sprintf(caminhoArquivoBuscado, "%s/%d_%d_%d.txt", nome_diretorio, dataScaneada.data.dia, dataScaneada.data.mes, dataScaneada.data.ano);

        arquivoBuscado = fopen(caminhoArquivoBuscado, "r");
        if (arquivoBuscado == NULL) {
            printf("Erro ao abrir o arquivo.\n");
            return;
        }

        printf("\nA entrada desse dia é:\n"); 
        char linha[1000];

        // Ler uma linha até que não haja mais linhas para ler
        while (fgets(linha, sizeof(linha), arquivoBuscado) != NULL) {
            printf("%s", linha);
        }

        fclose(arquivoBuscado);
    } else {
        printf("\nNão há entrada para esta data.\n");
    }
}

void listarEntradasPorDatas(LISTA* lista){
    exibirLista(lista);
}

void excluiArquivo(LISTA* lista) {
    DATAFORNECIDA dataScaneada;
    printf("Digite a data (DD MM AAAA): ");
  
    scanf("%d %d %d", &dataScaneada.data.dia, &dataScaneada.data.mes, &dataScaneada.data.ano);

    if (buscarData(lista, dataScaneada.data.dia, dataScaneada.data.mes, dataScaneada.data.ano)) {
        char caminhoArquivoBuscado[MAX_TAMANHO + strlen(nome_diretorio) + 15];
        sprintf(caminhoArquivoBuscado, "%s/%d_%d_%d.txt", nome_diretorio, dataScaneada.data.dia, dataScaneada.data.mes, dataScaneada.data.ano);

        if (remove(caminhoArquivoBuscado) == 0) {
            removerElemento(lista, dataScaneada);
            printf("\nArquivo excluído com sucesso.\n");
        } else {
            printf("\nErro ao excluir o arquivo.\n");
        }
    } else {
        printf("\nNão há entrada para esta data.\n");
    }
}

// aqui termina o comando inicial
//
// aqui começa a implementacao da senha

void salvarSenha(const char *senha) {
    FILE *arquivo = fopen("senha.bin", "rb+");
    if (arquivo != NULL) {
        fwrite(senha, sizeof(char), strlen(senha) + 1, arquivo);
        fclose(arquivo);
        printf("Senha salva com sucesso!\n");
    } else {
        printf("Erro ao salvar a senha.\n");
    }
}

void lerSenha(char *senha) {
    FILE *arquivo = fopen("senha.bin", "rb+");
    if (arquivo != NULL) {
        fread(senha, sizeof(char), TAMANHO_MAX_SENHA, arquivo);
        fclose(arquivo);
    }
}

void atualizarSenha() {
    char senhaCorreta[TAMANHO_MAX_SENHA];
    char senhaAntiga[TAMANHO_MAX_SENHA];
    char novaSenha[TAMANHO_MAX_SENHA];

    // Lê a senha atual do arquivo
    lerSenha(senhaCorreta);

    // Solicita a senha antiga ao usuário
    printf("Digite a senha antiga: ");
    scanf("%s", senhaAntiga);

    // Verificar se a senha antiga está correta
    if (strcmp(senhaAntiga, senhaCorreta) == 0) {
        // Solicita a nova senha ao usuário
        printf("Digite a nova senha: ");
        scanf("%s", novaSenha);

        // Salva a nova senha no arquivo
        salvarSenha(novaSenha);

        printf("Senha atualizada com sucesso!\n");
    } else {
        printf("Senha antiga incorreta. Acesso negado.\n");
    }
}

// aqui termina a implementacao de senha

int main() {
    int choice;
    char *senha = (char *)malloc(50 * sizeof(char));

    printf("Digite sua senha: ");
    scanf("%s", senha);

    salvarSenha(senha);

    LISTA* lista;
    inicializarLista(lista);
    popularListaDeArquivo(lista);

    while (1) {
        printf("\nMenu:\n");
        printf("\n1. Adicionar entrada\n");
        printf("2. Editar entrada\n");
        printf("3. Ler entrada (seleciona por data)\n");
        printf("4. Listar entradas por data\n");
        printf("5. Excluir entrada (seleciona por data)\n");
        printf("6. Sair\n");
        printf("\nEscolha uma opção: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                adicionarEntrada(lista);
                break;
            case 2:
                editarEntrada(lista);
                break;
            case 3:
                LerEntradaPorData(lista);
                break;
            case 4:
                listarEntradasPorDatas(lista);
                break;
            case 5:
                excluiArquivo(lista);
                break;
            case 6:
                printf("Encerrando o programa...\n");
                salvarEmArquivo(lista);
                exit(0);
            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    }

    return 0;
}


