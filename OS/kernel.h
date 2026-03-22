#ifndef KERNEL_H
#define KERNEL_H

// Macro to access memory-mapped registers
#define REG(x) (*(volatile unsigned int * const)(x))

// Uncomment the platform you are targeting
// #define PLATFORM_VERSATILEPB
// TODO:
// - Update OS dynamically based on the platform
// - Update linkers dynamically based on the platform
// #define PLATFORM_BEAGLEBONE
// NOTE: Ensure to comment both of them when compiling

#ifdef PLATFORM_VERSATILEPB // ARM926EJ-S

    // VesatilePB base address
    #define MEM_ADDR 0x00000000

#elif defined(PLATFORM_BEAGLEBONE) // ARM Cortex-A8 (AM335x)

    // BeagleBone Black base address
    #define MEM_ADDR 0x82000000

    // BeagleBone Black watchdog timer base address
    #define WDT1_BASE 0x44E35000
    #define WDT1_WWPS (WDT1_BASE + 0x34) // Watchdog Write Posted Status Register
    #define WDT1_WSPR (WDT1_BASE + 0x48) // Watchdog Start/Stop Register

    // BeagleBone Black DMTIMER2 base address
    #define DMTIMER2_BASE 0x48040000
    #define DTIMER2_TISR (DMTIMER2_BASE + 0x28) // Timer Interrupt Status Register (IRQSTATUS)
    #define DTIMER2_TIER (DMTIMER2_BASE + 0x2C) // Timer Interrupt Enable Register (IRQSTATUS_SET)
    #define DTIMER2_TCLR (DMTIMER2_BASE + 0x38) // Timer Control Register (TCLR)
    #define DTIMER2_TCRR (DMTIMER2_BASE + 0x3C) // Timer Counter Register (TCRR)
    #define DTIMER2_TLDR (DMTIMER2_BASE + 0x40) // Timer Load Register (TLDR)

    // BeagleBone Black Interrupt Controller (INTCPS) base address
    #define INTCPS_BASE 0x48200000
    #define INTC_MIR_CLEAR2 (INTCPS_BASE + 0xC8) // Interrupt Mask Clear Register 2
    #define INTC_CONTROL (INTCPS_BASE + 0x48)    // Interrupt Controller Control
    #define INTC_ILR68 (INTCPS_BASE + 0x110)     // Interrupt Line Register 68 (INTC_ILR4)

    // Clock Manager base address
    #define CM_PER_BASE 0x44E00000
    #define CM_PER_TIMER2_CLKCTRL (CM_PER_BASE + 0x80) // Timer2 Clock Control

#endif

// ============================================================================
// Timer Functions
// ============================================================================

void enable_irq(void);

// ============================================================================
// Process Control Block (PCB) and Scheduler
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
    unsigned int pid;      // Process ID (0 = OS, 1 = P1, 2 = P2)
    unsigned int regs[13]; // R0 - R12
    unsigned int sp;       // Stack pointer (R13)
    unsigned int lr;       // Link register (R14)
    unsigned int pc;       // Program counter (R15)
    unsigned int spsr;     // Saved Program Status Register
    ProcessState state;    // NEW, READY, RUNNING, WAITING, SUSPENDED, TERMINATED
} PCB;

// Number of user processes (OS, P1 and P2)
#define NUM_PROCESSES 3

extern PCB pcb[NUM_PROCESSES];
extern unsigned int process_count;
extern unsigned int current_process;
extern unsigned int next_process;

void update_process_state(unsigned int pid, ProcessState new_state);
void save_context(unsigned int pid);
void schedule(void);
void restore_context(unsigned int pid);
void context_switch(void);

void print_pcb(void);

#endif