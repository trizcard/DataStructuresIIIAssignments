#include "../headers/funcCheck.h"


/**
 * @brief Function responsible for checking the consistency of a file and
 * returning an error message if the file is inconsistent.
 *
 * @param reg Pointer to the header record
 * @return int Returns 1 if the file is inconsistent, 0 otherwise.
 */


int checkConsistency(regHead* reg){
    if(reg->status[0] != '1'){ // If the status is not equal to 1
        printf("Falha na execução da funcionalidade.\n");
        return 1;
    }
    return 0;
}

