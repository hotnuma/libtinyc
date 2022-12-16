#include "libtest.h"
#include "libhtml.h"

#include <string.h>

#include <print.h>

#if 0

void test_libhtml()
{
    CString buffer;

    buffer = "<body> <p>bla</p> </body>";

    //void writeIndent(CString &outbuff, int indent, const CString &str);
    const char *result;
    int length;

    htmlGetTag(buffer, &result, &length);
    ASSERT(strncmp(result, "body", 4) == 0);
    ASSERT(length == 4);

    htmlGetElement(buffer, &result, &length);
    ASSERT(strncmp(result, " <p>", 4) == 0);
    ASSERT(length == 12);

}

#endif


