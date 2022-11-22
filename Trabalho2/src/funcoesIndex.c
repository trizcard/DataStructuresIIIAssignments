#include "funcoesIndex.h"


int buscarArvore (pagArvore *raiz, int dado){
    pagArvore *noAtual;
    noAtual = raiz;
    int posicao = 0;

    if (noAtual == NULL){
        return -1; // arvore vazia
    }

    while (noAtual != NULL){
        posicao = buscaBin(noAtual, dado);
        if (posicao < noAtual->nroChavesNo && dado == noAtual->C[posicao]){
            return 1; // achou
        }
        else{
            noAtual = noAtual->P[posicao];
        }
    }
    return 0; // nao achou    
}

int buscaBin (pagArvore *noAtual, int dado){
    int inicio = 0;
    int fim = noAtual->nroChavesNo - 1;
    int meio;

    while (inicio <= fim){
        meio = (inicio + fim) / 2;
        if (dado == noAtual->C[meio]){
            return meio;
        }
        else if (dado < noAtual->C[meio]){
            fim = meio - 1;
        }
        else{
            inicio = meio + 1;
        }
    }
    return inicio;
}