#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "../lib/stdio.h"

#ifdef PLATFORM_VERSATILEPB // ARM926EJ-S

    // VesatilePB base address
    #define MEM_ADDR 0x00000000

#elif defined(PLATFORM_BEAGLEBONE) // ARM Cortex-A8 (AM335x)

    // BeagleBone Black base address
    #define MEM_ADDR 0x82000000

#endif

// ============================================================================
// Process Control Block (PCB)
// ============================================================================

typedef enum
{
    PROCESS_NEW = 0,
    PROCESS_READY = 1,
    PROCESS_RUNNING = 2,
    PROCESS_WAITING = 3,
    PROCESS_SUSPENDED = 4,
    PROCESS_TERMINATED = 5
} ProcessState;

typedef struct
{
    // Process context
    unsigned int regs[13]; // R0 - R12
    unsigned int sp;       // Stack pointer (R13)
    unsigned int lr;       // Link register (R14)
    unsigned int pc;       // Program counter (R15)
    unsigned int spsr;     // Saved Program Status Register

    // Process information
    unsigned int pid;      // Process ID
    ProcessState state;    // NEW, READY, RUNNING, WAITING, SUSPENDED, TERMINATED
} PCB;

// Number of user processes (OS, P1 and P2)
#define NUM_PROCESSES 3

extern PCB pcb[NUM_PROCESSES];

void pcb_init(unsigned int pid);
void setup_process_stack(unsigned int pid);
unsigned int get_process_stack_top(unsigned int pid);
void update_process_state(unsigned int pid, ProcessState new_state);

// ============================================================================
// Scheduler
// ============================================================================

extern unsigned int current_process;
extern unsigned int next_process;
extern unsigned int quantum;

void save_context(void);
void restore_context(void);
void schedule(void);
void schedule_yield(void);

#endif