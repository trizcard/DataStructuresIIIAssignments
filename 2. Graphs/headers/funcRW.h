#ifndef __RW_H__
#define __RW_H__

#include "../headers/struct.h"

// Register reading
void readRegisters(regData*, FILE*);
void readFieldFixed(regData*, FILE*);
void readFieldVariable(regData*, FILE*);

// File reading
int readFile(regData*, FILE*);
void readHeadReg(FILE*, regHead*);

// Register writing
void writeStrFile(char*, FILE*, int);
void writeHeadFile(FILE*, regHead*);

#endif