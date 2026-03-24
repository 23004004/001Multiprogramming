#include "../../lib/stdio.h"

void process_2(void)
{
    char c = 'a';

    while (1)
    {
        PRINT("----From P2: %c\n", c);

        c++;
        if (c > 'z')
        {
            c = 'a';
        }

        // Small delay to prevent overwhelming UART
        for (volatile int i = 0; i < 150000000; i++)
            ;
    }
}