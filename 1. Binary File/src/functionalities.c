#include "functionalities.h"

void funcONE(char fileName[25], char fileName2[25]){
    // Open file and check if it worked
    FILE *inputFile;
    inputFile = fopen(fileName, "r");
    if(inputFile == NULL){
        printf("Falha no processamento do file.\n");
        return;
    }

    // Open output file and check if it worked
    FILE *outFile;
    outFile = fopen(fileName2, "wb");

    // Create head
    head cab;
    createHead(&cab);   

    // Add head to output file
    addHeadFile(outFile, &cab);

    int qttByte= 960;
    int qttPagDisco = 1;

    int RRN = 0;


    char *line = (char*) malloc(200 * sizeof(char));

    // Skip the first line
    fgets(line, 200, inputFile);

    // Read the registers from the input file
    while(fgets(line, 200, inputFile)){
        // Create registers
        registers reg;
        
        // Initialize registers
        initRegister(&reg);
    
        // Get the fields and call the function to add the values to the registers
        breakStr(line, &reg);
        
        // Check if the registers fits in the disk page
        if(qttByte + 64 > REG_SIZE){
            qttPagDisco++;
            qttByte = 64;       
        }else{
            qttByte += 64; 
        }
        
        // Add the registers to the output file
        addRegOutFile(outFile,&reg);
        RRN++;

        // Deallocate registers
        deallocateReg(&reg);
    }

    // Deallocate line
    free(line);
    
    // Update the head
    cab.nPagDisk = qttPagDisco;
    updateHead(outFile, &cab, RRN, 0);
    
    // Free the head trash
    free(cab.trash);

    // Close the files
    fclose(inputFile);
    fclose(outFile);

    // Print the result
    binScreen(fileName2);
}

void funcTWO(char fileName[25]){
    // Open file and check if it worked
    FILE *file = NULL;
    file = fopen(fileName, "rb");

    if (file == NULL){
        printf("Falha no processamento do file.\n");
        return;
    }
    
    printOutput(file);
    fclose(file);
}

void funcTHREE(char fileName[25]){
    // Open file and check if it worked
    FILE *file = NULL;
    file = fopen(fileName, "rb");

    if (file == NULL){
        printf("Falha no processamento do file.\n");
        return;
    }

    filterByType(file, 3);
    fclose(file);
}

void funcFOUR(char fileName[25]){
    // Open file and check if it worked
    FILE *file = NULL;
    file = fopen(fileName, "rb+");
    int flag;

    if (file == NULL){
        printf("Falha no processamento do file.\n");
        return;
    }

    flag = filterByType(file, 4);
    fseek(file, 0, SEEK_SET);
    char status = '1';
    fwrite(&status, sizeof(char), 1, file);
    fclose(file);
    if (flag){
        binScreen(fileName);
    }
}

void funcFIVE(char fileName[25], int n){
    // Open file and check if it worked
    FILE *inputFile;
    inputFile = fopen(fileName, "rb+");
    if(inputFile == NULL){
        printf("Falha no carregamento do file.\n");
        return;
    }

    // Input the registers
    for(int i = 0; i < n; i++){
        // Create registers
        registers reg;

        // Initialize registers
        initRegister(&reg);

        // Input fields
        inputRegister(&reg);

        // Add trash to the registers
        addTrashFixed(&reg);

        // Check if there are any registers removed
        int RRN = checkRemoved(inputFile);

        // If there are, add the registers in the place of the removed registers
        if(RRN != -1){
            // Get the RRN of the removed registers to add to the stack (top)
            int RRNRemoved = getLinkedRRN(inputFile, RRN);

            // Add the registers to the output file
            fseek(inputFile, (960 + (RRN * 64)), SEEK_SET);
            addRegOutFile(inputFile, &reg);

            // Update the linked of the removed registers
            updateTop(inputFile,RRNRemoved);

            // Update the number of removed registers
            updateNRegRemoved(inputFile);
        
        }
        else{ // If there are no removed registers, add the registers to the end of the file
            RRN = getRRN(inputFile);
            
            fseek(inputFile, (960 + (RRN * 64)), SEEK_SET);
            addRegOutFile(inputFile, &reg);

            // Update the nextRRN
            updateRRN(inputFile, RRN);

            // Update the number of disk pages
            updateNPagDisk(inputFile, RRN);
        }

        // Free the registers
        deallocateReg(&reg);
    }
    
    // Close the file
    fclose(inputFile);

    // Print the result
    binScreen(fileName); 
}

void funcSIX(char fileName[25]){
    // Open file and check if it worked
    FILE *inputFile;
    inputFile = fopen(fileName, "rb+");
    if(inputFile == NULL){
        printf("Falha no carregamento do file.\n");
        return;
    }

    // Check if there are any inconsistencies
    char status;
    fread(&status, sizeof(char), 1, inputFile);
    if(status == '0'){
        printf("Falha no processamento do file.\n");
        fclose(inputFile);
        return;
    }

    // Check quantity of registers
    fseek(inputFile, 5, SEEK_SET);
    int qttRRN;
    fread(&qttRRN, sizeof(int), 1, inputFile);

    // Check if there are any registers removed
    int nRegRemoved = getNRegRemoved(inputFile);

    if (nRegRemoved != 0){ 
        // Create a new file
        FILE *outFile;
        outFile = fopen("file.bin", "wb");
        if (outFile == NULL){
            printf("Falha no carregamento do file.\n");
            return;
        }

        // Copy the head to the new file
        head cab;
        createHead(&cab);

        // Add head to output file
        addHeadFile(outFile, &cab);

        // Page counter
        int qttByte = 0;
        cab.nPagDisk++;

        int RRN = 0;
        int RRNatual = 0;

        // Skip the header
        fseek(inputFile, 960, SEEK_SET);

        // Copy the registers to the new file
        while(RRNatual < qttRRN){
            // Create registers
            registers reg;

            // Initialize registers
            initRegister(&reg);

            // Read the registers from the input file
            if(readRegister(inputFile, &reg)){
                // Check if the registers fits in the disk page
                if(qttByte + 64 > REG_SIZE){
                    cab.nPagDisk++;
                    qttByte = 64;       
                }else{
                    qttByte += 64; 
                }

                // Add the registers to the output file
                addRegOutFile(outFile,&reg);
                RRN++;
            }

            // Deallocate registers  
            deallocateReg(&reg);

            RRNatual++;
        }
       
        // Update the head
        int quantidadeCompressda = getCompressedSize(inputFile);
        quantidadeCompressda++;
        updateHead(outFile, &cab, RRN, quantidadeCompressda);

        // Free the head trash
        free(cab.trash);
        
        // Close the files
        fclose(inputFile);
        fclose(outFile);
        
        // Remove the old file and rename the new file
        remove(fileName);
        rename("file.bin", fileName);
       
    }
    else{
        updateCompressedSize(inputFile);
        // Close the file
        fclose(inputFile);
    }

    // Print the result
    binScreen(fileName);  
}