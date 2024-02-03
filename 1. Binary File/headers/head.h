#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TRASH '$'

struct head{
    char status;
    int top;
    int nextRRN;
    int nRegRemov;
    int nPagDisk;
    int qttCompact;
    char *trash;

};
typedef struct head head;


struct registers{
    char removed[1];
    int linked;
    int idConnect;
    char *country;
    int idPoPsConec;
    char *unitMeas;
    int speed;

    char *namePoPs;
    char *nameCountry;
};
typedef struct registers registers;

struct filter{
    char fieldName[25];
    char value[25];
};
typedef struct filter filter;

struct element{
    registers *reg;
    struct element *next;
};
typedef struct element element;

typedef struct element* List;