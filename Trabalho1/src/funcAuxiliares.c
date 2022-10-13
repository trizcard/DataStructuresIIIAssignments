#include "funcAuxiliares.h"
// 2 topologiaRede.bin

void addLixo(char *lixo, int inicio, int tam){
    for (int i = inicio; i < (inicio + tam); i++){
        lixo[i] = LIXO;
    }
}

void lerCampoVar(char *string, char *strPoPs, char *strPais){
    char c = 0;
    int i = 0;

    do{
        c = string[i];
        strPoPs[i] = c;
        i++;
    } while(string[i] != '|' && i < 44);
    strPoPs[i] = '\0';
    i++;
    int j = i;
    
    c = string[i];
    while(c != '|' && i < 44){
        strPais[(i-j)] = c;
        i++;
        c = string[i];
    }
    strPais[i-j] = '\0';
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
    fread((reg->siglaPais), sizeof(char), 2, arq);
    reg->siglaPais[2] = '\0';
    fread(&reg->idPoPsConec, sizeof(int), 1, arq);
    fread((reg->undMedida), sizeof(char), 1, arq);
    reg->undMedida[1] = '\0';
    fread(&reg->veloc, sizeof(int), 1, arq);

    char string[45];
    fread(string, sizeof(char), 44, arq);
    
    lerCampoVar(string, reg->nomePoPs, reg->nomePais);

    return 1;
}

void imprimeRegistro(registro *regAux){
    printf("Identificador do ponto: %d\n", regAux->idConecta);
    printf("Nome do ponto: %s\n", regAux->nomePoPs);
    printf("Nome do pais: %s\n", regAux->nomePais);
    printf("Sigla do pais: %s\n", regAux->siglaPais);
    printf("Identificador do ponto conectado: %d\n", regAux->idPoPsConec);
    printf("Velocidade de transmissao: %d %sbps\n\n", regAux->veloc, regAux->undMedida);
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
    regAux->nomePoPs = malloc(44*sizeof(char));
    regAux->nomePais = malloc(44*sizeof(char));
    cabecalho *cabAux;
    cabAux = (cabecalho*) malloc(sizeof(cabecalho));

    lerCabecalho(arq, cabAux);
    printf("%d", cabAux->proxRRN);
    int i = 0;
    // percorre todo o arquivo
    while(i < cabAux->proxRRN){
        // se registro removido retorna 0
        if (lerRegistro(arq, regAux)){
            imprimeRegistro(regAux);
        }
        i++;
    }
    printf("Numero de paginas de disco: %d\n\n", cabAux->nPagDisco);
    free(regAux->nomePoPs);
    free(regAux->nomePais);
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
