#include "funcoesDados.h"

// Função que lê o cabeçalho do arquivo de entrada
void lerCabecalho(FILE *arq, cabecalho *cab){
    fread(&cab->status, sizeof(char), 1, arq);
    fread(&cab->topo, sizeof(int), 1, arq);
    fread(&cab->proxRRN, sizeof(int), 1, arq);
    fread(&cab->nRegRemov, sizeof(int), 1, arq);
    fread(&cab->nPagDisco, sizeof(int), 1, arq);
    fread(&cab->qtdCompact, sizeof(int), 1, arq);
    fread(cab->lixo, sizeof(char), 939, arq);
}

// Função que aloca registro na memória
void alocaRegistro(registro *reg){
    reg->nomePais = (char *) malloc(45 * sizeof(char));
    reg->nomePoPs = (char *) malloc(45 * sizeof(char));
    reg->siglaPais = (char *) malloc(3 * sizeof(char));
    reg->undMedida = (char *) malloc(2 * sizeof(char));
}

//Função que desaloca o registro
void desalocarRegistro(registro *reg){
    free(reg->nomePais);
    free(reg->nomePoPs);
    free(reg->siglaPais);
    free(reg->undMedida);
}

// Função que lê o registro do arquivo de entrada
int lerRegistro(FILE *arq, registro *reg){
    fread(&reg->removido, sizeof(char), 1, arq);
    if (reg->removido == '1'){
        fseek(arq, 63, SEEK_CUR);
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

// Passa as informações do registro origem para o registro destino
void passarReg(registro *reg, registro *regOrig){
    reg->idConecta = regOrig->idConecta;

    reg->siglaPais = (char*) malloc(3 * sizeof(char));
    reg->siglaPais[0] = regOrig->siglaPais[0];
    reg->siglaPais[1] = regOrig->siglaPais[1];
    reg->siglaPais[2] = '\0';

    reg->idPoPsConec = regOrig->idPoPsConec;
    
    reg->undMedida = (char*) malloc(2 * sizeof(char));
    reg->undMedida[0] = regOrig->undMedida[0];
    reg->undMedida[1] = '\0';

    reg->veloc = regOrig->veloc;

    reg->nomePais = (char*) malloc(45 * sizeof(char));
    strcpy(reg->nomePais, regOrig->nomePais);
    reg->nomePoPs = (char*) malloc(45 * sizeof(char));
    strcpy(reg->nomePoPs, regOrig->nomePoPs);
}

// Implementar lista dinamica encadeada de registros
void adicionarListaReg(Lista *lista, registro *regOrig){
    elemento *novo;
    novo = (elemento*) malloc(sizeof(elemento));
    novo->reg = (registro*) malloc(sizeof(registro));
    passarReg(novo->reg, regOrig);
    novo->prox = NULL;

    if ((*lista) == NULL){
        *lista = novo;
    } else {
        elemento *aux;
        aux = *lista;
        while(aux->prox != NULL){
            aux = aux->prox;
        }
        aux->prox = novo;
    }
}

// Funcao que busca registro do index na lista
int buscaLista(Lista *lista, registro *reg, int index){
    elemento *aux;
    aux = *lista;
    int i = 0;
    while(aux != NULL){
        if(i == index){
            passarReg(reg, aux->reg);
            return 1;
        }
        aux = aux->prox;
        i++;
    }
    return 0;
}

// Função qu lê o campo variável do registro
void lerCampoVar(char *string, char *strPoPs, char *strPais){
    char c = 0;
    int i = 0;
    if (string[0] != '|'){
        do{
            c = string[i];
            strPoPs[i] = c;
            i++;
        } while(string[i] != '|' && i < 44);
    }
    strPoPs[i] = '\0';
    i++;
    int j = i;
    
    if (i < 44){
        c = string[i];
        while(c != '|' && i < 44){
            strPais[(i-j)] = c;
            i++;
            c = string[i];
        }
    }
    strPais[i-j] = '\0';
}

// Função que le os filtros de um arquivo de entrada
void lerFiltros(filtro *filtros, int n){
    for(int i = 0; i < n; i++){
        fscanf(stdin, "%s", filtros[i].nomeCampo);
        scan_quote_string(filtros[i].valorCampo);
    }
}

// Função que le o arquivo de entrada e cria uma lista com todos os registros
void lerArquivo(FILE *arq, cabecalho *cab, Lista *lista){
    fseek(arq, 959, SEEK_SET);

    // cria um registro auxiliar
    registro regAux;
    alocaRegistro(&regAux);

    // escreve status 0 enquanto arquivo é lido
    fseek(arq, 0, SEEK_SET);
    char status = '0';
    fwrite(&status, sizeof(char), 1, arq);
    fseek(arq, 960, SEEK_SET);
    
    int rrn = 0;
    // percorre todo o arquivo
    while(rrn < cab->proxRRN){
        // se registro removido retorna 0
        lerRegistro(arq, &regAux);
        adicionarListaReg(lista, &regAux);
        rrn++;
    }

    // escreve status 1 no arquivo
    fseek(arq, 0, SEEK_SET);
    status = '1';
    fwrite(&status, sizeof(char), 1, arq);
    desalocarRegistro(&regAux);
}

// Função que printa o registro
void imprimeRegistro(registro *regAux){
    if (regAux->idConecta != 0){
        printf("Identificador do ponto: %d\n", regAux->idConecta);
    }
    if (regAux->nomePoPs[0] != '\0'){
        printf("Nome do ponto: %s\n", regAux->nomePoPs);
    }
    if (regAux->nomePais[0] != '\0'){
        printf("Pais de localizacao: %s\n", regAux->nomePais);
    }
    if (regAux->siglaPais[0] != '$'){
        printf("Sigla do pais: %s\n", regAux->siglaPais);
    }
    if (regAux->idPoPsConec != -1){
        printf("Identificador do ponto conectado: %d\n", regAux->idPoPsConec);
    }
    if (regAux->undMedida[0] != '$'){
        printf("Velocidade de transmissao: %d %sbps\n", regAux->veloc, regAux->undMedida);
    }
    printf("\n");
}

// Função que analisa os campos de um registros de acordo com o filtro
int analisarCampo(filtro filtros, registro *reg){
    if (!strcmp(filtros.nomeCampo, "idConecta")){
        int idConecta;
        idConecta = atoi(filtros.valorCampo);
        if (idConecta == reg->idConecta){
            return 1;
        }
        return 0;
    } else if (!strcmp(filtros.nomeCampo, "siglaPais")){
        if (!strcmp(filtros.valorCampo, reg->siglaPais)){
            return 1;
        }
        return 0;
    } else if (!strcmp(filtros.nomeCampo, "idPoPsConectado")){
        int idPoPsConectado;
        idPoPsConectado = atoi(filtros.valorCampo);
        if (idPoPsConectado == reg->idPoPsConec){
            return 1;
        }
        return 0;
    } else if (!strcmp(filtros.nomeCampo, "unidadeMedida")){
        if (!strcmp(filtros.valorCampo, reg->undMedida)){
            return 1;
        }
        return 0;
    } else if (!strcmp(filtros.nomeCampo, "velocidade")){
        int velocidade;
        velocidade = atoi(filtros.valorCampo);
        if (velocidade == reg->veloc){
            return 1;
        }
        return 0;
    } else if (!strcmp(filtros.nomeCampo, "nomePoPs")){
        if (!strcmp(filtros.valorCampo, reg->nomePoPs)){
            return 1;
        }
        return 0;
    } else if (!strcmp(filtros.nomeCampo, "nomePais")){
        if (!strcmp(filtros.valorCampo, reg->nomePais)){
            return 1;
        }
        return 0;
    } else {
        return 0;
    }
}

// Função que filtra os registros de acordo com as entradas
int filtrar(cabecalho *cab, Lista *listaCompleta, filtro filtro){ 
    int regRRN = 0;
    int flag = 0;

    // cria um registro auxiliar
    registro regAux;
    alocaRegistro(&regAux);

    // percorre todo o arquivo
    while(buscaLista(listaCompleta, &regAux, regRRN)){
        if (regAux.removido != '1'){
            if (analisarCampo(filtro, &regAux)){
                imprimeRegistro(&regAux);
                flag = 1;
            }
        }
        regRRN++;
    }

    desalocarRegistro(&regAux);
    return flag;
}