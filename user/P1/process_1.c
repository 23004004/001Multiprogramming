#include "../../lib/stdio.h"

void process_1(void)
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

        //yield();

        // Small delay to prevent overwhelming UART
        for (volatile int i = 0; i < 95000000; i++)
            ;
    }
}