#include "libtest.h"

#include "cprocess.h"

#include "cstring.h"
#include "libapp.h"

#include "print.h"

#if 0

void test_cprocess()
{
    CProcess process;
    process.start("uname -a", CP_PIPEOUT);
    ASSERT(process.outBuff.startsWith("Linux"));

}

#endif


