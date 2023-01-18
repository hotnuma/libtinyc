#ifndef MACROS_H
#define MACROS_H

#include <stdlib.h>
#include "print.h"

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

// suppress warnings for unused static inline functions
#define inline inline __attribute__((unused))

#define _CCLEANUP(func) __attribute__((cleanup(func)))
#define c_autofree _CCLEANUP(_freeBuffer)

static inline void _freeBuffer(void *p)
{
    void **pp = (void**) p;

    free(*pp);
}

#ifdef GLIB_CHECK_VERSION
#define c_autounref _CCLEANUP(_c_autounref_func)

static inline void _c_autounref_func(void *p)
{
    void **pp = (void**) p;

    if (*pp)
        g_object_unref(*pp);
}
#endif

#endif // MACROS_H


