#include <stdio.h>
#include "funcionalidades.h"

int main() {
    int func;
    char nomeArq[25];
    scanf("%d ", &func);
    fscanf(stdin, "%s", nomeArq);

    switch (func) {
    case 1:
        funcUM(nomeArq);
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
        funcCINCO(nomeArq);
        break;
    
    case 6:
        funcSEIS(nomeArq);
        break;
    }
}