#include "funcAux.h"

void addTrash(char *trash, int begin, int size){
    for (int i = begin; i < (begin + size); i++){
        trash[i] = TRASH;
    }
}

// Update the compressed size in the head file
void updateCompressedSize(FILE* outFile){
    int compressAmount = getCompressedSize(outFile);
    compressAmount++;
    fseek(outFile, 17, SEEK_SET);
    fwrite(&compressAmount, sizeof(int), 1, outFile);
}	

// Read the amount of compressions of the file
int getCompressedSize(FILE *file){
    int compressedAmount;
    fseek(file, 17, SEEK_SET);
    fread(&compressedAmount, sizeof(int), 1, file);
    return compressedAmount;
}

// Break the string 
void breakStr(char *line, registers *reg){
    // 7 fields, so 6 commas  
    int i = 0;
    int amountCommas = 0;
    
    // Scan the line and add the fields in the registers
    while(line[i] != '\n'){
        if(amountCommas == 0){
            // String aux to save the idConnect
            char *idConnect = (char*) malloc(10 * sizeof(char));
            scanField(&reg->idConnect, &i, line, idConnect, 1);
            free(idConnect);
            amountCommas++;
        }
        else if(amountCommas == 1){ // Field namePoPs
            // String aux to save the namePoPs
            char *namePoPs = (char*) malloc(44 * sizeof(char));
            scanField(reg->namePoPs, &i, line, namePoPs, 0);
            free(namePoPs);
            amountCommas++;
        }
        else if(amountCommas == 2){
            // String aux to save the nameCountry
            char *nameCountry = (char*) malloc(44 * sizeof(char));
            scanField(reg->nameCountry, &i, line, nameCountry, 0);
            free(nameCountry);
            amountCommas++;         
        }
        else if(amountCommas == 3){
            // String aux to save the country
            char *country = (char*) malloc(3 * sizeof(char));
            scanField(reg->country, &i, line, country, 0);
            free(country);
            checkFieldStr(reg->country, 2, 0);
            amountCommas++;
        }
        else if(amountCommas == 4){
            // String aux to save the idPoPsConec
            char *idPoPsConec = (char*) malloc(10 * sizeof(char));
            scanField(&reg->idPoPsConec, &i, line, idPoPsConec, 1);
            free(idPoPsConec);
            amountCommas++; 
        }
        else if(amountCommas == 5){
            // String aux to save the unitMeas
            char *unitMeas = (char*) malloc(2 * sizeof(char));
            scanField(reg->unitMeas, &i, line, unitMeas, 0);
            free(unitMeas);
            checkFieldStr(reg->unitMeas, 1, 0);
            amountCommas++;         
        }
        else if(amountCommas == 6){
            // String aux to save the speed
            char *speed = (char*) malloc(10 * sizeof(char));
            scanField(&reg->speed, &i, line, speed, 1);
            free(speed);
            amountCommas++;
        }
        i++;
    }  
}

// Check if the fixed size field exists and add the trash if it has not been filled
void checkFieldStr(char *field, int size, int begin){
    if (!strcmp(field, "")){
        addTrash(field, begin, size);
    }   
}

// Add the variable size field in the output file
void addVariableField(FILE *outFile, char *field){
    if(strcmp(field, "")){
        fwrite(field, sizeof(char) , strlen(field), outFile);
    }
    fwrite("|", sizeof(char), 1, outFile);
}

// Add the value of the field Int in the registers
void addValueInt(int *set, int flag, char *value){
    if(flag){
        *set = atoi(value);
    }else{
        *set = -1;
    }
}

// Add the value of the field String in the registers
void addValueStr(char *set, int flag, char* value){
    if(flag){
        strcpy(set, value);
    }else{
        strcpy(set, "");
    }
}

