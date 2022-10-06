#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cabecalho.h"
#define LIXO '$'

void addLixo(char *lixo, int inicio, int tam);

int lerCampoVar(char* string, char *strPais, char *strPoPs);

int lerRegistro(FILE *arq, pagDisco *pag);

void lerPagDisco(FILE *arq, pagDisco *pag, cabecalho *cab);

int lerArq(char nome[25], cabecalho *cab);

void imprimirSaida(cabecalho *cab);