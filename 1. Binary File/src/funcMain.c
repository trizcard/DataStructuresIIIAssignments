#include "funcMain.h"

void createHead(head *cab){
    cab->status = '0';
    cab->top = -1;
    cab->nextRRN = 0;
    cab->nRegRemov = 0;
    cab->nPagDisk = 1;
    cab->qttCompact = 0;
    cab->trash = malloc(939 * sizeof(char));
    addTrash(cab->trash, 0, 939); 
}

// Add the head to the output file
void addHeadFile(FILE *outFile, head *cab){
    fwrite(&cab->status, sizeof(char), 1, outFile);
    fwrite(&cab->top, sizeof (int), 1, outFile);
    fwrite(&cab->nextRRN, sizeof (int), 1, outFile);
    fwrite(&cab->nRegRemov, sizeof (int), 1, outFile);
    fwrite(&cab->nPagDisk, sizeof (int), 1, outFile);
    fwrite(&cab->qttCompact, sizeof (int), 1, outFile);
    fwrite(cab->trash, sizeof(char), 939, outFile);
}

// Update the head after the registers insertion
void updateHead(FILE* outFile, head *cab, int RRN, int compressQtt){
    cab->status = '1';
    cab->top = -1;
    cab->nextRRN = RRN;
    cab->qttCompact = compressQtt;
    fseek(outFile, 0, SEEK_SET);
    addHeadFile(outFile, cab);
}

// Read the head from the input file
void readHead(FILE *file, head *cab){
    fread(&cab->status, sizeof(char), 1, file);
    fread(&cab->top, sizeof(int), 1, file);
    fread(&cab->nextRRN, sizeof(int), 1, file);
    fread(&cab->nRegRemov, sizeof(int), 1, file);
    fread(&cab->nPagDisk, sizeof(int), 1, file);
    fread(&cab->qttCompact, sizeof(int), 1, file);
    fread(cab->trash, sizeof(char), 939, file);
}

// Inicialize the registers after its creation and set the initial values
void initRegister(registers *reg){
    reg->removed[0] = '0';
    reg->linked = -1;
    reg->nameCountry = (char *) malloc(45 * sizeof(char));
    reg->namePoPs = (char *) malloc(45 * sizeof(char));
    reg->country = (char *) malloc(3 * sizeof(char));
    reg->unitMeas = (char *) malloc(2 * sizeof(char));
    reg->speed = -1;
}

// Allocates registers in memory
void allocateReg(registers *reg){
    reg->nameCountry = (char *) malloc(45 * sizeof(char));
    reg->namePoPs = (char *) malloc(45 * sizeof(char));
    reg->country = (char *) malloc(3 * sizeof(char));
    reg->unitMeas = (char *) malloc(2 * sizeof(char));
}

// Free the registers
void deallocateReg(registers *reg){
    free(reg->nameCountry);
    free(reg->namePoPs);
    free(reg->country);
    free(reg->unitMeas);
}

// Read the registers from the input file
int readRegister(FILE *file, registers *reg){
    fread(reg->removed, sizeof(char), 1, file);
    if (reg->removed[0] == '1'){
        fseek(file, 63, SEEK_CUR);
        return 0;
    }

    fread(&reg->linked, sizeof(int), 1, file);
    fread(&reg->idConnect, sizeof(int), 1, file);
    fread((reg->country), sizeof(char), 2, file);
    reg->country[2] = '\0';
    fread(&reg->idPoPsConec, sizeof(int), 1, file);
    fread((reg->unitMeas), sizeof(char), 1, file);
    reg->unitMeas[1] = '\0';
    fread(&reg->speed, sizeof(int), 1, file);
    
    char string[45];
    fread(string, sizeof(char), 44, file);
    
    readVarField(string, reg->namePoPs, reg->nameCountry);

    return 1;
}

// Read the variable field of the registers
void readVarField(char *string, char *strPoPs, char *strCountry){
    char c = 0;
    int i = 0;
    if (string[0] != '|'){
        do{
            c = string[i];
            strPoPs[i] = c;
            i++;
        } while(string[i] != '|' && i < 44);
    }
    strPoPs[i] = '\0';
    i++;
    int j = i;
    
    if (i < 44){
        c = string[i];
        while(c != '|' && i < 44){
            strCountry[(i-j)] = c;
            i++;
            c = string[i];
        }
    }
    strCountry[i-j] = '\0';
}

// Logical removal the registers from the input file
void removeReg(FILE *file, head *cab){
    fseek(file, -64, SEEK_CUR);
    long position;
    position = ftell(file);

    fwrite("1", sizeof(char), 1, file);
    fwrite(&cab->top, sizeof(int), 1, file);
    fwrite("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$", sizeof(char), 59, file);
    cab->top = (position-960)/64;
    cab->nRegRemov++; 
}

// Print the registers
void printReg(registers *regAux){
    if (regAux->idConnect != 0){
        printf("Identificador do ponto: %d\n", regAux->idConnect);
    }
    if (regAux->namePoPs[0] != '\0'){
        printf("Nome do ponto: %s\n", regAux->namePoPs);
    }
    if (regAux->nameCountry[0] != '\0'){
        printf("Pais de localizacao: %s\n", regAux->nameCountry);
    }
    if (regAux->country[0] != '$'){
        printf("Sigla do pais: %s\n", regAux->country);
    }
    if (regAux->idPoPsConec != -1){
        printf("Identificador do ponto connected: %d\n", regAux->idPoPsConec);
    }
    if (regAux->unitMeas[0] != '$'){
        printf("Velocidade de transmissao: %d %sbps\n", regAux->speed, regAux->unitMeas);
    }
    printf("\n");
}

