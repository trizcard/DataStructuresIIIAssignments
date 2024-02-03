#include "../headers/struct.h"

/**
 * @brief Function responsible for allocating space for the data register and initializing its values.
 * 
 * @return regData* Pointer to the allocated data register
 */

regData* createRegData(){

    regData* newReg;
    newReg = (regData*)malloc(sizeof(regData));

    if(newReg == NULL){
        return NULL;
    }

    strcpy(newReg->removed, "0");
    newReg->linked = -1;

    return newReg;
}

/**
 * @brief Function responsible for allocating space for the header register and initializing its values.
 * 
 * @return regHead* Pointer to the allocated header register
 */

regHead* createRegHead(){

    regHead* newReg;
    newReg = (regHead*)malloc(sizeof(regHead));

    if(newReg == NULL){
        return NULL;
    }

    strcpy(newReg->status, "0");
    newReg->top = -1;
    newReg->nextRRN = 0;
    newReg->nRegRem = 0;
    newReg->nRegDisk = 0;
    newReg->qttCompress = 0;

    return newReg;
}