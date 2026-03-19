#include "../lib/stdio.h"

void main(void)
{
    int n = 0;

    while (1)
    {
        PRINT("----From P1: %d\n", n);

        n++;
        if (n == 10)
        {
            n = 0;
        }

        // Small delay to prevent overwhelming UART
        for (volatile int i = 0; i < 150000000; i++)
            ;
    }
}