// Scan the field of the input line and add in the registers
void scanField(void *field, int *pos, char *line, char *varAux, int integer){
    int j = 0;
            
    // Synalize if the field has a value
    int flag = 0;

    // Scan the field until find the separator
    while(line[*pos] != ',' && line[*pos] != '\0'){
        varAux[j] = line[*pos];
        if(!flag){
        flag = 1;
        }
        *pos += 1;
        j++;
    }

    if(varAux[j-1] == ' '){
        varAux[j-1] = '\0';
    }else{
        varAux[j] = '\0';
    }

    if(integer){
        addValueInt(field, flag, varAux);
    }else{
        addValueStr(field, flag, varAux);
    }
}

// Get the input of the registers
void inputRegister(registers *reg){
    // Get the idConnect
    char *idConnect = (char*) malloc(10 * sizeof(char));
    scanf("%s ",idConnect );
    checkNull(idConnect, &reg->idConnect, 1);
    free(idConnect);

    // Get the nameCountry
    scanQuoteString(reg->nameCountry);
    checkNull(reg->nameCountry, reg->nameCountry, 0);

    // Get the namePoPs
    scanQuoteString(reg->namePoPs);
    checkNull(reg->namePoPs, &reg->namePoPs, 0);

    // Get the country
    scanQuoteString(reg->country);
    checkNull(reg->country,&reg->country, 0);

    // Get the idPoPsConec
    char *idPoPsConec = (char*) malloc(10 * sizeof(char));
    scanf("%s ", idPoPsConec);
    checkNull(idPoPsConec, &reg->idPoPsConec,1);
    free(idPoPsConec);

    // Get the unitMeas
    scanQuoteString(reg->unitMeas);
    checkNull(reg->unitMeas, &reg->unitMeas, 0);

    // Get the speed
    char *speed = (char*) malloc(10 * sizeof(char));
    scanf("%s", speed);
    checkNull(speed, &reg->speed,1);
    free(speed);
}

// Add the registers in the output file
void addRegOutFile(FILE *outFile, registers *reg){
    // Add the fields in the output file

    // Fixed size fields
    fwrite(&reg->removed, sizeof(char), 1, outFile);
    fwrite(&reg->linked, sizeof (int), 1, outFile);
    fwrite(&reg->idConnect, sizeof (int), 1, outFile);
    fwrite(reg->country, sizeof (char), 2, outFile);
    fwrite(&reg->idPoPsConec, sizeof (int), 1, outFile);
    fwrite(reg->unitMeas, sizeof(char), 1, outFile);
    fwrite(&reg->speed, sizeof (int), 1, outFile);
    
    // Variable size fields
    addVariableField(outFile, reg->namePoPs);
    addVariableField(outFile, reg->nameCountry);

    // Add the trash in the end of the registers
    int qttFinalTrash = 64 - (strlen(reg->namePoPs) + strlen(reg->nameCountry) + 22);
    char *trash = malloc(qttFinalTrash * sizeof(char));
    addTrash(trash, 0, qttFinalTrash);
    fwrite(trash, sizeof(char) , qttFinalTrash, outFile);
    free(trash);
}

void addTrashFixed(registers *reg){
    // Check if the fixed size fields are filled
    checkFieldStr(reg->country, 2, 0);
    checkFieldStr(reg->unitMeas, 1, 0);
}

// Implement the linked list of registers
void addListReg(List *list, registers *regOrig){
    element *new;
    new = (element*) malloc(sizeof(element));
    new->reg = (registers*) malloc(sizeof(registers));
    passReg(new->reg, regOrig);
    new->next = NULL;

    if ((*list) == NULL){
        *list = new;
    } else {
        element *aux;
        aux = *list;
        while(aux->next != NULL){
            aux = aux->next;
        }
        aux->next = new;
    }
}

// Release the linked list of registers
void releaseList(List* li){
    if(li != NULL){
        element* no;
        while((*li) != NULL){
            no = *li;
            *li = (*li)->next;
            free(no->reg->nameCountry);
            free(no->reg->namePoPs);
            free(no->reg->unitMeas);
            free(no->reg->country);
            free(no->reg);
            free(no);
        }
        free(li);
    }
}

