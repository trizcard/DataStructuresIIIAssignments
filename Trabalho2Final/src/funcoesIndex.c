#include "funcoesIndex.h"

void lerPgDados(FILE *arqDados, pagArvore *arvore){
    // Lê uma página de dados da árvore-B
    fread(&arvore->folha, sizeof(char), 1, arqDados);
    fread(&arvore->nroChavesNo, sizeof(int), 1, arqDados);
    fread(&arvore->alturaNo, sizeof(int), 1, arqDados);
    fread(&arvore->RRNdoNo, sizeof(int), 1, arqDados);
    for(int i = 0; i < ORDEM; i++){
        fread(&arvore->P[i], sizeof(int), 1, arqDados);
    }
    for(int i = 0; i < ORDEM-1; i++){
        fread(&arvore->C[i], sizeof(int), 1, arqDados);
    }
    for(int i = 0; i < ORDEM-1; i++){
        fread(&arvore->Pr[i], sizeof(int), 1, arqDados);
    }
}

int buscarArvore(FILE *arq, int RRNarv, int chave, int *RRNachado, int *PosiAchada){
    fseek(arq, (RRNarv * TAMANHO_REG_DADOS), SEEK_SET);

    pagArvore pagAux;
    lerPgDados(arq, &pagAux);
    for (int i = 0; i < pagAux.nroChavesNo; i++){
        if (chave == pagAux.C[i]){
            RRNachado = pagAux.Pr[i];
            PosiAchada = i;
            return 1;
        }
        else if (chave < pagAux.C[i]){
            if (pagAux.folha == '0'){
                return buscarArvore(arq, pagAux.P[i], chave, &RRNachado, &PosiAchada);
            }
            else {
                return 0;
            }
        }
    }
    if (pagAux.folha == '0'){
        return buscarArvore(arq, pagAux.P[pagAux.nroChavesNo-1], chave, &RRNachado, &PosiAchada);
    }
    else {
        RRNachado = pagAux.Pr[pagAux.nroChavesNo-1];
        PosiAchada = pagAux.nroChavesNo-1;
        return 0;
    }
}
