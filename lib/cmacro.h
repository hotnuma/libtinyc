#ifndef CMACRO_H
#define CMACRO_H

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

#define c_auto(t) __attribute__ ((__cleanup__(_free##t))) t

#endif // CMACRO_H


