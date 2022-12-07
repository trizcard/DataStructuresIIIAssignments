#include "funcoesAuxiliares.h"

void addLixo(char *lixo, int inicio, int tam){
    for (int i = inicio; i < (inicio + tam); i++){
        lixo[i] = LIXO;
    }
}


// Pega o RRN do próximo registro
int pegarRRN(FILE* arqSaida){
    int RRN;
    fseek(arqSaida, 5, SEEK_SET);
    fread(&RRN, sizeof(int), 1, arqSaida);
    return RRN;
}

void adicionarLixoCampFixo(registro *reg){
    //Verificar se o campo é nulo
    verificarCampoString(reg->siglaPais, 2, 0);
    verificarCampoString(reg->undMedida, 1, 0);
}

void verificarCampoString(char *campo, int tam, int inicio){
    if (!strcmp(campo, "")){
        addLixo(campo, inicio, tam);
    }   
}

int verificarRemovido(FILE* arqSaida){
    int removido;
    fseek(arqSaida, 1, SEEK_SET);
    fread (&removido, sizeof(int), 1, arqSaida);
    return removido;
}

int pegarRRNencadeado(FILE* arqSaida, int RRN){
    int encadeamento;
    fseek(arqSaida, (1 +(960 + (RRN * 64))), SEEK_SET);
    fread(&encadeamento, sizeof(int), 1, arqSaida);
    return encadeamento;
}

void verificaCampoNulo(char* campo, void *campoReg, int flagInt){
    if(!strcmp(campo, "NULO")){
        if(flagInt == 1){
            converterInt(campoReg, 1, NULL);
        }else{
            strcpy(campoReg, "");
        }
    }else{
        if(flagInt == 1){
            converterInt(campoReg, 0, campo);
        }
    }
}

void adicionarCampoVariavel(FILE *arqSaida, char *campo){
    if(strcmp(campo, "")){
        fwrite(campo, sizeof(char) , strlen(campo), arqSaida);
    }
    fwrite("|", sizeof(char), 1, arqSaida);
}

void converterInt(int *campo, int flagNulo, char *valor){
    if(flagNulo){
        *campo = -1;
    }else{
        *campo = atoi(valor);
    }
}

//Abre o arquivo de entrada e verifica se ele está consistente
FILE* abrirArquivo(char nomeArq[25], char modo[2]){
    FILE *arqEntrada;
    arqEntrada = fopen(nomeArq, modo);
    if(arqEntrada == NULL){
        printf("Falha no carregamento do arquivo.\n");
        exit(0);
    }

    //Verifica inconsistencias no arquivo de entrada
    char status;
    fread(&status, sizeof(char), 1, arqEntrada);
    if(status == '0'){
        printf("Falha no processamento do arquivo.\n");
        fclose(arqEntrada);
        exit(0);
    }
    return arqEntrada;
}

//Verificar se campo de busca é idConect (se é a chave)
int verificarChave(char campo[20]){
    if(strcmp(campo, "idConecta") == 0){
        return 1;
    }
    return 0;
}

//Função para verificar se campo int é -1
int verificarInt(int campo){
    if(campo == -1){
        return 1;
    }
    return 0;
}

//Função para verificar se campo string é nulo
int verificarString(char campo[20]){
    if(strcmp(campo, "") == 0){
        return 1;
    }
    return 0;
}
