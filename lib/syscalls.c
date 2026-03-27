#include "syscalls.h"

void yield(void)
{
    asm volatile("swi 0" ::: "memory");
}
