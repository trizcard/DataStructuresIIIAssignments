#ifndef __LEITURA_H__
#define __LEITURA_H__

#include "../headers/estrutura.h"

//leitura do registro
void le_registro(reg_dados*, FILE*);
void le_campos_tam_fixo(reg_dados*, FILE*);
void le_campos_tam_variavel(reg_dados*, FILE*);

int le_arquivo(reg_dados*, FILE*);
void ler_reg_cabecalho(FILE*, reg_cabecalho*);

//escrita

void escreve_string_no_arquivo(char*, FILE*, int);
void escrever_no_arquivo_cabecalho(FILE*, reg_cabecalho*);

#endif