#include "funcoesPrincipais.h"

void criarCabecalho(cabecalho *cab){
    cab->status = '0';
    cab->topo = -1;
    cab->proxRRN = 0;
    cab->nRegRemov = 0;
    cab->nPagDisco = 1;
    cab->qtdCompact = 0;
    cab->lixo = malloc(939 * sizeof(char));
    addLixo(cab->lixo, 0, 939); 
}

// Função que adiciona o cabeçalho no arquivo de saída
void adicionarCabecalhoArq(FILE *arqSaida, cabecalho *cab){
    fwrite(&cab->status, sizeof(char), 1, arqSaida);
    fwrite(&cab->topo, sizeof (int), 1, arqSaida);
    fwrite(&cab->proxRRN, sizeof (int), 1, arqSaida);
    fwrite(&cab->nRegRemov, sizeof (int), 1, arqSaida);
    fwrite(&cab->nPagDisco, sizeof (int), 1, arqSaida);
    fwrite(&cab->qtdCompact, sizeof (int), 1, arqSaida);
    fwrite(cab->lixo, sizeof(char), 939, arqSaida);
}

// Função que atualiza o cabecalho após a inserção dos registros
void atualizarCabecalho(FILE* arqSaida, cabecalho *cab, int RRN, int qtdCompactado){
    cab->status = '1';
    cab->topo = -1;
    cab->proxRRN = RRN;
    cab->qtdCompact = qtdCompactado;
    fseek(arqSaida, 0, SEEK_SET);
    adicionarCabecalhoArq(arqSaida, cab);
}

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

