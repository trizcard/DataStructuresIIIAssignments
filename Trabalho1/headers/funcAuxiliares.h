#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cabecalho.h"
#include "funcFornecidas.h"
#define LIXO '$'

void addLixo(char *lixo, int inicio, int tam);

int lerCampoVar(char* string, char *strPais, char *strPoPs);

int lerRegistro(FILE *arq, pagDisco *pag);

void lerPagDisco(FILE *arq, pagDisco *pag, cabecalho *cab);

int lerArq(char nome[25], cabecalho *cab);

void imprimirSaida(cabecalho *cab);

int analisarCampo(char nomeCampo[25]);

void filtrar(cabecalho *cab, pagDisco *pagNova, int tipo);

void rodarPagina(cabecalho *cab, pagDisco *pag, pagDisco *pagNova, int campo, char valorCampo[25], int tipo);

void passarReg(cabecalho *cab, pagDisco *pag, pagDisco *pagNova);

void removeReg(cabecalho *cab, pagDisco *pag, pagDisco *pagNova, int reg);