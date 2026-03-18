#ifndef STDIO_H
#define STDIO_H

#include "../OS/os.h"

#include <stdarg.h>   //Este se usa para parametros dinamicos

#define FLOAT_SCALE 100
#define INPUT_BUF_SIZE 32

void PRINT(const char *fmt, ...);
void READ(const char *fmt, ...);

#endif
