#include "libtest.h"

#include "cstring.h"
#include "libstr.h"

#include <string.h>

//inline char* stralloc(int size);
//inline wchar_t* wcsalloc(int size);
//bool str_get_part(char **start, char **result, int *length);
//bool str_get_lineptr(char **start, char **result, int *length);
//char* stristr(const char *haystack, const char *needle);
//char* strrstr(const char *haystack, const char *needle);

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


