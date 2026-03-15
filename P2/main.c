#include "../lib/stdio.h"

void main(void){

    char i = 'a';

    while (1) {
        PRINT("----From P2: %c\n", i);

        i++;
        if (i > 'z') {
            i = 'a';
        }
    }
    
}