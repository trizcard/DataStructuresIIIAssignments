#include "funcoesIndex.h"

void lerPgDados(FILE *arqDados, no *arvore){
    // Lê uma página de dados da árvore-B
    fread(&arvore->folha, sizeof(char), 1, arqDados);
    fread(&arvore->nroChavesNo, sizeof(int), 1, arqDados);
    fread(&arvore->alturaNo, sizeof(int), 1, arqDados);
    fread(&arvore->RRNdoNo, sizeof(int), 1, arqDados);
    for(int i = 0; i < (MAX_CHAVE); i++){
        fread(&arvore->P[i], sizeof(int), 1, arqDados);
        fread(&arvore->CP[i].c, sizeof(int), 1, arqDados);
        fread(&arvore->CP[i].Pr, sizeof(int), 1, arqDados);
    }
    fread(&arvore->P[(MAX_CHAVE)], sizeof(int), 1, arqDados);
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
    int pagAcessadas = 2; // inicia com 2, pois conta a pagina do cabeçalho dos dois arquivos
    
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
// arq -> arquivo com a arvore B
// chave -> chave a ser inserida
// RRNchave -> RRN do registro que contem a chave
// RRNarv -> RRN da pagina atual da arvore 

void inicializarArv(no *pagArv){
    (*pagArv).folha = '1';
    (*pagArv).nroChavesNo = 0;
    (*pagArv).alturaNo = 0;
    (*pagArv).RRNdoNo = 0;

    for(int i = 0; i < MAX_CHAVE; i++){
        (*pagArv).P[i] = -1;
        (*pagArv).CP[i].c = -1;
        (*pagArv).CP[i].Pr = -1;
    }
    (*pagArv).P[MAX_CHAVE] = -1;
}

int buscarPagina(no pagAtual, int chave, int *PosiAchada){
    for (int i = 0; i < pagAtual.nroChavesNo && chave < pagAtual.CP[i].c; i++){
        *PosiAchada = i;
        if (chave == pagAtual.CP[i].c && i < pagAtual.nroChavesNo){
            return 1;
        }
    }
    *PosiAchada = pagAtual.nroChavesNo;
    return 0;
}

int inserirArvore(FILE *arq, int chave, int RRNchave, int RRNarv, promovidos *Promovido){
    no pagAtual, pagProx;

    int promovido; // 1 se houve promocao, 0 se nao houve

    // Chave e RRN da arvoreB a serem promovidos
    int posicao = 0;
    promovidos PromB;

    if(RRNarv == -1){
        Promovido->chave = chave;
        Promovido->RRN = RRNchave;
        Promovido->filho = -1;
        return 1;
    }
    fseek(arq, ((RRNarv+1) * TAMANHO_REG_DADOS), SEEK_SET);
    lerPgDados(arq, &pagAtual);

    if (buscarPagina(pagAtual, chave, &posicao) != 0){
        printf("Chave ja existente.\n");   
        return -1;
    }

    promovido = inserirArvore(arq, chave, RRNchave, pagAtual.P[posicao], &PromB);

    if (promovido == 0){
        return 0; // sem promoção
    }
    if (pagAtual.nroChavesNo < MAX_CHAVE){
        inserirNo(&pagAtual, PromB);
        alterarNo(arq, &pagAtual, RRNarv);
        return 0; // sem promoção
    }
    else {
        split(PromB, &pagAtual, Promovido, &pagProx);
        alterarNo(arq, &pagAtual, RRNarv);
        alterarNo(arq, &pagProx, Promovido->filho);
        return 1; // com promoção
    }
}

void split(promovidos PromB, no *pagAntiga, promovidos *Promovido, no *pagNova){
    int i;
    int chavesPresentes[MAX_CHAVE+1];
    int RRNsPresentes[MAX_CHAVE+1];
    int filhosPresentes[MAX_CHAVE+2];

    for (i = 0; i < MAX_CHAVE; i++){
        chavesPresentes[i] = pagAntiga->CP[i].c;
        RRNsPresentes[i] = pagAntiga->CP[i].Pr;
        filhosPresentes[i] = pagAntiga->P[i];
    }

    filhosPresentes[i] = pagAntiga->P[i];

    for(i = MAX_CHAVE; PromB.chave < chavesPresentes[i-1] && i > 0; i--){
        chavesPresentes[i] = chavesPresentes[i-1];
        RRNsPresentes[i] = RRNsPresentes[i-1];
        filhosPresentes[i+1] = filhosPresentes[i];
    }
    chavesPresentes[i] = PromB.chave;
    RRNsPresentes[i] = PromB.RRN;
    filhosPresentes[i+1] = PromB.filho;

    inicializarArv(pagNova);
    Promovido->filho = pagNova->RRNdoNo;

    for(i = 0; i < MIN_CHAVE; i++){
        pagAntiga->CP[i].c = chavesPresentes[i];
        pagAntiga->CP[i].Pr = RRNsPresentes[i];
        pagAntiga->P[i] = filhosPresentes[i];

        pagNova->CP[i].c = chavesPresentes[i+MIN_CHAVE+1];
        pagNova->CP[i].Pr = RRNsPresentes[i+MIN_CHAVE+1];
        pagNova->P[i] = filhosPresentes[i+MIN_CHAVE+1];

        pagAntiga->CP[i + MIN_CHAVE].c = -1;
        pagAntiga->CP[i + MIN_CHAVE].Pr = -1;
        pagAntiga->P[i + MIN_CHAVE + 1] = -1;
    }

    pagAntiga->P[MIN_CHAVE] = filhosPresentes[MIN_CHAVE];
    pagNova->P[MIN_CHAVE] = filhosPresentes[i + MIN_CHAVE + 1];

    pagAntiga->nroChavesNo = MIN_CHAVE;
    pagNova->nroChavesNo = MAX_CHAVE - MIN_CHAVE;
    
    Promovido->chave = chavesPresentes[MIN_CHAVE];
    Promovido->RRN = RRNsPresentes[MIN_CHAVE];
}

void inserirNo(no *pagAtual, promovidos PromB){
    int i = pagAtual->nroChavesNo;
    while (i > 0 && PromB.chave < pagAtual->CP[i-1].c){
        pagAtual->CP[i].c = pagAtual->CP[i-1].c;
        pagAtual->CP[i].Pr = pagAtual->CP[i-1].Pr;
        pagAtual->P[i+1] = pagAtual->P[i];
        i--;
    }
    pagAtual->CP[i].c = PromB.chave;
    pagAtual->CP[i].Pr = PromB.RRN;
    pagAtual->P[i+1] = PromB.filho;
    pagAtual->nroChavesNo++;
}

void alterarNo(FILE *arq, no *pagAtual, int RRNarv){
    fseek(arq, ((RRNarv+1) * TAMANHO_REG_DADOS), SEEK_SET);
    fwrite(&pagAtual->folha, sizeof(char), 1, arq);
    fwrite(&pagAtual->nroChavesNo, sizeof(int), 1, arq);
    fwrite(&pagAtual->alturaNo, sizeof(int), 1, arq);
    fwrite(&pagAtual->RRNdoNo, sizeof(int), 1, arq);
    for (int i = 0; i < MAX_CHAVE; i++){
        fwrite(&pagAtual->CP[i].c, sizeof(int), 1, arq);
        fwrite(&pagAtual->CP[i].Pr, sizeof(int), 1, arq);
        fwrite(&pagAtual->P[i], sizeof(int), 1, arq);
    }
    fwrite(&pagAtual->P[pagAtual->nroChavesNo], sizeof(int), 1, arq);
}
