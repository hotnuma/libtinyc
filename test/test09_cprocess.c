#include "libtest.h"
#include "cprocess.h"

void test_cprocess()
{
    CProcess *process = cprocess_new();

    cprocess_start(process, "uname -a", CP_PIPEOUT);
    ASSERT(cstr_startswith(cprocess_outbuff(process), "Linux", true));

    cprocess_free(process);
}


