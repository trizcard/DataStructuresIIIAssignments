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
    char siglaPais[3];
    int idPoPsConec;
    char undMedida[2];
    int veloc;

    char *nomePoPs;
    char *nomePais;
};
typedef struct registro registro;

struct filtro{
    char nomeCampo[25];
    char valorCampo[25];
};
typedef struct filtro filtro;