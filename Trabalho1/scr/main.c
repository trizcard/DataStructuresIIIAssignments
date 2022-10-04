#include <stdio.h>

int main() {
    int func;
    scanf("%d", &func);

    switch (func) {
    case 1:
        funcUM();
        break;
    
    case 2:
        funcDOIS();
        break;
    
    case 3:
        funcTRES();
        break;
    
    case 4:
        funcQUATRO();
        break;

    case 5:
        funcCINCO();
        break;
    
    case 6:
        funcSEIS();
        break;
    }
}