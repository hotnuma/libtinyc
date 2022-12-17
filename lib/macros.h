#ifndef MACROS_H
#define MACROS_H

#include <stdlib.h>
#include "print.h"

#ifdef __clang_analyzer__
#define _freeBuffer free
#endif

typedef void (*CDeleteFunc) (void *data);

#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof(0[arr]))

#define MIN(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

#define MAX(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define inline inline __attribute__((unused))

#define _CCLEANUP(func) __attribute__((cleanup(func)))
#define c_auto(t) _CCLEANUP(_free##t) t
#define c_autofree _CCLEANUP(_freeBuffer)

static inline void _freeBuffer(void *p)
{
    print("_freeBuffer");

    void **pp = (void**) p;

    free(*pp);
}

#endif // MACROS_H


