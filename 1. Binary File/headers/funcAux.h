#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "head.h"
#include "funcGiven.h"
#define TRASH '$'
#define REG_SIZE 960

// Adds trash in the fields
void addTrash (char *trash, int begin, int size);

// Head auxiliar functions
void updateCompressedSize (FILE* outFile);
int getCompressedSize (FILE *file);

// Manipulation of the fields
void breakStr (char *line, registers *reg);
void checkFieldStr (char *field, int size, int begin);
void addVariableField (FILE *outFile, char *field);
void addValueInt (int *set, int flag, char *value);
void addValueStr (char *set, int flag, char* value);
void scanField (void *field, int *pos, char *line, char *varAux, int integer);

// Entry and reading of the registers
void inputRegister (registers *reg);
void addRegOutFile (FILE *outFile, registers *reg);
void addTrashFixed (registers *reg);
void addListReg (List *list, registers *regOrig);
void releaseList (List* li);
void passReg (registers *reg, registers *regOrig);

// Entry manipulation
void convertInt (int *field, int flagNull, char *value);
void checkNull (char* field, void *fieldReg, int flagInt);

// Verify file
int checkRemoved (FILE* outFile);

// Update file
void updateNRegRemoved (FILE* outFile);
void updateTop (FILE* outFile, int RRN);
void updateRRN (FILE* outFile, int RRN);
void updateNPagDisk (FILE* outFile, int RRN);

// Get file information
int getNRegRemoved (FILE* outFile);
int getLinkedRRN (FILE* outFile, int RRN);
int getRRN (FILE* outFile);