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
        strPoPs = realloc(strPoPs, (i+1)*sizeof(char));
        i++;
    } while(c != '|');
    int j = i;
    i++;
    while(c != '|' || c != '\0'){
        c = string[i];
        strPais[i] = c;
        strPais = realloc(strPais, (j-(i+1))*sizeof(char));
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
        registro *regAnt;
        regAnt = pag->fim;
        regAnt->prox = reg;
        pag->fim = reg;
        pag->tamanho++;
    }

    free(nomePais);
    free(nomePoPs);

    return 1;
}

void lerPagDisco(FILE *arq, pagDisco *pag, cabecalho *cab){
    if (arq == NULL){
        printf("Registro inexistente.");
        return;
    }

    while(arq != NULL){
        lerRegistro(arq, pag);
        cab->proxRRN++;
        if(pag->tamanho == 15){
            pagDisco *pagAnt;
            pagAnt = pag;
            pagAnt->prox = pag;
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

int lerArq(char nome[25], cabecalho *cab){
    FILE *arq = NULL;
    arq = fopen(nome, "rb");
    if (arq == NULL){
        printf("Falha no processamento do arquivo.\n");
        return 0;
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
    pag->tamanho = 1;
    pag->prox = NULL;

    lerPagDisco(arq, pag, cab);

    fclose(arq);
    return 1;
}

void imprimirSaida(cabecalho *cab){
    pagDisco *pagAux;

    pagAux = cab->prox;

    while(pagAux->prox != NULL){
        registro *regAux;
        regAux = pagAux->inicio;

        while(regAux->prox != NULL && strcpy(regAux->removido, "1")){
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

int analisarCampo(char nomeCampo[25]){
    if (!strcmp(nomeCampo, "idConecta")){
        return 1;
    } else if (!strcmp(nomeCampo, "siglaPais")){
        return 2;
    } else if (!strcmp(nomeCampo, "idPoPsConectado")){
        return 3;
    } else if (!strcmp(nomeCampo, "unidadeMedida")){
        return 4;
    } else if (!strcmp(nomeCampo, "velocidade")){
        return 5;
    } else if (!strcmp(nomeCampo, "nomePoPs")){
        return 6;
    } else if (!strcmp(nomeCampo, "nomePais")){
        return 7;
    } else {
        return 0;
    }
}

void filtrar(cabecalho *cab, pagDisco *pagNova, int tipo){
    int n;
    scanf("%d", &n);

    char nomeCampo[25];

    char valorCampo[25];

    for(int i = 0; i < n; i++){
        fscanf(stdin, "%s", nomeCampo);
        fscanf(stdin, "%s", valorCampo);
        scan_quote_string(valorCampo);

        int campo;
        campo = analisarCampo(nomeCampo);
        pagDisco *pagAux;
        pagAux = NULL;

        rodarPagina(cab, pagNova, pagAux, campo, valorCampo, tipo);
    }

}

void rodarPagina(cabecalho *cab, pagDisco *pag, pagDisco *pagNova, int campo, char valorCampo[25], int tipo){
    int nReg = 0;
    while (pag->prox != NULL){
        while (pag->inicio->prox != NULL){
            switch (campo){
                case 1:
                    if (pag->inicio->idConecta == atoi(valorCampo)){
                        passarReg(cab, pag, pagNova);
                    }
                    else if (pag->inicio->idConecta != atoi(valorCampo) && tipo == 4){
                        removeReg(cab, pag, pagNova, nReg);
                    }
                    break;
                case 2:
                    if (!strcmp(pag->inicio->siglaPais, valorCampo) && tipo == 3){
                        passarReg(cab, pag, pagNova);
                    } else if (strcmp(pag->inicio->siglaPais, valorCampo) && tipo == 4){
                        removeReg(cab, pag, pagNova, nReg);
                    }
                    break;
                case 3:
                    if (pag->inicio->idPoPsConec == atoi(valorCampo) && tipo == 3){
                        passarReg(cab, pag, pagNova);
                    } else if (pag->inicio->idPoPsConec != atoi(valorCampo) && tipo == 4){
                        removeReg(cab, pag, pagNova, nReg);
                    }
                    break;
                case 4:
                    if (!strcmp(pag->inicio->undMedida, valorCampo) && tipo == 3){
                        passarReg(cab, pag, pagNova);
                    } else if (strcmp(pag->inicio->undMedida, valorCampo) && tipo == 4){
                        removeReg(cab, pag, pagNova, nReg);                           
                    }
                    break;
                case 5:
                    if (pag->inicio->veloc == atoi(valorCampo) && tipo == 3){
                        passarReg(cab, pag, pagNova);
                    } else if (pag->inicio->veloc != atoi(valorCampo) && tipo == 4){
                        removeReg(cab, pag, pagNova, nReg);
                    }
                    break;
                case 6:
                    if (!strcmp(pag->inicio->nomePoPs, valorCampo) && tipo == 3){
                        passarReg(cab, pag, pagNova);
                    } else if (strcmp(pag->inicio->nomePoPs, valorCampo) && tipo == 4){
                        removeReg(cab, pag, pagNova, nReg);
                    }
                    break;
                case 7:
                    if (!strcmp(pag->inicio->nomePais, valorCampo) && tipo == 3){
                        passarReg(cab, pag, pagNova);
                    } else if (strcmp(pag->inicio->nomePais, valorCampo) && tipo == 4){
                        removeReg(cab, pag, pagNova, nReg);
                    }
                    break;
                
                default:
                    if(tipo == 3){
                        printf("Campo filtrado nao existe\n");
                    } else{
                        printf("Campo removido nao existe\n");
                    }
                    return;
            }
            pag->inicio = pag->inicio->prox;
            nReg++;
        }
        pag = pag->prox;
        cab->nPagDisco++;
    }
}

void passarReg(cabecalho *cab, pagDisco *pag, pagDisco *pagNova){
    if(pagNova->inicio == NULL){
        pagNova->inicio = pag->inicio;
        pagNova->fim = pag->inicio;
        pagNova->tamanho = 1;
    } else if (pagNova->tamanho == 15){
        pagDisco *pagAnt;
        pagAnt = pagNova;
        pagAnt->prox = pagNova;
        pagNova->inicio = NULL;
        pagNova->fim = NULL;

        cab->nPagDisco++;
        pagNova->inicio = pag->inicio;
        pagNova->fim = pag->inicio;
        pagNova->tamanho = 1;
    } else{
        pagNova->inicio->prox = pag->inicio;
        pagNova->fim = pag->inicio;
    }
}

void removeReg(cabecalho *cab, pagDisco *pag, pagDisco *pagNova, int reg){
    passarReg(cab, pag, pagNova);
    strcpy(pagNova->fim->removido, "1");
    int RRN = reg * 64 - 1;
    pagNova->fim->encadeamento = cab->topo;
    cab->topo = RRN;
    cab->nRegRemov++;
}