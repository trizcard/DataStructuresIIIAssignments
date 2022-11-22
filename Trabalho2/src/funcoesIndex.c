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