#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LIXO '$'

typedef struct{
    char status[1];
    int topo;
    int proxRRN;
    int nRegRemov;
    int nPagDisco;
    int qtdCompact;
    char lixo[939];
}cabecalho;

typedef struct{
    char removido[1];
    int encadeamento;
    int idConecta;
    char siglaPais[2];
    int idPoPsConec;
    char undMedida[1];
    int veloc;

    char *nomePoPs;
    char *nomePais;
}registro;