// Pass the information of the registers origin to the registers destiny
void passReg(registers *reg, registers *regOrig){
    reg->idConnect = regOrig->idConnect;

    reg->country = (char*) malloc(3 * sizeof(char));
    reg->country[0] = regOrig->country[0];
    reg->country[1] = regOrig->country[1];
    reg->country[2] = '\0';

    reg->idPoPsConec = regOrig->idPoPsConec;
    
    reg->unitMeas = (char*) malloc(2 * sizeof(char));
    reg->unitMeas[0] = regOrig->unitMeas[0];
    reg->unitMeas[1] = '\0';

    reg->speed = regOrig->speed;

    reg->nameCountry = (char*) malloc(45 * sizeof(char));
    strcpy(reg->nameCountry, regOrig->nameCountry);
    reg->namePoPs = (char*) malloc(45 * sizeof(char));
    strcpy(reg->namePoPs, regOrig->namePoPs);
}

// Check if the integer field is null, if it is, add -1, if not, add the value
void convertInt(int *field, int flagNull, char *value){
    if(flagNull){
        *field = -1;
    }else{
        *field = atoi(value);
    }
}

// Check if the input is null
void checkNull(char* field, void *fieldReg, int flagInt){
    if(!strcmp(field, "NULO")){
        if(flagInt == 1){
            convertInt(fieldReg, 1, NULL);
        }else{
            strcpy(fieldReg, "");
        }
    }else{
        if(flagInt == 1){
            convertInt(fieldReg, 0, field);
        }
    }
}

// Check if there is any removed registers in the file, get the top
int checkRemoved(FILE* outFile){
    int removed;
    fseek(outFile, 1, SEEK_SET);
    fread (&removed, sizeof(int), 1, outFile);
    return removed;
}

// Update the number of removed registers
void updateNRegRemoved(FILE* outFile){
    // Get the number of removed registers and decrement 1
    int regRemoved = getNRegRemoved(outFile);
    regRemoved--;

    // Update the number of removed registers
    fseek(outFile,9, SEEK_SET);
    fwrite(&regRemoved, sizeof(int), 1, outFile);
}

// Update the linked of the top of the stack
void updateTop(FILE* outFile, int RRN){
    fseek(outFile, 1, SEEK_SET);
    fwrite(&RRN, sizeof(int), 1, outFile);
}

// Update the RRN of the next registers
void updateRRN(FILE* outFile, int RRN){
    int nextRRN = RRN+1;
    fseek(outFile, 5, SEEK_SET);
    fwrite(&nextRRN, sizeof(int), 1, outFile);
}

// Update the number of disk pages
void updateNPagDisk(FILE* outFile, int RRN){
    float qttRegPag = 960/64;
    float nroPagDisk = (RRN/qttRegPag)+1;
    int nroPagDiskInt = ceil(nroPagDisk); 
    fseek(outFile, 13, SEEK_SET);
    fwrite(&nroPagDiskInt, sizeof(int), 1, outFile);
}

// Get the number of removed registers
int getNRegRemoved(FILE* outFile){
    int nRegRemoved;
    fseek(outFile, 9, SEEK_SET);
    fread(&nRegRemoved, sizeof(int), 1, outFile);
    return nRegRemoved;
}

// Get the RRN of the next removed registers
int getLinkedRRN(FILE* outFile, int RRN){
    int linked;
    fseek(outFile, (1 +(960 + (RRN * 64))), SEEK_SET);
    fread(&linked, sizeof(int), 1, outFile);
    return linked;
}

// Get the RRN of the next registers
int getRRN(FILE* outFile){
    int RRN;
    fseek(outFile, 5, SEEK_SET);
    fread(&RRN, sizeof(int), 1, outFile);
    return RRN;
}
