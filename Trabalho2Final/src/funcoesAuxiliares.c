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
