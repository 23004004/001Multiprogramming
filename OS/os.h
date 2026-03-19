#ifndef OS_H
#define OS_H

// Macro to access memory-mapped registers
#define REG(x) (*(volatile unsigned int * const)(x))

// Uncomment the platform you are targeting
//#define PLATFORM_VERSATILEPB
// TODO:
// - Update OS dynamically based on the platform
// - Update linkers dynamically based on the platform
//#define PLATFORM_BEAGLEBONE
// NOTE: Ensure to comment both of them when compiling

#ifdef PLATFORM_VERSATILEPB // ARM926EJ-S
    // VersatilePB UART0 base address
    #define UART0_BASE      0x101F1000
    #define UART0_DR        (UART0_BASE + 0x00) // Data Register
    #define UART0_FR        (UART0_BASE + 0x18) // Flag Register
    #define UART0_FR_TXFE   (1 << 7)            // Transmit FIFO Empty
    #define UART0_FR_RXFE   (1 << 4)            // Receive FIFO Empty

#elif defined(PLATFORM_BEAGLEBONE) // ARM Cortex-A8

    // BeagleBone Black watchdog timer base address
    #define WDT1_BASE       0x44E35000
    #define WDT1_WWPS       (WDT1_BASE + 0x34) // Watchdog Write Posted Status Register
    #define WDT1_WSPR       (WDT1_BASE + 0x48) // Watchdog Start/Stop Register

    // BeagleBone Black UART0 base address
    #define UART0_BASE      0x44E09000
    #define UART0_DR        (UART0_BASE + 0x00) // Data Register (THR/RHR)
    #define UART0_FR        (UART0_BASE + 0x14) // Flag Register (UART LSR)
    #define UART0_FR_TXFE   (1 << 5)            // Transmit FIFO Empty (LSR_TXFIFOE)
    #define UART0_FR_RXFE   (1 << 4)            // Receive FIFO Empty (LSR_RXBI)

    // BeagleBone Black DMTIMER2 base address
    #define DMTIMER2_BASE   0x48040000
    #define DTIMER2_TISR    (DMTIMER2_BASE + 0x28)  // Timer Interrupt Status Register (IRQSTATUS)
    #define DTIMER2_TIER    (DMTIMER2_BASE + 0x2C)  // Timer Interrupt Enable Register (IRQSTATUS_SET)
    #define DTIMER2_TCLR    (DMTIMER2_BASE + 0x38)  // Timer Control Register (TCLR)
    #define DTIMER2_TCRR    (DMTIMER2_BASE + 0x3C)  // Timer Counter Register (TCRR)
    #define DTIMER2_TLDR    (DMTIMER2_BASE + 0x40)  // Timer Load Register (TLDR)

    // BeagleBone Black Interrupt Controller (INTCPS) base address
    #define INTCPS_BASE     0x48200000
    #define INTC_MIR_CLEAR2 (INTCPS_BASE + 0xC8)    // Interrupt Mask Clear Register 2
    #define INTC_CONTROL    (INTCPS_BASE + 0x48)    // Interrupt Controller Control
    #define INTC_ILR68      (INTCPS_BASE + 0x110)   // Interrupt Line Register 68 (INTC_ILR4)

    // Clock Manager base address
    #define CM_PER_BASE 0x44E00000
    #define CM_PER_TIMER2_CLKCTRL (CM_PER_BASE + 0x80) // Timer2 Clock Control

#endif

// ============================================================================
// UART Functions
// ============================================================================

void uart_putc(char c);
char uart_getc(void);
void uart_puts(const char *s);
void uart_gets(char *buffer, int max_length);

// ============================================================================
// Timer Functions
// ============================================================================

void enable_irq(void);

#endif