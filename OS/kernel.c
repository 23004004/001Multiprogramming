#include "os.h"

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
// UART Functions
// ============================================================================

// Function to send a single character via UART
void uart_putc(char c)
{
    // Wait until there is space in the FIFO
    while ((REG(UART0_FR) & UART0_FR_TXFE) == 0)
        ;
    REG(UART0_DR) = c;
}

// Function to receive a single character via UART
char uart_getc(void)
{
    // Wait until data is available
    while ((REG(UART0_FR) & UART0_FR_RXFE) != 0)
        ;
    return (char)(REG(UART0_DR) & 0xFF);
}

// Function to send a string via UART
void uart_puts(const char *s)
{
    while (*s)
    {
        uart_putc(*s++);
    }
}

// Function to receive a line of input via UART
void uart_gets(char *buffer, int max_length)
{
    int i = 0;
    char c;
    while (i < max_length - 1)
    { // Leave space for null terminator
        c = uart_getc();
        if (c == '\n' || c == '\r')
        {
            uart_putc('\n'); // Echo newline
            break;
        }
        uart_putc(c); // Echo character
        buffer[i++] = c;
    }
    buffer[i] = '\0'; // Null terminate the string
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
    uart_puts("Tick\n");
}

// ============================================================================
// Main Function
// ============================================================================

void os_init(void)
{
    // Welcome message
    uart_puts("\n=== 0001Multiprogramming ===\n");
    uart_puts(" - Carlos Alvarez - 23004004\n");
    uart_puts(" - Gabriel Garcia - 17001171\n");
    uart_puts("\nStarting ...\n\n");

    // Disable the watchdog timer to prevent resets
    uart_puts("Disabling watchdog ... ");
    watchdog_disable();
    uart_puts("OK\n");

    uart_puts("Initializing timer ... ");
    timer_init();
    uart_puts("OK\n");

    uart_puts("Enabling interrupts ... ");
    //enable_irq(); // Uncomment this line when building OS (temporary)
    uart_puts("OK\n");

    uart_puts("\nOS started successfully.\n\n");
}