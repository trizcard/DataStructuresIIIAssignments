#include <stdio.h>
#include "funcionalidades.h"

// Integrantes da dupla
// Beatriz Cardoso de Oliveira NUSP: 12566400
// Participação: 100%
// Beatriz Lomes da Silva NUSP: 12548038
// Participação: 100%

int main() {
    int func;
    char nomeArq[25];
    
    scanf("%d ", &func); // seleciona funcao
    fscanf(stdin, "%s", nomeArq); // nome do arquivo
    char nomeArq2[25];
    int n;

    // chama a funcao a ser executada
    switch (func) {
        case 1:
            fscanf(stdin, "%s", nomeArq2); // nome do arquivo
            funcUM(nomeArq, nomeArq2);
            break;
        
        case 2:
            funcDOIS(nomeArq);
            break;
        
        case 3:
            funcTRES(nomeArq);
            break;
        
        case 4:
            funcQUATRO(nomeArq);
            break;

        case 5:
            scanf("%d", &n);
            funcCINCO(nomeArq, n);
            break;
        
        case 6:
            funcSEIS(nomeArq);
            break;
    }
}