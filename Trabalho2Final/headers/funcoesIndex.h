#include "funcoesPrincipais.h"

void lerPgDados(FILE *arqDados, no *arvore);
void lerCabecalhoArv(FILE *arq, cabecalhoArv *cab);
void inicializarArv(no *pagArv);
void atualizarCabArv(FILE *arqArvore, cabecalhoArv *cabArv);
int buscarArvore(FILE *arq, int RRNarv, int chave, int *RRNachado, int *PosiAchada, int pagAcessadas);
int buscarPagina(no pagAtual, int chave, int *PosiAchada);
int filtrarChave(FILE *arq, FILE *arqDados, char *valorCampo);
int inserirArvore(FILE *arq, int chave, int RRNchave, int RRNarv, promovidos *Promovido, cabecalhoArv *cabArv);
void split(promovidos PromB, no *pagAntiga, promovidos *Promovido, no *pagNova);
void inserirNo(no *pagAtual, promovidos PromB);
void alterarNo(FILE *arq, no *pagAtual, int RRNarv);