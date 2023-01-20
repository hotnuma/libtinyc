#ifndef CPROCESS_H
#define CPROCESS_H

#include "cstring.h"

#define CP_NONE     0x00
#define CP_PIPEIN   0x01
#define CP_PIPEOUT  0x02
#define CP_PIPEERR  0x04
#define CP_MERGEERR 0x08

#define CPH_INVALID -1
#define CPH_OUT     0
#define CPH_IN      1

typedef struct _CProcess CProcess;

CProcess* cprocess_new();
void cprocess_free(CProcess *cpr);

// auto free ------------------------------------------------------------------

#define CProcessAuto GC_CLEANUP(_freeCProcess) CProcess
GC_UNUSED static inline void _freeCProcess(CProcess **process)
{
    cprocess_free(*process);
}

bool cprocess_start(CProcess *cpr, const char *cmd, int flags);
CString* cprocess_outbuff(CProcess *cpr);
int cprocess_exitstatus(CProcess *cpr);

#endif // CPROCESS_H


