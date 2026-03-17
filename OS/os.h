#ifndef OS_H
#define OS_H

// Uncomment the platform you are targeting
//#define PLATFORM_VERSATILEPB
//#define PLATFORM_BEAGLEBONE
// NOTE: Ensure to comment both of them when compiling

#ifdef PLATFORM_VERSATILEPB

    #define UART0_BASE 0x101F1000
    #define UART_DR 0x00      // Data Register
    #define UART_FR 0x18      // Flag Register
    #define UART_FR_TXFF 0x20 // Transmit FIFO Full
    #define UART_FR_RXFE 0x10 // Receive FIFO Empty

#elif defined(PLATFORM_BEAGLEBONE)

    // BeagleBone Black UART0 base address
    #define UART0_BASE 0x44E09000
    #define UART_THR (UART0_BASE + 0x00) // Transmit Holding Register
    #define UART_LSR (UART0_BASE + 0x14) // Line Status Register
    #define UART_LSR_THRE 0x20           // Transmit Holding Register Empty
    #define UART_LSR_RXFE 0x10           // Receive FIFO Empty

    // BeagleBone Black DMTIMER2 base address
    #define DMTIMER2_BASE 0x48040000
    #define TCLR (DMTIMER2_BASE + 0x38) // Timer Control Register
    #define TCRR (DMTIMER2_BASE + 0x3C) // Timer Counter Register
    #define TISR (DMTIMER2_BASE + 0x28) // Timer Interrupt Status Register
    #define TIER (DMTIMER2_BASE + 0x2C) // Timer Interrupt Enable Register
    #define TLDR (DMTIMER2_BASE + 0x40) // Timer Load Register

    // BeagleBone Black Interrupt Controller (INTCPS) base address
    #define INTCPS_BASE 0x48200000
    #define INTC_MIR_CLEAR2 (INTCPS_BASE + 0xC8) // Interrupt Mask Clear Register 2
    #define INTC_CONTROL (INTCPS_BASE + 0x48)    // Interrupt Controller Control
    #define INTC_ILR68 (INTCPS_BASE + 0x110)     // Interrupt Line Register 68

    // Clock Manager base address
    #define CM_PER_BASE 0x44E00000
    #define CM_PER_TIMER2_CLKCTRL (CM_PER_BASE + 0x80) // Timer2 Clock Control

#endif

// UART function prototypes
void uart_putc(char c);
char uart_getc(void);
void uart_puts(const char *s);
void uart_gets_input(char *buffer, int max_length);

#endif