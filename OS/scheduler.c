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

// Function to save the context of the current process
void save_context(void)
{
    update_process_state(current_process, PROCESS_WAITING);
    asm volatile(
        "stm %0, {r0-r12} \n"  // Save R0-R12
        "str sp, [%0, #52] \n" // Save SP (R13)
        "str lr, [%0, #56] \n" // Save LR (R14)
        "mov r1, lr \n"        // Save PC (R15)
        "subs r1, r1, #4 \n"   // PC = LR_irq - 4
        "str r1, [%0, #60] \n"
        "mrs r2, SPSR \n" // Save SPSR
        "str r2, [%0, #64] \n"
        "dsb \n" // Data Synchronization Barrier
        :
        : "r"(&pcb[current_process])
        : "memory");
}

// Function to restore the context of a process
void restore_context(void)
{
    update_process_state(current_process, PROCESS_RUNNING);
    asm volatile(
        "ldr r1, [%0, #64] \n" // Restore SPSR
        "msr SPSR, r1 \n"
        "ldr r1, [%0, #60] \n" // Restore LR (R14)
        "adds lr, r1, #4 \n"   // LR_irq = saved_PC + 4
        "ldm %0, {r0-r12} \n"  // Restore R0-R12
        "subs pc, lr, #4 \n"   // Running the other process
        :
        : "r"(&pcb[current_process])
        : "memory");
}

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
        current_process = next_process;
        update_process_state(current_process, PROCESS_READY);
    }
}
