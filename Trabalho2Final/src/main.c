#include <stdio.h>
#include "funcionalidades.h"

// Integrantes da dupla
// Beatriz Cardoso de Oliveira NUSP: 12566400
// Participação: 
// Beatriz Lomes da Silva NUSP: 12548038
// Participação: 

int main() {
    int func;
    char nomeArq[25];
    
    scanf("%d ", &func); // seleciona funcao
    fscanf(stdin, "%s", nomeArq); // nome do arquivo
    char nomeArq2[25];
    char nomeArq3[25];
    char nomeCampo1[25];
    char nomeCampo2[25];
    int n;

    // chama a funcao a ser executada
    switch (func) {
        /*
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
*/
        case 7:
            fscanf(stdin, "%s", nomeArq2); // nome do arquivo
            funcSETE(nomeArq,nomeArq2);
            break;
        
        case 8:
            fscanf(stdin, "%s", nomeArq2); // nome do arquivo
            scanf("%d", &n);
            funcOITO(nomeArq, nomeArq2, n);
            break;
        case 9:
            fscanf(stdin, "%s", nomeArq2);
            scanf("%d", &n);
            funcNOVE(nomeArq, nomeArq2, n);
            break;
        case 10:
            fscanf(stdin, "%s", nomeArq2); // nome do arquivo 2
            scanf("%d", &n);
            //entrada do campo do arq 1 e 2
            fscanf(stdin, "%s", nomeCampo1);
            fscanf(stdin, "%s", nomeCampo2);
            //entrada do nome do arquivo de indice
            fscanf(stdin, "%s", nomeArq3);
            funcDEZ(nomeArq, nomeArq2, nomeCampo1, nomeCampo2, nomeArq3);
    }
}