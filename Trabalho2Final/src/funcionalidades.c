#include "funcionalidades.h"

void funcSETE(char nomeArq[25]){

}

void funcOITO(char nomeArqDados[25], char nomeArq[25], int n){
    // abre arquivo e verifica se funcionou de acordo com a funcao
    FILE *arq = NULL;
    arq = fopen(nomeArq, "rb");

    if (arq == NULL){
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    filtro *filtros;
    filtros = (filtro*) malloc(n * sizeof(filtro));

    // cria cabecalho
    cabecalho cab;
    cab.lixo = (char*) malloc(939*sizeof(char));
    lerCabecalho(arq, &cab);

    int flagJaLeu = 0;
    Lista *listaCompleta;
    listaCompleta = (Lista*) malloc(sizeof(Lista));
    
    lerFiltros(filtros, n);
    for (int i = 0; i < n; i++){
        printf("Busca %d\n", (i+1));

        // caso seja busca pela chave
        if (!strcmp(filtros[i].nomeCampo, "idConecta")){
            // abre arquivo e verifica se funcionou de acordo com a funcao
            FILE *arqDados = NULL;
            arqDados = fopen(nomeArqDados, "rb");
            
            filtrarChave(arq, arqDados, filtros[i].valorCampo);
            fclose(arqDados);
        }

        // caso não seja
        else {
            if (flagJaLeu == 0){
                // cria lista com todos os registros
                lerArquivo(arq, &cab, listaCompleta);
                flagJaLeu = 1;
            }
            if (filtrar(&cab, listaCompleta, (filtros[i])) == 0){
                printf("Registro inexistente.\n");
            }
        }
        printf("\nNumero de paginas de disco: %d\n\n", cab.nPagDisco);
    }

    free(listaCompleta);
    free(filtros);
    fclose(arq);
}

void funcNOVE(char nomeArq[25]){

}

void funcDEZ(char nomeArq[25]){

}