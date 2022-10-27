#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cabecalho.h"
#include "funcFornecidas.h"
#define LIXO '$'

void addLixo(char *lixo, int inicio, int tam);

void lerCampoVar(char* string, char *strPais, char *strPoPs);

void lerCabecalho(FILE *arq, cabecalho *cab);

int lerRegistro(FILE *arq, registro *reg);

void imprimeRegistro(registro *regAux);

void removerRegistro(FILE *arq, cabecalho *cab);

void imprimirSaida(FILE *arq);

int analisarCampo(filtro *filtros, int i, registro *reg);

void filtrar(FILE *arq, int tipo);

void adicionarListaReg(ldedRegistros *lista, registro *regAux);

void imprimirListaReg(ldedRegistros *lista);

void freeListaReg(ldedRegistros *lista);