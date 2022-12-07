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

struct adjacencia{
    int vertice; // vertice de destino
    int peso; // peso da aresta
    adjacencia *prox; // proximo elemento da lista de adjacencia
};
typedef struct adjacencia adjacencia;

struct vertice{
    registro reg;
    adjacencia *cab;
};
typedef struct vertice vertice;

struct grafo{
    int qtdA; // quantidade de arestas do grafo
    int qtdV; // quantidade de vertices do grafo
    vertice *adj; // arranjo de vertices
};
typedef struct grafo grafo;