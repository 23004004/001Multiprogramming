#include "scheduler.h"

// ============================================================================
// Process Control Block (PCB)
// ============================================================================

PCB pcb[NUM_PROCESSES];

static unsigned int get_process_stack_top(unsigned int pid)
{
    unsigned int stack_top = MEM_ADDR + pid * 0x100000 + 0x10000;

    // PID 0 runs inside the OS image, so keep its process stack below the
    // exception stacks used by IRQ/SVC handlers at the top of the OS region.
    if (pid == 0)
    {
        stack_top -= 0x2000;
    }

    return stack_top;
}

static unsigned int select_next_process(void)
{
    return (current_process + 1) % NUM_PROCESSES;
}

static int is_runnable_process(unsigned int pid)
{
    return pcb[pid].state == PROCESS_READY ||
           pcb[pid].state == PROCESS_WAITING ||
           pcb[pid].state == PROCESS_RUNNING;
}

// Function to Initialize PCBs
void pcb_init(unsigned int pid)
{
    unsigned int stack_top = get_process_stack_top(pid);

    pcb[pid].pid = pid;
    pcb[pid].pc = MEM_ADDR + pid * 0x100000;           // Entry point
    pcb[pid].sp = stack_top;                           // Stack top
    pcb[pid].lr = MEM_ADDR + pid * 0x100000;           // Entry point
    pcb[pid].spsr = 0x1F;                              // System mode, IRQs enabled

    for (int i = 0; i < 13; i++)
        pcb[pid].regs[i] = 0x0;

    update_process_state(pid, PROCESS_NEW);

    // Memory barrier to ensure PCB writes are visible before any context switch
    asm volatile("dsb" ::: "memory");
}

// Function to setup the stack frame of a process
void setup_process_stack(unsigned int pid)
{
    unsigned int stack_top = get_process_stack_top(pid);

    // Building a saved context at the top of the region:
    // Set LR to the process entry point
    pcb[pid].lr = MEM_ADDR + pid * 0x100000;

    // Set the process SP to the address at the lowest word of this frame
    pcb[pid].sp = stack_top - 14 * sizeof(unsigned int);

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
unsigned int quantum = 10;

// Fuction to choose the next process to run (round-robin scheduler)
void schedule(void)
{
    if (quantum == 0)
    {
        PRINT("...\n");
        next_process = select_next_process();

        // Only switch to the next process if it's READY or WAITING
        if (is_runnable_process(next_process))
        {
            update_process_state(current_process, PROCESS_WAITING);
            current_process = next_process;
            update_process_state(current_process, PROCESS_RUNNING);
            quantum = 10;
        }
    }
    else
    {
        quantum--;
    }
}

// Function to voluntarily yield the CPU to the other user process
void schedule_yield(void)
{
    PRINT(".\n");
    next_process = select_next_process();

    if (is_runnable_process(next_process))
    {
        update_process_state(current_process, PROCESS_READY);
        current_process = next_process;
        update_process_state(current_process, PROCESS_RUNNING);
    }
    else
    {
        update_process_state(current_process, PROCESS_RUNNING);
    }

    quantum = 10;
}
