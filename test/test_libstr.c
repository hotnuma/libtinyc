#include "libtest.h"

#include "cstring.h"
#include "libstr.h"

#include <string.h>

#if 0

void test_libstr()
{
    CString buffer;

    //bool strGetPart(char **start, char **result, int *length);

    //bool strEllipsize(CString &str, int length, const char *part = "...");

    buffer = "blablebliebloblu";
    strEllipsize(buffer, 9);
    ASSERT(strcmp(buffer, "blable...") == 0);

    buffer = "blablebliebloblu";
    strEllipsize(buffer, 7, "+");
    ASSERT(strcmp(buffer, "blable+") == 0);

    //bool strPadLeft(CString &str, int length, char c);
    //bool strPadRight(CString &str, int length, char c);

    //CString strBaseName(const char *path);

    //char* stristr(const char *haystack, const char *needle);
    //char* strrstr(const char *haystack, const char *needle);

    //int streol(const char *str, int *pos = NULL);

    buffer = "bla\r\nble\r\nblie\r\n";
    int eol = streol(buffer);
    ASSERT(eol == STREOL_CRLF);

    //int utf8len(const char *str);

    int len = utf8len("éèà");
    ASSERT(len == 3);

    //CString utf8wrap(const char *str, int num);

}

#endif


