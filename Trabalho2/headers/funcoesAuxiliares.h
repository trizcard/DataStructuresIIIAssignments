#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "estruturas.h"
#include "funcoesFornecidas.h"
#define LIXO '$'
#define TAMANHO_REG 960
#define TAMANHO_REG_DADOS 65

// Função que adiciona lixo nos campos
void addLixo (char *lixo, int inicio, int tam);

// Funções aux do cabecalho
void atualizarQuantidadeCompactada (FILE* arqSaida);
int pegarQuantidadeCompactada (FILE *arq);

// Funções de manipulação dos campos da tabela
void quebrarString (char *linha, registro *reg);
void verificarCampoString (char *campo, int tam, int inicio);
void adicionarCampoVariavel (FILE *arqSaida, char *campo);
void addValorInt (int *set, int flag, char *valor);
void addValorString (char *set, int flag, char* valor);
void percorrerCampo (void *campo, int *posi, char *linha, char *varAux, int inteiro);

// entrada e leitura
void entradaRegistro (registro *reg);
void adicionarRegArqSaida (FILE *arqSaida, registro *reg);
void adicionarLixoCampFixo (registro *reg);
void adicionarListaReg (Lista *lista, registro *regOrig);
void libera_lista (Lista* li);
void passarReg (registro *reg, registro *regOrig);

// Manipulação da entrada
void converterInt (int *campo, int flagNulo, char *valor);
void verificaCampoNulo (char* campo, void *campoReg, int flagInt);

// Funções de verificar dados do arq
int verificarRemovido (FILE* arqSaida);

// Atualizar dados do arq
void atualizarNroRegRemovidos (FILE* arqSaida);
void atualizarTopo (FILE* arqSaida, int RRN);
void atualizarRRN (FILE* arqSaida, int RRN);
void atualizarNroPagDisco (FILE* arqSaida, int RRN);

// Pegar dados do arq
int pegarNroRegRemovidos (FILE* arqSaida);
int pegarRRNencadeado (FILE* arqSaida, int RRN);
int pegarRRN (FILE* arqSaida);