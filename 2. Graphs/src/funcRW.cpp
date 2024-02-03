#include "../headers/funcRW.h"
#include "../headers/funcGiven.h"

/**
 * @brief Function responsible for reading the header record from a binary file and placing it into a struct.
 * 
 * @param file Pointer to the binary file
 * @param reg Pointer to the struct that will receive the header record data
 */

void readHeadReg(FILE* file, regHead* reg){
    fread(reg->status, sizeof(char), 1, file);
    reg->status[1] = '\0';
    fread(&reg->top, sizeof(int), 1, file);
    fread(&reg->nextRRN, sizeof(int), 1, file); // Reads the next RRN
    fread(&reg->nRegRem, sizeof(int), 1, file); // Reads the number of removed registers
    fread(&reg->nRegDisk, sizeof(int), 1, file); // Reads the number of registers in the file
    fread(&reg->qttCompress, sizeof(int), 1, file); // Reads the number of compressions
    fseek(file, (SIZE_PAGE_DISK - SIZE_REG_HEAD), SEEK_CUR); // Skips the trash
}

/**
 * @brief Function responsible for reading all the fields of the record.
 * 
 * @param reg Pointer to the data record
 * @param inputFile Pointer to the binary file
 */

void readRegisters(regData* reg, FILE* inputFile){
  readFieldFixed(reg,inputFile);
  readFieldVariable(reg,inputFile);
}

/**
 * @brief Function responsible for reading the fixed-size fields of a data record.
 * 
 * @param reg Pointer to the data record
 * @param inputFile Pointer to the binary file
 */

void readFieldFixed(regData* reg, FILE* inputFile){

    fread(&reg->linked, sizeof(int), 1, inputFile);
    fread(&reg->idConnect,sizeof(int), 1, inputFile);
    fread(reg->country,sizeof(char)*2, 1, inputFile);
    reg->country[2]='\0';
    fread(&reg->idPoPsConnected,sizeof(int), 1, inputFile);
    fread(reg->unitMeas,sizeof(char), 1, inputFile);
    reg->unitMeas[1]='\0';
    fread(&reg->speed,sizeof(int), 1, inputFile);

}

/**
 * @brief Function responsible for reading the variable-size fields of a data record.
 * 
 * @param reg Pointer to the data record
 * @param inputFile Pointer to the binary file
 */

void readFieldVariable(regData* reg, FILE* inputFile){

    char buffer = '0'; 
    int pos_namePoPs = 0; // Position in the namePoPs string
    int pos_nameCountry = 0; // Position in the nameCountry string
    int size_trash; // Size of the trash
    
    // Reads the variable-size fields
    while (buffer !='|' && buffer != '$') // while it doesn't reach the | (field ended) or the $ (record was removed)
    {
        fread(&buffer, sizeof(char), 1, inputFile);  

        reg->namePoPs[pos_namePoPs] = buffer; // Builds the string
        pos_namePoPs += 1; // Updates the position
                
    }
        
    buffer = '0'; // Reset the buffer

    while (buffer !='|' && buffer != '$') // while it doesn't reach the | (field ended) or the $ (record was removed)
    {
        fread(&buffer, sizeof(char), 1, inputFile); 
                
        reg->nameCountry[pos_nameCountry] = buffer; // Builds the string 
        pos_nameCountry += 1; // Updates the position
    }
             
    reg->namePoPs[pos_namePoPs-1] = '\0'; // Replace the last character by \0
    reg->nameCountry[pos_nameCountry-1] = '\0';

    size_trash = SIZE_REG_DATA-pos_namePoPs-pos_nameCountry-20; // Calculates the size of the trash
         
    if (size_trash != 0) {  // If the trash is not empty
        char trash[size_trash];  // Creates a trash
        fread(trash,sizeof(char)*size_trash, 1, inputFile); // Reads the trash
    }
}

/**
 * @brief Function responsible for reading the file until the end.
 * 
 * @param reg Pointer to the data record
 * @param inputFile Pointer to the binary file
 * @return int Returns 1 if the file has not reached the end, 0 otherwise
 */

int readFile(regData* reg, FILE* inputFile){
    if(fread(reg->removed,sizeof(char), 1, inputFile) != 0){ // Reads the removed field
        reg->removed[1] = '\0'; 
        readRegisters(reg,inputFile);
        return 1; 
    }
    return 0; 
}


/**
 * @brief Function responsible for writing a string using fwrite.
 * 
 * @param str String to be written
 * @param file Pointer to the binary file
 * @param size Size of the string
 */

void writeStrFile(char* str, FILE* file, int size){
    fwrite(str, sizeof(char), size, file);
}

/**
 * @brief Function responsible for writing the header record struct data to the file.
 * 
 * @param file Pointer to the binary file
 * @param reg Pointer to the header record
 */

void writeHeadFile(FILE* file, regHead* reg){
    writeStrFile(reg->status, file, 1);
    fwrite(&reg->top, sizeof(int), 1, file);
    fwrite(&reg->nextRRN, sizeof(int), 1, file);
    fwrite(&reg->nRegRem, sizeof(int), 1, file);
    fwrite(&reg->nRegDisk, sizeof(int), 1, file);
    fwrite(&reg->qttCompress, sizeof(int), 1, file);
    
    for(int i = 0; i < (SIZE_PAGE_DISK - SIZE_REG_HEAD); i++){ // Writes the trash
        fwrite(TRASH, sizeof(char), 1, file);
    }
}

