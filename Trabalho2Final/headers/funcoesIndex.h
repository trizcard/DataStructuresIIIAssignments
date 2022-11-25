#include "funcoesDados.h"

void lerPgDados(FILE *arqDados, pagArvore *arvore);
int buscarArvore(FILE *arq, int RRNarv, int chave, int *RRNachado, int *PosiAchada);
void filtrarChave(FILE *arq, FILE *arqDados, char *valorCampo);
