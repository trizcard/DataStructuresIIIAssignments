#include "../headers/estrutura.h"
#include "../headers/funcionalidades.h"

// Integrantes
// Beatriz Cardoso de Oliveira NUSP: 12566400
// Participação:
// Beatriz Lomes da Silva NUSP: 12548038
// Participação: 
// Beatriz Aimee Teixeira Furtado Braga NUSP: 12547934
// Participação:

int main() {
    
    int func;
    char nomeArq[TAM_NOME_ARQUIVO];
    
    scanf("%d ", &func); // seleciona funcao
    scanf("%s", nomeArq); // nome do arquivo de entrada
    
    //int n; 

    // chama a funcao a ser executada
    switch (func) {
        case 11:
            comando11(nomeArq);
            break;
        
       /* case 12:
            comando12(nomeArq);
            break;
        
        case 13:
            scanf("%d", &n);
            comando13(nomeArq, n);
            break;
        */
        case 14:
            int n;
            scanf("%d", &n);
            comando14(nomeArq, n);
            break;
    }
    return 0;
}