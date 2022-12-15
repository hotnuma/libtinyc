#ifndef PRINT_H
#define PRINT_H

#if defined(DEBUG)
#define dprint print
#else
#define dprint(fmt, ...) do {} while (0)
#endif

void print(const char *fmt, ...);

#endif // PRINT_H


