#include "estrutura.h"
#include "funcoesFornecidas.h"

void lerCabecalho (FILE *arq, cabecalho *cab);

// Funções relacionadas aos registros
void alocaRegistro(registro *reg);
void desalocarRegistro (registro *reg);
int lerRegistro (FILE *arq, registro *reg);
void passarReg(registro *reg, registro *regOrig);
void adicionarListaReg(Lista *lista, registro *regOrig);
int buscaLista(Lista *lista, registro *reg, int index);
void lerCampoVar (char *string, char *strPoPs, char *strPais);
void lerArquivo(FILE *arq, cabecalho *cab, Lista *lista);
void imprimeRegistro (registro *regAux);

// Funções de filtro
int analisarCampo(filtro filtros, registro *reg);
void lerFiltros(filtro *filtros, int n);
int filtrar(cabecalho *cab, Lista *listaCompleta, filtro filtro);