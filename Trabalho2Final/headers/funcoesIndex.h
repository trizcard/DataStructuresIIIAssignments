#include "funcoesPrincipais.h"

void lerPgDados(FILE *arqDados, no *arvore);
void lerCabecalhoArv(FILE *arq, cabecalhoArv *cab);
void inicializarArv(no *pagArv);
int buscarArvore(FILE *arq, int RRNarv, int chave, int *RRNachado, int *PosiAchada, int pagAcessadas);
int buscarPagina(no pagAtual, int chave, int *PosiAchada);
int filtrarChave(FILE *arq, FILE *arqDados, char *valorCampo);
int inserirArvore(FILE *arq, int chave, int RRNchave, int RRNarv, promovidos *Promovido);
void split(promovidos PromB, no *pagAntiga, promovidos *Promovido, no *pagNova);
void inserirNo(no *pagAtual, promovidos PromB);
void alterarNo(FILE *arq, no *pagAtual, int RRNarv);