#include "stdio.h"

// Simple function to convert string to integer
int uart_atoi(const char *s) {
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
void uart_itoa(int num, char *buffer) {
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

//Funcion para convertir integer a string
static void print_int(int value) {
    char buf[16];
    uart_itoa(value, buf);
    uart_puts(buf);
}

//Apunta al inicio del puntero, le decimos el tipo con el %d %f o %s
void PRINT(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    while (*fmt) {
        if (*fmt == '%') {
            fmt++; //despues de leer el de arriba se mueve al siguiente caracter
            if (*fmt == 'd') { //Un entero
                int val = va_arg(args, int);
                print_int(val); //Imprime el entero
            } else if (*fmt == 's') { //Texto
                char *s = va_arg(args, char *);
                uart_puts(s); //Imprime le texto
            } else if (*fmt == 'c') {
                int val = va_arg(args, int); // char se promueve a int en va_arg
                uart_putc((char)val);
            } else {
                uart_putc('%');
                uart_putc(*fmt);
            }
        } else {
            uart_putc(*fmt); //Solo imprime
        }
        fmt++;
    }

    va_end(args);
}

void READ(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    if (*fmt == '%' && *(fmt + 1) == 'd') {
        int *out = va_arg(args, int *);
        char buf[INPUT_BUF_SIZE];
        uart_gets_input(buf, INPUT_BUF_SIZE);
        *out = uart_atoi(buf);
    }
    else if (*fmt == '%' && *(fmt + 1) == 's') {
        char *out = va_arg(args, char *);
        uart_gets_input(out, INPUT_BUF_SIZE);
    }

    va_end(args);
}

int parse_int(const char *s){
    return uart_atoi(s);
}