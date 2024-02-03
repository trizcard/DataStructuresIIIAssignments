#ifndef __STRUCT_H__
#define __STRUCT_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*

Definition of constants that will be used throughout the program:

SIZE_PAGE_DISK -> defines the size of a disk page in bytes (960).
SIZE_MAX_VARIABLE -> defines the maximum size of variable-length fields in bytes (32).
SIZE_REG_DATA -> defines the size of a data record in bytes (64).
TAM_REG_CABECALHO -> defines the size of a header record in bytes (21).
SIZE_NAME_FILE -> defines a maximum size for a file name in bytes (20)..

*/
#define SIZE_PAGE_DISK 960
#define SIZE_MAX_VARIABLE 32
#define SIZE_REG_DATA 64
#define SIZE_REG_HEAD 21
#define SIZE_NAME_FILE 20
#define TRASH "$"

/*

Definition of a data record struct, with the fields defined according to specifications.
The fields "removed", "country", and "unitMeas" are initialized with 1 byte of space
more than necessary, for storing the '\0' character, for internal program control. Additionally,
the fields "namePoPs" and "nameCountry", of variable size, are statically allocated with a fixed
size of 32 bytes for internal program control.

*/
typedef struct regData{

    char removed[2];
    int linked;
    int idConnect;
    char country[3];
    int idPoPsConnected;
    char unitMeas[2];
    int speed;
    char namePoPs[SIZE_MAX_VARIABLE];
    char nameCountry[SIZE_MAX_VARIABLE];
  
}regData;

/*

Definition of a header record struct, with the fields defined according to specifications.
The field "status", which contains only one character that can be '0' or '1', is initialized
with 2 bytes for storing the '\0' character, for internal program control.
*/
typedef struct regHead{

    char status[2];
    int top;
    int nextRRN;
    int nRegRem;
    int nRegDisk;
    int qttCompress;
  
}regHead;

regHead* createRegHead();
regData* createRegData();

#endif