#include "funcoesPrincipais.h"

//Cabeçalho da árvore
void inicializaCabecalho(cabecalhoArv *cab);
void criarCabArqIndice(FILE *arqIndice, cabecalhoArv *cabArv);
void lerCabecalhoArv(FILE *arq, cabecalhoArv *cab);

void lerPgDados(FILE *arqDados, no *arvore);
void inicializarArv(no *pagArv);
void atualizarCabArv(FILE *arqArvore, cabecalhoArv *cabArv);
int buscarArvore(FILE *arq, int RRNarv, int chave, int *RRNachado, int *PosiAchada, int pagAcessadas);
int buscarPagina(no pagAtual, int chave, int *PosiAchada);
int filtrarChave(FILE *arq, FILE *arqDados, char *valorCampo);
int getRRNproxNo(FILE *arqArvore);
int criaRaiz(FILE *arq, int chave, int RRNchave, int esq, int dir);

int inserirArv(FILE *arq, int chave, int RRNchave, int RRNarv, promovidos *Promovido, cabecalhoArv *cabArv);

void split(promovidos PromB, no *pagAntiga, promovidos *Promovido, no *pagNova);
void inserirNo(no *pagAtual, promovidos PromB);
void alterarNo(FILE *arq, no *pagAtual, int RRNarv);
int getProxRRN(FILE *arq);
void atualizaProxRRN(FILE *arq, int RRN);
Dado buscarDado(FILE *arqDados, int RRN, int posi);


void imprimeArvoreB(FILE* arquivo, cabecalhoArv* c);
void imprimeNo (no* n);

void incrementarRRNarv(FILE *arqArvore);
