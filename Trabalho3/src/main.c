#include <stdio.h>
#include "funcionalidades.h"

// Integrantes
// Beatriz Cardoso de Oliveira NUSP: 12566400
// Participação:
// Beatriz Lomes da Silva NUSP: 12548038
// Participação: 
// Beatriz Aimee Teixeira Furtado Braga NUSP: 
// Participação:

int main() {
    int func;
    char nomeArq[25];
    
    scanf("%d ", &func); // seleciona funcao
    fscanf(stdin, "%s", nomeArq); // nome do arquivo
    int n;

    // chama a funcao a ser executada
    switch (func) {
        case 11:
            funcONZE(nomeArq);
            break;
        
        case 12:
            funcDOZE(nomeArq);
            break;
        
        case 13:
            scanf("%d", &n);
            funcTREZE(nomeArq, n);
            break;
        
        case 14:
            scanf("%d", &n);
            funcCATORZE(nomeArq, n);
            break;
    }
}