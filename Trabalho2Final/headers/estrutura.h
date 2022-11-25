#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ORDEM 5
#define MAX_CHAVE (ORDEM - 1)
#define MIN_CHAVE (MAX_CHAVE / 2)
#define TAMANHO_REG 64
#define TAMANHO_REG_DADOS 65

typedef struct {
    char status; // 0 ou 1
    int noRaiz; // -1 p/ vazio
    int nroChavesTotal;
    int alturaArvore;
    int RRNproxNo; //rrn do proximo No
    char *lixo; // 49 bytes 
}cabecalhoArv;

typedef struct {
    int c;//chave
    int Pr;//Ponteiro para o registro
}Dado;

typedef struct {
    char folha;
    int nroChavesNo;
    int alturaNo;
    int RRNdoNo;

    int P[ORDEM];
    Dado CP[ORDEM-1];
}no;

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