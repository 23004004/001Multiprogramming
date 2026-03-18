#include "../lib/stdio.h"

void main(void)
{
    char i = 'a';

    while (1)
    {
        PRINT("----From P2: %c\n", i);

        i++;
        if (i > 'z')
        {
            i = 'a';
        }

        // Small delay to prevent overwhelming UART
        for (volatile int i = 0; i < 150000000; i++)
            ;
    }
}