// Função que inicializa o registro após a sua criação e seta os valores iniciais
void inicializarRegistro(registro *reg){
    reg->removido = '0';
    reg->encadeamento = -1;
    reg->nomePais = (char *) malloc(45 * sizeof(char));
    reg->nomePoPs = (char *) malloc(45 * sizeof(char));
    reg->siglaPais = (char *) malloc(3 * sizeof(char));
    reg->undMedida = (char *) malloc(2 * sizeof(char));
    reg->veloc = -1;
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

// Função de remoção lógica do registro
void removerRegistro(FILE *arq, cabecalho *cab){
    fseek(arq, -64, SEEK_CUR);
    long posicao;
    posicao = ftell(arq);

    fwrite("1", sizeof(char), 1, arq);
    fwrite(&cab->topo, sizeof(int), 1, arq);
    fwrite("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$", sizeof(char), 59, arq);
    cab->topo = (posicao-960)/64;
    cab->nRegRemov++; 
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

// Função que printa os registros de uma lista encadeada de registros
void imprimirListaReg(Lista *lista){
    elemento *aux;
    aux = *lista;
    
    while(aux != NULL){
        imprimeRegistro(aux->reg);
        aux = aux->prox;
    }

    libera_lista(lista);
}

// Função que printa todos os registros do arquivo de entrada
void imprimirSaida(FILE *arq){
    // cria um registro auxiliar e cabecalho auxiliar
    registro regAux;
    alocaRegistro(&regAux);

    cabecalho cabAux;
    cabAux.lixo = (char *) malloc(939*sizeof(char));

    lerCabecalho(arq, &cabAux);

    if (cabAux.status == '0'){
        printf("Falha no processamento do arquivo.\n");
        free(cabAux.lixo);
        desalocarRegistro(&regAux);
        return;
    }
    
    int i = 0;

    if (i == cabAux.proxRRN){
        printf("Registro inexistente.\n\n");
    }
    
    // percorre todo o arquivo
    while(i < cabAux.proxRRN){
        // se registro removido retorna 0
        if (lerRegistro(arq, &regAux)){
            imprimeRegistro(&regAux);
        }
        i++;
    }
    printf("Numero de paginas de disco: %d\n\n", cabAux.nPagDisco);

    desalocarRegistro(&regAux);
    free(cabAux.lixo);
}

// Função que analisa os campos de um registros de acordo com o filtro
int analisarCampo(filtro *filtros, int i, registro *reg){
    if (!strcmp(filtros[i].nomeCampo, "idConecta")){
        int idConecta;
        idConecta = atoi(filtros[i].valorCampo);
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
        int idPoPsConectado;
        idPoPsConectado = atoi(filtros[i].valorCampo);
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
        int velocidade;
        velocidade = atoi(filtros[i].valorCampo);
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

// Função que filtra os registros de acordo com as entradas
void filtrar(FILE *arq){ 
    int n;
    scanf("%d", &n);
    
    // cria um registro auxiliar
    registro regAux;
    alocaRegistro(&regAux);

    // cria e le cabecalho
    cabecalho cab;
    cab.lixo = (char*) malloc(939*sizeof(char));

    lerCabecalho(arq, &cab);
    if (cab.status == '0'){
        printf("Falha no processamento do arquivo.\n");
        free(cab.lixo);
        desalocarRegistro(&regAux);
        return;
    }
    fseek(arq, 0, SEEK_SET);
    char status = '0';
    fwrite(&status, sizeof(char), 1, arq);
    fseek(arq, 960, SEEK_SET);

    filtro filtros[n];

    // uma lista de registros para cada um dos filtros
    Lista **regFiltrados;
    regFiltrados = (Lista**) malloc(n * sizeof(Lista*));

    for(int i = 0; i < n; i++){
        fscanf(stdin, "%s", filtros[i].nomeCampo);
        scan_quote_string(filtros[i].valorCampo);
        regFiltrados[i] = (Lista*) malloc(sizeof(Lista));
    }

    int regRRN = 0;

    // percorre todo o arquivo
    while(regRRN < cab.proxRRN){
        // se registro removido retorna 0
        if (lerRegistro(arq, &regAux)){
            for (int i = 0; i < n; i++){
                int filtrado = 0; // se for 1, o registro passou pelos filtros
                filtrado = analisarCampo(filtros, i, &regAux);

                if (filtrado){
                    adicionarListaReg((regFiltrados[i]), &regAux);
                }
            }
        }
        regRRN++;
    }

    for (int i = 0; i < n; i++){
        printf("Busca %d\n", (i+1));
        if (*regFiltrados[i] == NULL){
            printf("Registro inexistente.\n\n");
            free(regFiltrados[i]);
        }else{
            imprimirListaReg((regFiltrados[i]));
        }
        printf("Numero de paginas de disco: %d\n\n", cab.nPagDisco);
    }

    desalocarRegistro(&regAux);
    free(regFiltrados);
    free(cab.lixo);
}

// Função que remove os registros filtrados de acordo com as entradas
int remover(FILE *arq){
    int n;
    scanf("%d", &n);
    
    // cria um registro auxiliar
    registro regAux;
    alocaRegistro(&regAux);

    // cria e le cabecalho
    cabecalho cab;
    cab.lixo = (char*) malloc(939*sizeof(char));

    lerCabecalho(arq, &cab);
    if (cab.status == '0'){
        printf("Falha no processamento do arquivo.\n");
        free(cab.lixo);
        desalocarRegistro(&regAux);
        return 0;
    }
    fseek(arq, 0, SEEK_SET);
    char status = '0';
    fwrite(&status, sizeof(char), 1, arq);
    fseek(arq, 960, SEEK_SET);

    filtro filtros[n];

    for(int i = 0; i < n; i++){
        fscanf(stdin, "%s", filtros[i].nomeCampo);
        scan_quote_string(filtros[i].valorCampo);
    }

    int regRRN = 0;

    // percorre todo o arquivo
    while(regRRN < cab.proxRRN){
        // se registro removido retorna 0
        if (lerRegistro(arq, &regAux)){
            for (int i = 0; i < n; i++){
                int filtrado = 0; // se for 1, o registro passou pelos filtros
                filtrado = analisarCampo(filtros, i, &regAux);

                if (filtrado){
                    removerRegistro(arq, &cab);
                    break;
                }
            }
        }
        regRRN++;
    }

    // atualiza cabecalho
    fseek(arq, 1, SEEK_SET);
    fwrite(&(cab.topo), sizeof(int), 1, arq);
    fseek(arq, 9, SEEK_SET);
    fwrite(&cab.nRegRemov, sizeof(int), 1, arq);

    desalocarRegistro(&regAux);
    free(cab.lixo);
    return 1;
}