#include "CProcess.h"
#include "libtest.h"

#include "CString.h"
#include "libapp.h"

#include "print.h"

void test_CProcess()
{
    CProcess process;
    process.start("uname -a", CP_PIPEOUT);
    ASSERT(process.outBuff.startsWith("Linux"));

}


