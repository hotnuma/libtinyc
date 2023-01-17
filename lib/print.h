#ifndef PRINT_H
#define PRINT_H

#if defined(CDEBUG_PRINT)
#define dprint print
#else
#define dprint(fmt, ...) do {} while (0)
#endif

#define c_refcount(obj) (print("%s count = %d", G_OBJECT_TYPE_NAME(obj), G_OBJECT(obj)->ref_count))

void print(const char *fmt, ...);

#endif // PRINT_H


