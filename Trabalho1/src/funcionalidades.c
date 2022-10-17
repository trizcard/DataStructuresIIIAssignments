#include "funcionalidades.h"

void funcUM(char nomeArq[25]){
}

void funcDOIS(char nomeArq[25]){
    // abre arquivo e verifica se funcionou de acordo com a funcao
    FILE *arq = NULL;
    arq = fopen(nomeArq, "rb");

    if (arq == NULL){
        printf("Falha no processamento do arquivo.\n");
        return;
    }
    
    imprimirSaida(arq);
    fclose(arq);
}

void funcTRES(char nomeArq[25]){
    // abre arquivo e verifica se funcionou de acordo com a funcao
    FILE *arq = NULL;
    arq = fopen(nomeArq, "rb");

    if (arq == NULL){
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    filtrar(arq, 3);
    fclose(arq);
}

void funcQUATRO(char nomeArq[25]){
    // abre arquivo e verifica se funcionou de acordo com a funcao
    FILE *arq = NULL;
    arq = fopen(nomeArq, "rb+");

    if (arq == NULL){
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    filtrar(arq, 4);
    fclose(arq);
    binarioNaTela(nomeArq);
}

void funcCINCO(char nomeArq[25]){}

void funcSEIS(char nomeArq[25]){}