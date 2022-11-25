#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ORDEM 5
#define TAMANHO_REG 960
#define TAMANHO_REG_DADOS 65

struct cabecalhoArv{
    char status; // 0 ou 1
    int noRaiz; // -1 p/ vazio
    int nroChavesTotal;
    int alturaArvore;
    int RRNproxNo; //rrn do proximo No
    char *lixo; // 48 bytes 
};
typedef struct cabecalhoArv cabecalhoArv;

struct cabecalho{
    char status; // 0 ou 1
    int topo; // -1 p/ vazio
    int proxRRN; // rrn do proximo registro
    int nRegRemov; // numero de registros removidos
    int nPagDisco; // numero de paginas de disco
    int qtdCompact; // quantidade de vezes que o arquivo foi compactado
    char *lixo; // 939 bytes

};
typedef struct cabecalho cabecalho;

struct pagArvore{
    char folha; // 0 ou 1, caso seja ou nao folha
    int nroChavesNo; // numero de chaves indexadas ao No
    int alturaNo; // altura com relacao ao No folha
    int RRNdoNo; // rrn do No

    // Pi -> ponteiro para uma subarvore de i (no total sao 5)
    // Ci -> chave de busca de i (no total sao 4)
    // Pri -> campo de referencia para o registro correspondente a Ci
    int P[ORDEM];
    int C[ORDEM-1];
    int Pr[ORDEM-1];
};
typedef struct pagArvore pagArvore;

struct registro{
    char removido; 
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

struct filtro{
    char nomeCampo[25];
    char valorCampo[25];
};
typedef struct filtro filtro;

struct elemento{
    registro *reg;
    struct elemento *prox;
};
typedef struct elemento elemento;

typedef struct elemento* Lista;