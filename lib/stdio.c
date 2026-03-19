#include "stdio.h"

// ============================================================================
// Standard Library Functions
// ============================================================================

// Function to convert string to integer
int atoi(const char *s) {
    int num = 0;
    int sign = 1;
    int i = 0;

    // Handle optional sign
    if (s[i] == '-') {
        sign = -1;
        i++;
    }

    for (; s[i] >= '0' && s[i] <= '9'; i++) {
        num = num * 10 + (s[i] - '0');
    }

    return sign * num;
}

// Function to convert integer to string
void itoa(int num, char *buffer) {
    int i = 0;
    int is_negative = 0;

    if (num == 0) {
        buffer[i++] = '0';
        buffer[i] = '\0';
        return;
    }

    if (num < 0) {
        is_negative = 1;
        num = -num;
    }

    while (num > 0 && i < 14) { // Reserve space for sign and null terminator
        buffer[i++] = '0' + (num % 10);
        num /= 10;
    }

    if (is_negative) {
        buffer[i++] = '-';
    }

    buffer[i] = '\0';

    // Reverse the string
    int start = 0, end = i - 1;
    char temp;
    while (start < end) {
        temp = buffer[start];
        buffer[start] = buffer[end];
        buffer[end] = temp;
        start++;
        end--;
    }
}

// ============================================================================
// Standard I/O Functions
// ============================================================================

// Function to print formatted output based on format specifier
// (%d for integers, %s for strings)
void PRINT(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    char buf[BUF_SIZE];

    while (*fmt)
    {
        if (*fmt == '%')
        {
            fmt++;
            switch (*fmt)
            {
            case 'd':
            {
                int val = va_arg(args, int);
                itoa(val, buf);
                uart_puts(buf);
                break;
            }
            case 's':
            {
                const char *s = va_arg(args, const char *);
                uart_puts(s);
                break;
            }
            default:
                /* Unknown specifier: print as-is */
                uart_putc('%');
                uart_putc(*fmt);
                break;
            }
        }
        else
        {
            uart_putc(*fmt);
        }
        fmt++;
    }

    va_end(args);
}

// Function to read input based on format specifiers
// (%d for integers, %s for strings)
void READ(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    while (*fmt)
    {
        if (*fmt == '%')
        {
            fmt++;
            switch (*fmt)
            {
            case 'd':
            {
                int *ptr = va_arg(args, int *);
                char buf[BUF_SIZE];
                uart_gets(buf, BUF_SIZE);
                *ptr = atoi(buf);
                break;
            }
            case 's':
            {
                char *ptr = va_arg(args, char *);
                uart_gets(ptr, BUF_SIZE);
                break;
            }
            default:
                /* Unknown specifier: ignore */
                break;
            }
        }
        fmt++;
    }

    va_end(args);
}
