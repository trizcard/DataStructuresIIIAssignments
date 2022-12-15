#include "../headers/funcAbertura.h"


/**
 * @brief Função responsável por abrir um arquivo binario para leitura a partir de uma string que contém o nome do arquivo, 
 * e printar mensagem de erro caso não seja possível abrir o arquivo
 * 
 * @param nome_do_arquivo String que contém o nome do arquivo
 * @return FILE* Ponteiro para o arquivo aberto
 */

FILE* abrir_leitura_binario(char* nome_do_arquivo){
    FILE* arquivo;
    arquivo = fopen(nome_do_arquivo, "rb");

    if(arquivo == NULL){
        printf("Falha na execução da funcionalidade.\n");
        return 0;
    }
    return arquivo;
}

