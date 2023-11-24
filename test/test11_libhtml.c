#include "libtest.h"
#include "libhtml.h"

void test_libhtml()
{
    CString *buffer = cstr_new("<body> <p>bla</p> </body>");

    const char *result;
    int length;

    html_get_tag(c_str(buffer), &result, &length);
    ASSERT(strncmp(result, "body", 4) == 0);
    ASSERT(length == 4);

    html_get_element(c_str(buffer), &result, &length, false);
    ASSERT(strncmp(result, " <p>", 4) == 0);
    ASSERT(length == 12);

    cstr_free(buffer);
}


