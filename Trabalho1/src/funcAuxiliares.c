#include "funcAuxiliares.h"

void addLixo(char *lixo, int inicio, int tam){
    for (int i = inicio; i < (inicio + tam); i++){
        lixo[i] = LIXO;
    }
}

int lerCampoVar(char* string, char *strPoPs, char *strPais){
    char c = 0;
    int i = 0;

    do{
        c = string[i];
        strPoPs[i] = c;
        realloc(strPoPs, (i+1)*sizeof(char));
        i++;
    } while(c != '|');
    int j = i;
    i++;
    while(c != '|' || c != '\0'){
        c = string[i];
        strPais[i] = c;
        realloc(strPais, (j-(i+1))*sizeof(char));
        i++;
    }

    if (c == '\0'){
        int tam = 44 - i;
        addLixo(strPais, i, tam);
        return 0; // retorna 0 se não houver truncamento
    } else {
        strcpy(string, "|");
        strPais[(i)] = string[0];
        return 1;
    }
}

int lerRegistro(FILE *arq, pagDisco *pag){
    char removido[1];
    fread(removido, sizeof(char), 1, arq);

    if (!strcmp(removido, "*")){
        fseek(arq, 64, SEEK_CUR);
        return 0;
    }
    registro reg[1];
    reg->removido[0] = removido[0];

    fread(&reg->encadeamento, sizeof(int), 1, arq);
    fread(&reg->idConecta, sizeof(int), 1, arq);
    fread(reg->siglaPais, sizeof(char), 2, arq);
    fread(&reg->idPoPsConec, sizeof(int), 1, arq);
    fread(reg->undMedida, sizeof(char), 1, arq);
    fread(&reg->veloc, sizeof(int), 1, arq);

    char string[44];
    char *nomePoPs;
    nomePoPs = (char*) malloc(1*sizeof(char));
    char *nomePais;
    nomePais = (char*) malloc(1*sizeof(char));
    fread(string, sizeof(char), 44, arq);
    lerCampoVar(string, nomePoPs, nomePais);

    strcpy(reg->nomePoPs, nomePoPs);
    strcpy(reg->nomePais, nomePais);

    if(pag->inicio == NULL){
        pag->inicio = reg;
        pag->fim = reg;
        pag->tamanho = 1;
    }else{
        pagDisco *pagAnt;
        pagAnt = pag->fim;
        pagAnt->prox = reg;
        pag->fim = reg;
        pag->tamanho++;
    }

    free(nomePais);
    free(nomePoPs);

    return 1;
}

void lerPagDisco(FILE *arq, pagDisco *pag, cabecalho *cab){

    while(arq != NULL){
        lerRegistro(arq, pag);
        cab->proxRRN++;
        if(pag->tamanho == 15){
            pag->prox = (pagDisco*) malloc(sizeof(pagDisco));
            pag = pag->prox;
            pag->inicio = NULL;
            pag->fim = NULL;
            pag->tamanho = 0;
            pag->prox = NULL;
            cab->nPagDisco++;
        }
    }
    cab->status[0] = '1';
    cab->topo = -1;
}

void lerArq(char nome[25], cabecalho *cab){
    FILE *arq;
    arq = fopen(nome, "rb");
    if (arq == NULL){
        printf("Falha no carregamento do arquivo.");
        return;
    }

    strcpy(cab->status, "0");
    cab->topo = -1;
    cab->proxRRN = 0;
    cab->nRegRemov = 0;
    cab->nPagDisco = 0;
    cab->qtdCompact = 0;
    addLixo(cab->lixo, 0, 939);
    
    pagDisco pag[1];

    cab->prox = pag;
    pag->inicio = cab;
    pag->fim = cab;
    pag->tamanho = 1;
    pag->prox = NULL;

    lerPagDisco(arq, pag, cab);

    fclose(arq);
}

void imprimirSaida(cabecalho *cab){
    pagDisco *pagAux;

    pagAux = cab->prox;

    while(pagAux->prox != NULL){
        registro *regAux;
        regAux = pagAux->inicio;

        while(regAux->prox != NULL){
            printf("Identificador do ponto: %d\n", regAux->idConecta);
            printf("Nome do ponto: %s\n", regAux->nomePoPs);
            printf("Nome do pais: %s\n", regAux->nomePais);
            printf("Sigla do pais: %s\n", regAux->siglaPais);
            printf("Identificador do ponto conectado: %d\n", regAux->idPoPsConec);
            printf("Velocidade de transmissao: %d Mbps\n\n", regAux->veloc);
            regAux = regAux->prox;
        }
        pagAux = pagAux->prox;
    }
    printf("Numero de paginas de disco: %d\n\n", cab->nPagDisco);
}