// Print the registers from a linked list of registers
void printListReg(List *list){
    element *aux;
    aux = *list;
    
    while(aux != NULL){
        printReg(aux->reg);
        aux = aux->next;
    }

    // free the list
    releaseList(list);
}

// Print all the registers from the input file
void printOutput(FILE *file){
    // Create an auxiliary registers and head
    registers regAux;
    allocateReg(&regAux);

    head cabAux;
    cabAux.trash = (char *) malloc(939*sizeof(char));

    readHead(file, &cabAux);

    if (cabAux.status == '0'){
        printf("Falha no processamento do file.\n");
        free(cabAux.trash);
        deallocateReg(&regAux);
        return;
    }
    
    int i = 0;

    if (i == cabAux.nextRRN){
        printf("Registro inexistente.\n\n");
    }
    
    // Scan the entire file
    while(i < cabAux.nextRRN){
        // If the registers is removed, skip it
        if (readRegister(file, &regAux)){
            printReg(&regAux);
        }
        i++;
    }
    printf("Numero de paginas de disco: %d\n\n", cabAux.nPagDisk);

    deallocateReg(&regAux);
    free(cabAux.trash);
}

// Analyze the fields of a registers accolording to the filter
int analyzeField(filter *filters, int i, registers *reg){
    if (!strcmp(filters[i].fieldName, "idConnect")){
        int idConnect;
        idConnect = atoi(filters[i].value);
        if (idConnect == reg->idConnect){
            return 1;
        }
        return 0;
    } else if (!strcmp(filters[i].fieldName, "country")){
        if (!strcmp(filters[i].value, reg->country)){
            return 1;
        }
        return 0;
    } else if (!strcmp(filters[i].fieldName, "idPoPsConnected")){
        int idPoPsConnected;
        idPoPsConnected = atoi(filters[i].value);
        if (idPoPsConnected == reg->idPoPsConec){
            return 1;
        }
        return 0;
    } else if (!strcmp(filters[i].fieldName, "unitMeas")){
        if (!strcmp(filters[i].value, reg->unitMeas)){
            return 1;
        }
        return 0;
    } else if (!strcmp(filters[i].fieldName, "speed")){
        int speed;
        speed = atoi(filters[i].value);
        if (speed == reg->speed){
            return 1;
        }
        return 0;
    } else if (!strcmp(filters[i].fieldName, "namePoPs")){
        if (!strcmp(filters[i].value, reg->namePoPs)){
            return 1;
        }
        return 0;
    } else if (!strcmp(filters[i].fieldName, "nameCountry")){
        if (!strcmp(filters[i].value, reg->nameCountry)){
            return 1;
        }
        return 0;
    } else {
        return 0;
    }
}

// Filter the registers accolording to the inputs
int filterByType(FILE *file, int type){ // type 3 = print, type 4 = remove
    int n;
    scanf("%d", &n);
    
    // Create an auxiliary registers
    registers regAux;
    allocateReg(&regAux);

    // Create an auxiliary head
    head cab;
    cab.trash = (char*) malloc(939*sizeof(char));

    readHead(file, &cab);
    if (cab.status == '0'){
        printf("Falha no processamento do file.\n");
        free(cab.trash);
        deallocateReg(&regAux);
        return 0;
    }

    fseek(file, 0, SEEK_SET);
    char status = '0';
    fwrite(&status, sizeof(char), 1, file);
    fseek(file, 960, SEEK_SET);

    filter filters[n];

    // List of registers for each filter
    List **regFiltereds;
    regFiltereds = (List**) malloc(n * sizeof(List*));

    for(int i = 0; i < n; i++){
        fscanf(stdin, "%s", filters[i].fieldName);
        scanQuoteString(filters[i].value);
        regFiltereds[i] = (List*) malloc(sizeof(List));
    }

    int regRRN = 0;

    // Scan the entire file
    while(regRRN < cab.nextRRN){
        // If the registers is removed, skip it
        if (readRegister(file, &regAux)){
            for (int i = 0; i < n; i++){
                int filtered = 0; // if the registers is filtered
                filtered = analyzeField(filters, i, &regAux);

                if (filtered && type == 3){
                    addListReg((regFiltereds[i]), &regAux);
                }
                else if (type == 4 && filtered){
                    removeReg(file, &cab);
                    break;
                }
            }
        }
        regRRN++;
    }

    for (int i = 0; i < n; i++){
        if (type == 3){
            printf("Busca %d\n", (i+1));
            if (*regFiltereds[i] == NULL){
                printf("Registro inexistente.\n\n");
                free(regFiltereds[i]);
                printf("Numero de paginas de disco: %d\n\n", cab.nPagDisk);
                return 1;
            }else{
                free(regFiltereds[i]);
                printListReg((regFiltereds[i]));
            }
            printf("Numero de paginas de disco: %d\n\n", cab.nPagDisk);
        }
        else{
            free(regFiltereds[i]);
        }
    }

    if (type == 4){
        // Update head
        fseek(file, 1, SEEK_SET);
        fwrite(&(cab.top), sizeof(int), 1, file);
        fseek(file, 9, SEEK_SET);
        fwrite(&cab.nRegRemov, sizeof(int), 1, file);
    }

    
    deallocateReg(&regAux);
    free(regFiltereds);
    free(cab.trash);
    return 1;
}
