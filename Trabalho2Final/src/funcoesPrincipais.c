#include "funcoesPrincipais.h"

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
