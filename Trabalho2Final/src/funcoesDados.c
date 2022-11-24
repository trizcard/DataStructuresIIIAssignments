#include "estrutura.h"

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
    // ler cabecalho
    lerCabecalho(arq, cab);
    // dar falha se status for 0
    if (cab->status == '0'){
        printf("Falha no processamento do arquivo.\n");
        return;
    }

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