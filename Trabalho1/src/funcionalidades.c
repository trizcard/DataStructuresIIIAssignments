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

void funcTRES(char nomeArq[25]){}

void funcQUATRO(char nomeArq[25]){}

void funcCINCO(char nomeArq[25]){}

void funcSEIS(char nomeArq[25]){}