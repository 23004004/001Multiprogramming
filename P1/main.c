#include "../lib/stdio.h"

void main(void)
{
    int i = 0;

    while (1)
    {
        PRINT("----From P1: %d\n", i);

        i++;
        if (i == 10)
        {
            i = 0;
        }

        // Small delay to prevent overwhelming UART
        for (volatile int i = 0; i < 150000000; i++)
            ;
    }
}