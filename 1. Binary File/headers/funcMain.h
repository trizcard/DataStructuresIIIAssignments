#include "funcAux.h"

// Head auxiliar functions
void createHead (head *cab);
void addHeadFile (FILE *outFile, head *cab);
void updateHead (FILE* outFile, head *cab, int RRN, int compressQtt);
void readHead (FILE *file, head *cab);

// Registers manipulation
void initRegister (registers *reg);
void allocateReg(registers *reg);
void deallocateReg (registers *reg);
int readRegister (FILE *file, registers *reg);
void readVarField (char *string, char *strPoPs, char *strCountry);

// Registers removal and print
void removeReg (FILE *file, head *cab);
void printReg (registers *regAux);
void printListReg (List *list);

// Verification of removed registers
int checkRemoved (FILE* outFile);

// General functions
void printOutput (FILE *file);
int analyzeField (filter *filters, int i, registers *reg);
int filterByType (FILE *file, int type);