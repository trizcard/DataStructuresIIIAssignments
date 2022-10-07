#include "funcionalidades.h"

void funcUM(char nomeArq[25]){
}

void funcDOIS(char nomeArq[25]){
    cabecalho *cab;
    cab = (cabecalho*) malloc(sizeof(cabecalho));

    int existe;
    existe = lerArq(nomeArq, cab);
    
    if(existe == 1){
        imprimirSaida(cab);
    }
}

void funcTRES(char nomeArq[25]){
    cabecalho *cab;
    cab = (cabecalho*) malloc(sizeof(cabecalho));

    int existe;
    existe = lerArq(nomeArq, cab);

    if(existe == 1){
        pagDisco *pagNova;
        if (!strcmp(cab->status, "0")){
            return;
        }
        pagNova = cab->prox;
        filtrar(cab, pagNova, 3);

        imprimirSaida(cab);
    }
    free(cab);
}

void funcQUATRO(char nomeArq[25]){
    cabecalho *cab;
    cab = (cabecalho*) malloc(sizeof(cabecalho));

    int existe;
    existe = lerArq(nomeArq, cab);

    cabecalho *cabOriginal;
    cabOriginal = (cabecalho*) malloc(sizeof(cabecalho));

    if(existe == 1){
        pagDisco *pagNova;
        if (!strcmp(cab->status, "0")){
            return;
        }
        pagNova = cab->prox;
        filtrar(cab, pagNova, 4);
    }
    free(cab);
    free(cabOriginal);
}

void funcCINCO(char nomeArq[25]){}

void funcSEIS(char nomeArq[25]){}