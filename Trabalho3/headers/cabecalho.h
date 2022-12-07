#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LIXO '$'

struct cabecalho{
    char status;
    int topo;
    int proxRRN;
    int nRegRemov;
    int nPagDisco;
    int qtdCompact;
    char *lixo;

};
typedef struct cabecalho cabecalho;

struct registro{
    char removido[1];
    int encadeamento;
    int idConecta;
    char *siglaPais;
    int idPoPsConec;
    char *undMedida;
    int veloc;

    char *nomePoPs;
    char *nomePais;
};
typedef struct registro registro;