#include "../headers/funcLeituraEscrita.h"
#include "../headers/funcFornecidas.h"

/**
 * @brief Função responsável por ler o registro de cabeçalho de um arquivo binário e colocar numa struct.
 * 
 * @param arquivo Ponteiro para o arquivo binário
 * @param reg Ponteiro para a struct que vai receber os dados do registro de cabeçalho
 */

void ler_reg_cabecalho(FILE* arquivo, reg_cabecalho* reg){
    fread(reg->status, sizeof(char), 1, arquivo);
    reg->status[1] = '\0'; //para tratar como string
    fread(&reg->topo, sizeof(int), 1, arquivo);
    fread(&reg->proxRRN, sizeof(int), 1, arquivo);//lê o próximo RRN disponível
    fread(&reg->nroRegRem, sizeof(int), 1, arquivo);//lê o número de registros removidos
    fread(&reg->nroPagDisco, sizeof(int), 1, arquivo);//lê o número de páginas de disco
    fread(&reg->qttCompacta, sizeof(int), 1, arquivo);//lê o qttCompacta
    fseek(arquivo, (TAM_PAG_DISCO - TAM_REG_CABECALHO), SEEK_CUR);//pula o lixo
}

/**
 * @brief Função responsável por ler todos os campos do registro
 * 
 * @param reg Ponteiro para o registro de dados
 * @param arquivo_entrada Ponteiro para o arquivo binário
 */

void le_registro(reg_dados* reg, FILE* arquivo_entrada){
  le_campos_tam_fixo(reg,arquivo_entrada);
  le_campos_tam_variavel(reg,arquivo_entrada);
}

/**
 * @brief Função responsável por ler os campos de tamanho fixo de um registro de dados.
 * 
 * @param reg Ponteiro para o registro de dados
 * @param arquivo_entrada Ponteiro para o arquivo binário
 */

void le_campos_tam_fixo(reg_dados* reg, FILE* arquivo_entrada){

    fread(&reg->encadeamento, sizeof(int), 1, arquivo_entrada);
    fread(&reg->idConecta,sizeof(int), 1, arquivo_entrada);
    fread(reg->siglaPais,sizeof(char)*2, 1, arquivo_entrada);
    reg->siglaPais[2]='\0';
    fread(&reg->idPoPsConectado,sizeof(int), 1, arquivo_entrada);
    fread(reg->unidadeMedida,sizeof(char), 1, arquivo_entrada);
    reg->unidadeMedida[1]='\0';
    fread(&reg->velocidade,sizeof(int), 1, arquivo_entrada);

}

/**
 * @brief Função responsável por ler os campos de tamanho variável de um registro de dados.
 * 
 * @param reg Ponteiro para o registro de dados
 * @param arquivo_entrada Ponteiro para o arquivo binário
 */


void le_campos_tam_variavel(reg_dados* reg, FILE* arquivo_entrada){

    char buffer = '0'; // armazena character lido
    int pos_nomePoPs=0; // posição na string de nomePops
    int pos_nomePais=0; // posição na string de nomePais
    int tam_lixo; // tamanho do lixo 
    
    //le campos variaveis
    while (buffer !='|' && buffer != '$') //enquanto não chegar no | (campo acabou) ou no $ (registro foi removido)
    {
        fread(&buffer, sizeof(char), 1, arquivo_entrada); // le o char 

        reg->nomePoPs[pos_nomePoPs]=buffer; //constroi a string
        pos_nomePoPs+=1; //atualiza a posição
                
    }
        
    buffer = '0'; //reseta o buffer

    while (buffer !='|' && buffer != '$') //enquanto não chegar no | (campo acabou) ou no $ (registro foi removido)
    {
        fread(&buffer, sizeof(char), 1, arquivo_entrada); // le o char
                
        reg->nomePais[pos_nomePais]=buffer; //constroi a string
        pos_nomePais+=1; //atualiza a posição
    }
             
    reg->nomePoPs[pos_nomePoPs-1]='\0'; // substitui o | por um \0
    reg->nomePais[pos_nomePais-1]='\0';

    tam_lixo=TAM_REG_DADOS-pos_nomePoPs-pos_nomePais-20; //descobre o tamanho do lixo 
         
    if (tam_lixo!=0){  //confere se existe lixo
    char lixo[tam_lixo];  //cria a string lixo
    fread(lixo,sizeof(char)*tam_lixo, 1, arquivo_entrada); //le o lixo
    }
}

/**
 * @brief Função responsável por ler o arquivo até o fim
 * 
 * @param reg Ponteiro para o registro de dados
 * @param arquivo_entrada Ponteiro para o arquivo binário
 * @return int Retorna 1 se o arquivo não chegou ao fim, 0 caso contrário
 */


int le_arquivo(reg_dados* reg, FILE* arquivo_entrada){
    if(fread(reg->removido,sizeof(char), 1, arquivo_entrada) != 0){//se não chegou ao fim
        reg->removido[1] = '\0'; //marca o fim da string
        le_registro(reg,arquivo_entrada);
        return 1; //retorna 1 para indicar que o arquivo não chegou ao fim
    }
    return 0; //retorna 0 para indicar que o arquivo chegou ao fim
}


/**
 * @brief Função responsável por escrever uma string usando fwrite
 * 
 * @param str String a ser escrita
 * @param arquivo Ponteiro para o arquivo binário
 * @param tam Tamanho da string
 */

void escreve_string_no_arquivo(char* str, FILE* arquivo, int tam){
    fwrite(str, sizeof(char), tam, arquivo);
}

/**
 * @brief Função responsável por escrever os dados da struct do registro de cabeçalho no arquivo
 * 
 * @param arquivo Ponteiro para o arquivo binário
 * @param reg Ponteiro para o registro de cabeçalho
 */

void escrever_no_arquivo_cabecalho(FILE* arquivo, reg_cabecalho* reg){
    
    escreve_string_no_arquivo(reg->status, arquivo, 1);
    fwrite(&reg->topo, sizeof(int), 1, arquivo);
    fwrite(&reg->proxRRN, sizeof(int), 1, arquivo);
    fwrite(&reg->nroRegRem, sizeof(int), 1, arquivo);
    fwrite(&reg->nroPagDisco, sizeof(int), 1, arquivo);
    fwrite(&reg->qttCompacta, sizeof(int), 1, arquivo);
    
    for(int i = 0; i < (TAM_PAG_DISCO - TAM_REG_CABECALHO); i++){ //escreve lixo completando a pagina de disco
        fwrite(LIXO, sizeof(char), 1, arquivo);
    }
}

