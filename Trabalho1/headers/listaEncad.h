#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcFornecidas.h"
#include "funcAuxiliares.h"
#define LIXO '$'

void libera_lista(Lista* li);

void adicionarListaReg(Lista *lista, registro *regOrig);

void imprimirListaReg(Lista *lista);