#ifndef CPROCESS_H
#define CPROCESS_H

#include "CString.h"

#define CP_NONE     0x00
#define CP_PIPEIN   0x01
#define CP_PIPEOUT  0x02
#define CP_PIPEERR  0x04
#define CP_MERGEERR 0x08

#define CPH_INVALID    -1
#define CPH_OUT         0
#define CPH_IN          1

class CProcess
{
public:

    CProcess();
    ~CProcess();

    bool start(const char *cmd, int flags = CP_NONE);
    int exitStatus() {return _exitStatus;}

    CString inBuff;
    CString outBuff;
    CString errBuff;

private:

    int _inPipe[2] = {CPH_INVALID};
    int _outPipe[2] = {CPH_INVALID};
    int _errPipe[2] = {CPH_INVALID};

    int _exitStatus = -1;

};

#endif // CPROCESS_H


