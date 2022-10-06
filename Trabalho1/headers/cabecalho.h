#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LIXO '$'

struct cabecalho{
    char status[1];
    int topo;
    int proxRRN;
    int nRegRemov;
    int nPagDisco;
    int qtdCompact;
    char lixo[939];

    struct pagDisco *prox;
};
typedef struct cabecalho cabecalho;


struct registro{
    char removido[1];
    int encadeamento;
    int idConecta;
    char siglaPais[2];
    int idPoPsConec;
    char undMedida[1];
    int veloc;

    char *nomePoPs;
    char *nomePais;

    struct registro *prox;
};
typedef struct registro registro;

struct pagDisco{
    struct registro *inicio;
    struct registro *fim;
    
    int tamanho;
    struct pagDisco *prox;
};
typedef struct pagDisco pagDisco;