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
    if (reg->removido[0] == '1'){
        fseek(arq, 63, SEEK_CUR);
        return 0;
    }

    fread(&reg->encadeamento, sizeof(int), 1, arq);
    fread(&reg->idConecta, sizeof(int), 1, arq);
    reg->siglaPais = malloc(3 * sizeof(char));
    fread((reg->siglaPais), sizeof(char), 2, arq);
    reg->siglaPais[2] = '\0';
    fread(&reg->idPoPsConec, sizeof(int), 1, arq);
    reg->undMedida = malloc(2 * sizeof(char));
    fread((reg->undMedida), sizeof(char), 1, arq);
    reg->undMedida[1] = '\0';
    fread(&reg->veloc, sizeof(int), 1, arq);
    
    char string[45];
    fread(string, sizeof(char), 44, arq);
    
    lerCampoVar(string, reg->nomePoPs, reg->nomePais);

    return 1;
}

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

void removerRegistro(FILE *arq, cabecalho *cab){
    fseek(arq, -64, SEEK_CUR);
    long posicao;
    int nulo = 0;
    posicao = ftell(arq);
    fwrite("1", sizeof(char), 1, arq);
    fwrite(&cab->topo, sizeof(int), 1, arq);
    fwrite(&nulo, sizeof(int), 1, arq);
    fwrite("$$", sizeof(char), 2, arq);
    fwrite(&nulo, sizeof(int), 1, arq);
    fwrite("$", sizeof(char), 1, arq);
    fwrite(&nulo, sizeof(int), 1, arq);
    fwrite("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$", sizeof(char), 44, arq);
    cab->topo = posicao;
    cab->nRegRemov++;
}

void imprimirSaida(FILE *arq){
    // cria um registro auxiliar e cabecalho auxiliar
    registro *regAux;
    regAux = (registro*) malloc(sizeof(registro));
    regAux->nomePoPs = (char *) malloc(45*sizeof(char));
    regAux->nomePais = (char *) malloc(45*sizeof(char));
    cabecalho *cabAux;
    cabAux = (cabecalho*) malloc(sizeof(cabecalho));

    lerCabecalho(arq, cabAux);

    if (cabAux->status[0] == '0'){
        printf("Falha no processamento do arquivo.\n");
        return;
    }
    
    int i = 0;

    if (i == cabAux->proxRRN){
        printf("Registro inexistente.\n\n");
    }
    
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
    int filtrado = 0; // se for 1, o registro passou pelos filtros
    
    filtro filtros[n];
    ldedRegistros **regFiltrados;
    regFiltrados = (ldedRegistros**) malloc(n * sizeof(ldedRegistros*));

    for(int i = 0; i < n; i++){
        fscanf(stdin, "%s", filtros[i].nomeCampo);
        scan_quote_string(filtros[i].valorCampo);
    }
    
    // cria um registro auxiliar
    registro *regAux;
    regAux = (registro*) malloc(sizeof(registro));
    regAux->nomePoPs = (char*) malloc(45*sizeof(char));
    regAux->nomePais = (char*) malloc(45*sizeof(char));

    // cria e le cabecalho
    cabecalho *cab;
    cab = (cabecalho*) malloc(sizeof(cabecalho));
    lerCabecalho(arq, cab);
    if (cab->status[0] == '0'){
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    int regRRN = 0;

    // percorre todo o arquivo
    while(regRRN < cab->proxRRN){

        // se registro removido retorna 0
        if (lerRegistro(arq, regAux)){
            for (int i = 0; i < n; i++){
                filtrado = analisarCampo(filtros, i, regAux);

                if (filtrado && tipo == 3){
                    adicionarListaReg((regFiltrados[i]), regAux);
                    filtrado = 0;
                }else if (tipo == 4 && filtrado){
                    removerRegistro(arq, cab);
                    filtrado = 0;
                    break;
                }
            }
            regAux = (registro*) malloc(sizeof(registro));
            regAux->nomePoPs = (char *) malloc(45*sizeof(char));
            regAux->nomePais = (char *) malloc(45*sizeof(char));
        }
        regRRN++;
    }
    for (int i = 0; i < n; i++){
        if (tipo == 3){
            printf("Busca %d\n", (i+1));
            imprimirListaReg((regFiltrados[i]));
            printf("Numero de paginas de disco: %d\n\n", cab->nPagDisco);
        }
    }

    if (tipo == 4){
        // atualiza cabecalho
        fseek(arq, 1, SEEK_SET);
        fwrite(&(cab->topo), sizeof(int), 1, arq);
        fseek(arq, 9, SEEK_SET);
        fwrite(&cab->nRegRemov, sizeof(int), 1, arq);
    }

    
    free(regAux->nomePoPs);
    free(regAux->nomePais);
    free(regAux);
    free(regFiltrados);
    free(cab);
}


// implementar lista dinamica encadeada de registros
void adicionarListaReg(ldedRegistros *lista, registro *reg){
    ldedRegistros *novo;
    novo = (ldedRegistros*) malloc(sizeof(ldedRegistros));
    novo->reg = reg;
    novo->prox = NULL;

    if (lista == NULL){
        lista = novo;
    } else {
        ldedRegistros *aux;
        aux = lista;
        while(aux->prox != NULL){
            aux = aux->prox;
        }
        aux->prox = novo;
    }
}

void imprimirListaReg(ldedRegistros *lista){
    ldedRegistros *aux;
    aux = lista;
    while(aux != NULL){
        imprimeRegistro(aux->reg);
        aux = aux->prox;
    }
    freeListaReg(lista);
}

void freeListaReg(ldedRegistros *lista){
    ldedRegistros *aux;
    while(lista != NULL){
        aux = lista;
        free(aux->reg->nomePoPs);
        free(aux->reg->nomePais);
        free(aux->reg);
        lista = lista->prox;
        free(aux);
    }
}