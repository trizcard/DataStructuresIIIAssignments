#include <stdio.h>
#include "functionalities.h"

// Author - University of São Paulo 
// Beatriz Cardoso de Oliveira NUSP: 12566400

int main() {
    int func;
    char fileName[25];
    
    scanf("%d ", &func); // Select the function to be executed
    fscanf(stdin, "%s", fileName); // File name
    char fileName2[25];
    int n;

    // Call the function to be executed
    switch (func) {
        case 1:
            fscanf(stdin, "%s", fileName2); // File name
            funcONE(fileName, fileName2);
            break;
        
        case 2:
            funcTWO(fileName);
            break;
        
        case 3:
            funcTHREE(fileName);
            break;
        
        case 4:
            funcFOUR(fileName);
            break;

        case 5:
            scanf("%d", &n);
            funcFIVE(fileName, n);
            break;
        
        case 6:
            funcSIX(fileName);
            break;
    }
}