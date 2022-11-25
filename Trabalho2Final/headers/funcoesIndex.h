#include "funcoesPrincipais.h"

void lerPgDados(FILE *arqDados, no *arvore);
int buscarArvore(FILE *arq, int RRNarv, int chave, int *RRNachado, int *PosiAchada, int pagAcessadas);
int filtrarChave(FILE *arq, FILE *arqDados, char *valorCampo);
int inserirArvore(FILE *arq, int RRNatual, int chave, int RRNchave, int *RRNnovo, int *chavePromovida, int *RRNpromovido);