#include "kernel.h"
#include "../lib/stdio.h"

// ============================================================================
// Watchdog Timer Functions
// ============================================================================

// Function to disable the watchdog timer
void watchdog_disable(void)
{
#ifdef PLATFORM_BEAGLEBONE
    // Write 0xAAAA then 0x5555 to WDT1_WSPR, waiting for WDT1_WWPS between writes
    REG(WDT1_WSPR) = 0xAAAA;
    while (REG(WDT1_WWPS) != 0)
        ;

    REG(WDT1_WSPR) = 0x5555;
    while (REG(WDT1_WWPS) != 0)
        ;
#endif
}

// ============================================================================
// Timer Functions
// ============================================================================

// Function to initialize the timer to generate an interrupt every second
void timer_init(void)
{
#ifdef PLATFORM_BEAGLEBONE
    // Enabling the timer clock
    REG(CM_PER_TIMER2_CLKCTRL) = 0x2;

    // Unmasking IRQ 68
    // Each register of INTC_MIR_CLEAR2 belongs to an IRQ
    // There are 3 INTC_MIR, so 96 IRQ
    // bit 0 of INTC_MIR_CLEAR2 -> IRQ 64,
    // bit 1 of INTC_MIR_CLEAR2 -> IRQ 65,
    // bit 2 of INTC_MIR_CLEAR2 -> IRQ 66,
    // bit 3 of INTC_MIR_CLEAR2 -> IRQ 67,
    // bit 4 of INTC_MIR_CLEAR2 -> IRQ 68,
    // etc ...
    REG(INTC_MIR_CLEAR2) = 0x10;

    // Setting interrupt priority to 0x0
    REG(INTC_ILR68) = 0x0;

    // Stopping the timer
    REG(DTIMER2_TCLR) = 0x0;

    // Clearing pending interrupts
    REG(DTIMER2_TISR) = 0x7;

    // Setting the load value for 1 second
    // f: 24 MHz, T: 1 s, f: 1/1 Hz, N = (24MHz)*1 = 24x10^6
    // max. counter: 0xFFFFFFFF, counter = 0xFFFFFFFF - 24x10^6 = 0xFE91C9FF
    REG(DTIMER2_TLDR) = 0xFE91C9FF;

    // Setting the counter to the same value
    REG(DTIMER2_TCRR) = 0xFE91C9FF;

    // Enabling overflow interrupt
    REG(DTIMER2_TIER) = 0x2;

    // Starting the timer in auto-reload mode
    REG(DTIMER2_TCLR) = 0x3;
#endif
}

// Function to handle timer interrupts
void timer_irq_handler(void)
{
#ifdef PLATFORM_BEAGLEBONE
    // Clearing the timer interrupt flag
    REG(DTIMER2_TISR) = 0x2;

    // Acknowledging the interrupt to the controller
    REG(INTC_CONTROL) = 0x1;
#endif

    // Printing "Tick\n" via UART
    PRINT("Tick\n");
}

// ============================================================================
// Process Control Block (PCB) and Scheduler
// ============================================================================

PCB pcb[NUM_PROCESSES];
unsigned int process_count = 0;

// Function to Initialize PCBs
void pcb_init(unsigned int pid)
{
    pcb[pid].pid = pid;
    pcb[pid].pc = MEM_ADDR + pid * 0x100000;           // Entry point
    pcb[pid].sp = MEM_ADDR + pid * 0x100000 + 0x10000; // Stack top (base + 64K)
    pcb[pid].lr = 0x0;
    pcb[pid].spsr = 0x0;
    pcb[pid].state = PROCESS_READY;

    for (int i = 0; i < 13; i++)
        pcb[pid].regs[i] = 0x0;

    // Memory barrier to ensure PCB writes are visible before any context switch
    asm volatile("dsb" ::: "memory");
}

// Function to setup the stack frame of a process
void setup_process_stack(unsigned int pid)
{
    // Reserve a contiguous region for the process stack
    pcb[pid].sp = pcb[pid].sp + 0x2000; // 8 KB stack

    // Building a saved context at the top of the region:
    // Set LR to the process entry point
    pcb[pid].lr = MEM_ADDR + pid * 0x100000;

    // Set the process SP to the address at the lowest word of this frame
    pcb[pid].sp = pcb[pid].sp - 14 * sizeof(unsigned int);

    // Set the PC to the process entry point
    pcb[pid].pc = MEM_ADDR + pid * 0x100000;
}

// Function to create a process
void create_process(unsigned int pid)
{
    pcb_init(pid);
    setup_process_stack(pid);
}

// ============================================================================
// Main Function
// ============================================================================

void os_init(void)
{
    // Welcome message
    PRINT("\n=== 0001Multiprogramming ===\n");
    PRINT(" - Carlos Alvarez - 23004004\n");
    PRINT(" - Gabriel Garcia - 17001171\n");
    PRINT("\nStarting Kernel ...\n\n");

    // Disable the watchdog timer to prevent resets
    PRINT("Disabling watchdog ... ");
    watchdog_disable();
    PRINT("OK\n");

    PRINT("Initializing timer ... ");
    timer_init();
    PRINT("OK\n");

    PRINT("Enabling interrupts ... ");
    enable_irq();
    PRINT("OK\n");

    PRINT("\nKernel started successfully.\n\n");

    PRINT("Starting OS ... ");
    create_process(process_count++); // OS process
    PRINT("OK\n");

    PRINT("Creating user processes ... ");
    create_process(process_count++); // User process 1
    create_process(process_count++); // User process 2
    PRINT("OK\n");
}