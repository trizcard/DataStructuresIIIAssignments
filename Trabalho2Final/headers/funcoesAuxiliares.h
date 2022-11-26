#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "estrutura.h"
#include "funcoesFornecidas.h"
#define LIXO '$'
#define TAMANHO_REG_DADOS 65

// Função que adiciona lixo nos campos
void addLixo (char *lixo, int inicio, int tam);

int pegarRRN (FILE* arqSaida);
void adicionarLixoCampFixo(registro *reg);
void verificarCampoString(char *campo, int tam, int inicio);
int verificarRemovido(FILE* arqSaida);
int pegarRRNencadeado(FILE* arqSaida, int RRN);
void verificaCampoNulo(char* campo, void *campoReg, int flagInt);
void adicionarCampoVariavel(FILE *arqSaida, char *campo);
void converterInt(int *campo, int flagNulo, char *valor);