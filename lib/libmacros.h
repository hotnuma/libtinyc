#ifndef LIBMACROS_H
#define LIBMACROS_H

#include <stdlib.h>

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

//https://stackoverflow.com/questions/7090998/
//#ifndef UNUSED
//#define UNUSED(x) (void)(x)
//#endif

#define GC_UNUSED __attribute__((__unused__))
#define GC_CLEANUP(func) __attribute__((cleanup(func)))

#define c_autofree GC_CLEANUP(_freeBuffer)

GC_UNUSED static inline void _freeBuffer(void *p)
{
    void **pp = (void**) p;

    if (*pp)
        free(*pp);
}

#ifdef GLIB_CHECK_VERSION
#define c_autounref GC_CLEANUP(_c_autounref_func)

GC_UNUSED static inline void _c_autounref_func(void *p)
{
    void **pp = (void**) p;

    if (*pp)
        g_object_unref(*pp);
}
#endif

#endif // LIBMACROS_H


