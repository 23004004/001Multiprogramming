#ifndef STDIO_H
#define STDIO_H

#include "../OS/os.h"

#include <stdarg.h> // For dynamic argument handling

#define FLOAT_SCALE 100
#define BUF_SIZE 32

// ============================================================================
// Standard Library Functions
// ============================================================================

int atoi(const char *s);
void itoa(int num, char *buffer);

// ============================================================================
// Standard I/O Functions
// ============================================================================

void PRINT(const char *fmt, ...);
void READ(const char *fmt, ...);

#endif
