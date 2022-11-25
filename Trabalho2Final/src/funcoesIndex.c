#include "funcoesIndex.h"

void lerPgDados(FILE *arqDados, no *arvore){
    // Lê uma página de dados da árvore-B
    fread(&arvore->folha, sizeof(char), 1, arqDados);
    fread(&arvore->nroChavesNo, sizeof(int), 1, arqDados);
    fread(&arvore->alturaNo, sizeof(int), 1, arqDados);
    fread(&arvore->RRNdoNo, sizeof(int), 1, arqDados);
    for(int i = 0; i < (arvore->nroChavesNo); i++){
        fread(&arvore->P[i], sizeof(int), 1, arqDados);
        fread(&arvore->CP[i].c, sizeof(int), 1, arqDados);
        fread(&arvore->CP[i].Pr, sizeof(int), 1, arqDados);
    }
    fread(&arvore->P[(arvore->nroChavesNo)], sizeof(int), 1, arqDados);
}

void lerCabecalhoArv(FILE *arq, cabecalhoArv *cab){
    // Lê o cabeçalho da árvore-B
    fread(&cab->status, sizeof(char), 1, arq);
    fread(&cab->noRaiz, sizeof(int), 1, arq);
    fread(&cab->nroChavesTotal, sizeof(int), 1, arq);
    fread(&cab->alturaArvore, sizeof(int), 1, arq);
    fread(&cab->RRNproxNo, sizeof(int), 1, arq);
    fread((cab->lixo), sizeof(char), 48, arq);
    cab->lixo[48] = '\0';
}

int buscarArvore(FILE *arq, int RRNarv, int chave, int *RRNachado, int *PosiAchada, int pagAcessadas){
    fseek(arq, ((RRNarv+1) * TAMANHO_REG_DADOS), SEEK_SET);
    no pagAux;
    lerPgDados(arq, &pagAux);
    pagAcessadas++;
    
    for (int i = 0; i < pagAux.nroChavesNo; i++){
        if (chave == pagAux.CP[i].c){
            pagAcessadas++;
            *RRNachado = RRNarv;
            *PosiAchada = i;
            return pagAcessadas;
        }
        else if (chave < pagAux.CP[i].c){
            if (pagAux.folha == '0'){
                return buscarArvore(arq, pagAux.P[i], chave, RRNachado, PosiAchada, pagAcessadas);
            }
            else {
                *RRNachado = RRNarv;
                *PosiAchada = i-1;
                return 0;
            }
        }
    }
    if (pagAux.folha == '0'){
        return buscarArvore(arq, pagAux.P[pagAux.nroChavesNo], chave, RRNachado, PosiAchada, pagAcessadas);
    }
    else {
        *RRNachado = RRNarv;
        *PosiAchada = pagAux.nroChavesNo;
        return 0;
    }
}

int filtrarChave(FILE *arq, FILE *arqDados, char *valorCampo){
    int RRNachado = 0;
    int PosiAchada = 0;
    int pagAcessadas = 1; // inicia com 1, pois conta a pagina do cabeçalho
    
    cabecalhoArv cabArv;
    cabArv.lixo = (char*)malloc(49*sizeof(char));
    lerCabecalhoArv(arqDados, &cabArv);
    
    pagAcessadas = buscarArvore(arqDados, cabArv.noRaiz, atoi(valorCampo), &RRNachado, &PosiAchada, pagAcessadas);
    if (pagAcessadas != 0){
        no pagAux;
        fseek(arqDados, ((RRNachado+1) * TAMANHO_REG_DADOS), SEEK_SET);
        lerPgDados(arqDados, &pagAux);

        // imprime registro
        fseek(arq, ((pagAux.CP[PosiAchada].Pr) * TAMANHO_REG) + 960, SEEK_SET);
        registro reg;
        alocaRegistro(&reg);
        pagAcessadas++; // acesso a pagina do registro
        lerRegistro(arq, &reg);
        imprimeRegistro(&reg);
        desalocarRegistro(&reg);
    }
    else {
        printf("Registro inexistente.\n");
    }
    free(cabArv.lixo);
    return pagAcessadas;
}

// Função para inserir uma chave numa arvore b
void inserirArvore(FILE *arq, int RRNatual, int chave, int RRNchave){
    no pagAux;

    lerPgDados(arq, &pagAux);
    for (int i = 0; i < pagAux.nroChavesNo; i++){
        if (chave == pagAux.CP[i].c){
            pagAux.CP[i].Pr = RRNchave;
            return;
        }
        else if (chave < pagAux.CP[i].c){
            if (pagAux.folha == '0'){
                inserirArvore(arq, pagAux.P[i], chave, RRNchave);
            }
            else {
                return;
            }
        }
    }
    if (pagAux.folha == '0'){
        inserirArvore(arq, pagAux.P[pagAux.nroChavesNo-1], chave, RRNchave);
    }
    else {
        pagAux.CP[pagAux.nroChavesNo-1].Pr = RRNchave;
        return;
    }
}