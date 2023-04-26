#ifndef CSINGLEINSTANCE_H
#define CSINGLEINSTANCE_H

#include "libsocket.h"
#include <glib-object.h>
#include "libmacros.h"
#include <stdbool.h>

typedef struct _CSingleInstance CSingleInstance;

CSingleInstance* csingleinst_new();
void csingleinst_free(CSingleInstance *inst);

// auto free ------------------------------------------------------------------

#define CSingleInstanceAuto GC_CLEANUP(_freeCSingleInstance) CSingleInstance
GC_UNUSED static inline void _freeCSingleInstance(CSingleInstance **ptr)
{
    //print("_freeCSingleInstance");

    csingleinst_free(*ptr);
}


bool csingleinst_open(CSingleInstance *inst, const char *socket_path,
                      GIOFunc func, gpointer user_data);
int csingleinst_close(CSingleInstance *inst);
bool csingleinst_isfirst(CSingleInstance *inst);
int csingleinst_sock(CSingleInstance *inst);

#endif // CSINGLEINSTANCE_H


