#include "../lib/stdio.h"

void main(void) {
    int i = 0;

    while (1) {
        PRINT("%s %d\n", "----From P1:", i);

        i++;
        if (i == 10) {
            i = 0;
        }
    }
}