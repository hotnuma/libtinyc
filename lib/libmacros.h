#ifndef LIBMACROS_H
#define LIBMACROS_H

#include <stdlib.h>
#include <stdbool.h>

#ifdef __clang_analyzer__
#define _freeBuffer free
#endif

typedef void (*CDeleteFunc) (void*);
typedef int (*CCompareFunc) (const void*, const void*);

#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof(0[arr]))

#ifndef MIN
#define MIN(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })
#endif

#ifndef MAX
#define MAX(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })
#endif

#define GC_UNUSED __attribute__((__unused__))
#define GC_CLEANUP(func) __attribute__((cleanup(func)))

#define c_autofree GC_CLEANUP(_freeBuffer)

GC_UNUSED static inline void _freeBuffer(void *p)
{
    void **pp = (void**) p;

    if (*pp)
        free(*pp);
}

#endif // LIBMACROS_H


