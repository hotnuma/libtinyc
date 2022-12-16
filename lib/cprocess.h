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

#if 0

bool start(const char *cmd, int flags = CP_NONE);
int exitStatus() {return _exitStatus;}

#endif

#endif // CPROCESS_H


