#include "../headers/funcOpen.h"


/**
 * @brief Function responsible for opening a binary file for reading from a string containing the file name,
 * and printing an error message if the file cannot be opened.
 * 
 * @param fileName String containing the file name
 * @return FILE* Pointer to the opened file
 */

FILE* openReadBin(char* fileName){
    FILE* file;
    file = fopen(fileName, "rb");

    if(file == NULL){
        printf("Falha na execução da funcionalidade.\n");
        return 0;
    }
    return file;
}

