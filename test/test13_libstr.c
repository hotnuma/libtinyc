#include "libtest.h"
#include "libstr.h"
#include "cstring.h"

void test_libstr()
{
    CString *buffer = cstr_new_size(32);

    cstr_copy(buffer, "bla\r\nble\r\nblie\r\n");
    int pos;
    int eol = streol(c_str(buffer), &pos);
    ASSERT(eol == STREOL_CRLF);

    int len = utf8len("éèà");
    ASSERT(len == 3);

    cstr_free(buffer);
}


