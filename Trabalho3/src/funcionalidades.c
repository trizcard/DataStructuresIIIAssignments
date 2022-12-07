#include "funcionalidades.h"

void funcONZE(char nomeArq[25]) {

}

void funcDOZE(char nomeArq[25]) {

}

void funcTREZE(char nomeArq[25], int n) {

}

void funcCATORZE(char nomeArq[25], int n) {
    int orig, fim, parada, cMin;
    grafo *g;
    lerGrafo(nomeArq, g);
    
    for (int i = 0; i < n; i++){
        scanf("%d %d %d", &orig, &fim, &parada);
        cMin = 0;
        // implementar Algoritmo de Dijkstra para encontrar o caminho minimo
        cMin += caminhoMinimo(orig, parada, g);
        cMin += caminhoMinimo(parada, fim, g);
        printf("Comprimento do caminho entre %d e %d parando em %d: %d Mpbs", orig, fim, parada, cMin);
    }
}
