#include "funcoesPrincipais.h"

void lerPgDados(FILE *arqDados, no *arvore);
int buscarArvore(FILE *arq, int RRNarv, int chave, int *RRNachado, int *PosiAchada);
void filtrarChave(FILE *arq, FILE *arqDados, char *valorCampo);
void inserirArvore(FILE *arq, int RRNatual, int chave, int RRNchave);