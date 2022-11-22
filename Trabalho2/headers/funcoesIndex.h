#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "estruturas.h"
#define LIXO '$'
#define TAMANHO_REG 960
#define TAMANHO_REG_DADOS 65

// Funções de manipulação da árvore-B
void inserirArvore (pagArvore arv, int chave);
int buscarArvore (pagArvore *raiz, int chave);
int buscaBin (pagArvore *arv, int chave);