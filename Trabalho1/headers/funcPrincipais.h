#include "funcAuxiliares.h"

// Funções do cabeçalho
void criarCabecalho (cabecalho *cab);
void adicionarCabecalhoArq (FILE *arqSaida, cabecalho *cab);
void atualizarCabecalho (FILE* arqSaida, cabecalho *cab, int RRN, int qtdCompactado);
void lerCabecalho (FILE *arq, cabecalho *cab);

// Funções do registro
void inicializarRegistro (registro *reg);
void alocaRegistro(registro *reg);
void desalocarRegistro (registro *reg);
int lerRegistro (FILE *arq, registro *reg);
void lerCampoVar (char *string, char *strPoPs, char *strPais);
// Remoção e impressão de registros
void removerRegistro (FILE *arq, cabecalho *cab);
void imprimeRegistro (registro *regAux);
void imprimirListaReg (Lista *lista);

// Funções de verificar dados do arq
int verificarRemovido (FILE* arqSaida);

// Funções gerais das funcionalidades
void imprimirSaida (FILE *arq);
int analisarCampo (filtro *filtros, int i, registro *reg);
int filtrar (FILE *arq, int tipo);