#include "os.h"

volatile unsigned int * const UART0 = (unsigned int *)UART0_BASE;

// Function to send a single character via UART
void uart_putc(char c) {
    // Wait until there is space in the FIFO
    while (UART0[UART_FR / 4] & UART_FR_TXFF);
    UART0[UART_DR / 4] = c;
}

// Function to receive a single character via UART
char uart_getc() {
    // Wait until data is available
    while (UART0[UART_FR / 4] & UART_FR_RXFE);
    return (char)(UART0[UART_DR / 4] & 0xFF);
}

// Function to send a string via UART
void uart_puts(const char *s) {
    while (*s) {
        uart_putc(*s++);
    }
}

// Function to receive a line of input via UART
void uart_gets_input(char *buffer, int max_length) {
    int i = 0;
    char c;
    while (i < max_length - 1) { // Leave space for null terminator
        c = uart_getc();
        if (c == '\n' || c == '\r') {
            uart_putc('\n'); // Echo newline
            break;
        }
        uart_putc(c); // Echo character
        buffer[i++] = c;
    }
    buffer[i] = '\0'; // Null terminate the string
}