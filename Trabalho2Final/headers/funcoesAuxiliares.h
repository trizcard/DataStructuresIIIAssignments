#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "estrutura.h"
#include "funcoesFornecidas.h"
#define LIXO '$'
#define TAMANHO_REG 960
#define TAMANHO_REG_DADOS 65

// Função que adiciona lixo nos campos
void addLixo (char *lixo, int inicio, int tam);

int pegarRRN (FILE* arqSaida);