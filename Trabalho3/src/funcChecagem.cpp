#include "../headers/funcChecagem.h"


/**
 * @brief Função responsável por checar a consistência de um arquivo e 
 * retornar uma mensagem de erro caso o arquivo esteja inconsistente
 *
 * @param reg Ponteiro para o registro de cabeçalho
 * @return int Retorna 1 caso o arquivo esteja inconsistente, 0 caso contrári
 */


int checa_consistencia(reg_cabecalho* reg){
    if(reg->status[0] != '1'){//se for diferente de 1, ou seja, inconsistente
        printf("Falha na execução da funcionalidade.\n");
        return 1;//retorna 1
    }
    return 0;//caso contrário, retorna 0
}

