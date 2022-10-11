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
    } while(c != '|' || i < 44);
    int j = i;
    i++;
    while(c != '|' || i < 44){
        c = string[i];
        strPais[i] = c;
        strPais = realloc(strPais, (j-(i+1))*sizeof(char));
        i++;
    }

    if (i < 44){
        int tam = 44 - i;
        addLixo(strPais, i, tam);
        return 0; // retorna 0 se não houver truncamento
    } else {
        strcpy(string, "|");
        strPais[(i)] = string[0];
        return 1;
    }
}

void lerCabecalho(FILE *arq, cabecalho *cab){
    fread(cab->status, sizeof(char), 1, arq);
    fread(&cab->topo, sizeof(int), 1, arq);
    fread(&cab->proxRRN, sizeof(int), 1, arq);
    fread(&cab->nRegRemov, sizeof(int), 1, arq);
    fread(&cab->nPagDisco, sizeof(int), 1, arq);
    fread(&cab->qtdCompact, sizeof(int), 1, arq);
    fread(cab->lixo, sizeof(char), 939, arq);
}

int lerRegistro(FILE *arq, registro *reg){
    fread(reg->removido, sizeof(char), 1, arq);

    if (!strcmp(reg->removido, "1")){
        fseek(arq, 64, SEEK_CUR);
        return 0;
    }

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

    free(nomePais);
    free(nomePoPs);

    return 1;
}

void imprimeRegistro(registro *regAux){
    printf("Identificador do ponto: %d\n", regAux->idConecta);
    printf("Nome do ponto: %s\n", regAux->nomePoPs);
    printf("Nome do pais: %s\n", regAux->nomePais);
    printf("Sigla do pais: %s\n", regAux->siglaPais);
    printf("Identificador do ponto conectado: %d\n", regAux->idPoPsConec);
    printf("Velocidade de transmissao: %d Mbps\n\n", regAux->veloc);
}

void removerRegistro(FILE *arq, registro *reg, cabecalho *cab){
    int RRNatual = SEEK_CUR;
    fseek(arq, -64, SEEK_CUR);
    fwrite("1", sizeof(char), 1, arq);
    fwrite(&cab->topo, sizeof(int), 1, arq);
    cab->topo = (RRNatual-64);
    cab->nRegRemov++;
    fseek(arq, 64, SEEK_CUR);
}

void imprimirSaida(FILE *arq){
    // cria um registro auxiliar e cabecalho auxiliar
    registro *regAux;
    regAux = (registro*) malloc(sizeof(registro));
    cabecalho *cabAux;
    cabAux = (cabecalho*) malloc(sizeof(cabecalho));

    lerCabecalho(arq, cabAux);

    // percorre todo o arquivo
    while(arq != NULL){
        // se registro removido retorna 0
        if (lerRegistro(arq, regAux)){
            imprimeRegistro(regAux);
            regAux = regAux->prox;
        }
    }
    printf("Numero de paginas de disco: %d\n\n", cabAux->nPagDisco);
    free(regAux);
    free(cabAux);
}

int analisarCampo(filtro *filtros, int i, registro *reg){
    if (!strcmp(filtros[i].nomeCampo, "idConecta")){
        int idConecta = atoi(filtros[i].valorCampo);
        if (idConecta == reg->idConecta){
            return 1;
        }
        return 0;
    } else if (!strcmp(filtros[i].nomeCampo, "siglaPais")){
        if (!strcmp(filtros[i].valorCampo, reg->siglaPais)){
            return 1;
        }
        return 0;
    } else if (!strcmp(filtros[i].nomeCampo, "idPoPsConectado")){
        int idPoPsConectado = atoi(filtros[i].valorCampo);
        if (idPoPsConectado == reg->idPoPsConec){
            return 1;
        }
        return 0;
    } else if (!strcmp(filtros[i].nomeCampo, "unidadeMedida")){
        if (!strcmp(filtros[i].valorCampo, reg->undMedida)){
            return 1;
        }
        return 0;
    } else if (!strcmp(filtros[i].nomeCampo, "velocidade")){
        int velocidade = atoi(filtros[i].valorCampo);
        if (velocidade == reg->veloc){
            return 1;
        }
        return 0;
    } else if (!strcmp(filtros[i].nomeCampo, "nomePoPs")){
        if (!strcmp(filtros[i].valorCampo, reg->nomePoPs)){
            return 1;
        }
        return 0;
    } else if (!strcmp(filtros[i].nomeCampo, "nomePais")){
        if (!strcmp(filtros[i].valorCampo, reg->nomePais)){
            return 1;
        }
        return 0;
    } else {
        return 0;
    }
}

void filtrar(FILE *arq, int tipo){ // tipo 3 = imprime, tipo 4 = remove
    int n;
    scanf("%d", &n);
    int filtrados = 1; // se for 1, o registro passou pelos filtros

    filtro filtros[n];

    for(int i = 0; i < n; i++){
        fscanf(stdin, "%s", filtros[i].nomeCampo);
        fscanf(stdin, "%s", filtros[i].valorCampo);
        scan_quote_string(filtros[i].valorCampo);
    }

    // cria um registro auxiliar
    registro *regAux;
    regAux = (registro*) malloc(sizeof(registro));

    // cria e le cabecalho
    cabecalho *cab;
    cab = (cabecalho*) malloc(sizeof(cabecalho));
    lerCabecalho(arq, cab);

    // percorre todo o arquivo
    while(arq != NULL){
        // se registro removido retorna 0
        if (lerRegistro(arq, regAux)){
            for (int i = 0; i < n; i++){
                filtrados = (analisarCampo(filtros, i, regAux)) * filtrados;
            }
            if (tipo == 3 && filtrados){
                imprimeRegistro(regAux);
            } else if (tipo == 4 && filtrados){
                removerRegistro(arq, regAux, cab);
            }
            regAux = regAux->prox;
        }
    }

    if (tipo == 3){
        printf("Numero de paginas de disco: %d\n\n", cab->nPagDisco);
    } else if (tipo == 4){
        // atualiza cabecalho
        fseek(arq, 1, SEEK_SET);
        fwrite(&cab->topo, sizeof(int), 1, arq);
        fseek(arq, 9, SEEK_SET);
        fwrite(&cab->nRegRemov, sizeof(int), 1, arq);
    }

    free(regAux);
    free(cab);
}
