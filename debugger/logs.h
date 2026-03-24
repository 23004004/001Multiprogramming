#ifndef LOGS_H
#define LOGS_H

#include "../lib/stdio.h"
#include "../OS/kernel.h"

// ============================================================================
// Debugging Functions
// ============================================================================

#define NUM_REGISTERS 18
unsigned int regs[NUM_REGISTERS];

void print_pcb(void);
void print_registers(void);

#endif