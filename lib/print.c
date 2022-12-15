#include "print.h"

#include <stdio.h>
#include <stdarg.h>

void print(const char *fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    vfprintf(stdout, fmt, va);
    va_end(va);

    fprintf(stdout, "\n");
    fflush(stdout);
}


