#include "scheduler.h"

// ============================================================================
// Process Control Block (PCB)
// ============================================================================

PCB pcb[NUM_PROCESSES];

// Function to Initialize PCBs
void pcb_init(unsigned int pid)
{
    pcb[pid].pid = pid;
    pcb[pid].pc = MEM_ADDR + pid * 0x100000;           // Entry point
    pcb[pid].sp = MEM_ADDR + pid * 0x100000 + 0x10000; // Stack top (base + 64K)
    pcb[pid].lr = MEM_ADDR + pid * 0x100000;           // Entry point
    pcb[pid].spsr = 0x13;                              // SVC mode, IRQs enabled

    for (int i = 0; i < 13; i++)
        pcb[pid].regs[i] = 0x0;

    update_process_state(pid, PROCESS_NEW);

    // Memory barrier to ensure PCB writes are visible before any context switch
    asm volatile("dsb" ::: "memory");
}

// Function to setup the stack frame of a process
void setup_process_stack(unsigned int pid)
{
    // Building a saved context at the top of the region:
    // Set LR to the process entry point
    pcb[pid].lr = MEM_ADDR + pid * 0x100000;

    // Set the process SP to the address at the lowest word of this frame
    pcb[pid].sp = MEM_ADDR + pid * 0x100000 + 0x10000 - 14 * sizeof(unsigned int);

    // Set the PC to the process entry point
    pcb[pid].pc = MEM_ADDR + pid * 0x100000;
}

// Function to update process state (NEW, READY, RUNNING, WAITING, SUSPENDED, TERMINATED)
void update_process_state(unsigned int pid, ProcessState new_state)
{
    pcb[pid].state = new_state;
}

// ============================================================================
// Scheduler
// ============================================================================

unsigned int current_process = 0;
unsigned int next_process = 0;


// Fuction to choose the next process to run (round-robin scheduler)
void schedule(void)
{
    if (current_process == 1)
    {
        next_process = 2;
    }
    else
    {
        next_process = 1;
    }

    // Only switch to the next process if it's READY or WAITING
    if (pcb[next_process].state == PROCESS_READY || pcb[next_process].state == PROCESS_WAITING)
    {
        update_process_state(current_process, PROCESS_WAITING);
        current_process = next_process;
        update_process_state(current_process, PROCESS_RUNNING);
    }
}
