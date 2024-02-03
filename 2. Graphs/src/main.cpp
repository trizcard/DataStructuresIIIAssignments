#include "../headers/struct.h"
#include "../headers/functionalities.h"

// Authors
// Beatriz Cardoso de Oliveira NUSP: 12566400
// Participação: 100%
// Beatriz Aimee Teixeira Furtado Braga NUSP: 12547934
// Participação: 100%

int main() {
    
    int func;
    char fileName[SIZE_NAME_FILE];
    
    scanf("%d ", &func); // Selects the function to be executed
    scanf("%s", fileName); // Reads the file name
    
    int n; 

    // Calls the function according to the input
    switch (func) {
        case 11:
            command11(fileName);
            break;
        
       case 12:
            command12(fileName);
            break;
        
        case 13:
            scanf("%d", &n);
            command13(fileName, n);
            break;
       
        case 14:
            scanf("%d", &n);
            command14(fileName, n);
            break;
    }
    return 0;
}