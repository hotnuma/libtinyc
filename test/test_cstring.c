#include "libtest.h"
#include "cstring.h"
#include <string.h>

#include "print.h"

void test_cstring()
{
    int array[10];
    ASSERT(ARRAY_SIZE(array) == 10);

    ASSERT(MIN(2, 10) == 2);
    ASSERT(MAX(2, 10) == 10);

    CString *strA = cstr_alloc(10, "alloc", -1);
    ASSERT(cstr_capacity(strA) == 10);
    ASSERT(strcmp(c_str(strA), "alloc") == 0);
    ASSERT(cstr_size(strA) == 5);

    cstr_resize(strA, 20);
    ASSERT(cstr_capacity(strA) == 20);

    CString *strB = cstr_new("strB");
    cstr_swap(strA, strB);
    ASSERT(strcmp(c_str(strA), "strB") == 0);
    ASSERT(strcmp(c_str(strB), "alloc") == 0);

    cstr_clear(strA);
    ASSERT(strcmp(c_str(strA), "") == 0);
    ASSERT(cstr_isempty(strA));

    cstr_append(strA, "blable");
    ASSERT(strcmp(c_str(strA), "blable") == 0);

    cstr_append_c(strA, 'b');
    ASSERT(strcmp(c_str(strA), "blableb") == 0);

    cstr_terminate(strA, 3);
    ASSERT(strcmp(c_str(strA), "bla") == 0);

    cstr_replace(strA, "l", "r", true);
    ASSERT(strcmp(c_str(strA), "bra") == 0);

    cstr_chop(strA, 1);
    ASSERT(strcmp(c_str(strA), "br") == 0);

    cstr_copy(strA, "   a b c   ");
    cstr_trim(strA, strB);
    ASSERT(strcmp(c_str(strB), "a b c") == 0);

    cstr_swap(strA, strB);
    cstr_remove(strA, 1, 3);
    ASSERT(strcmp(c_str(strA), "ac") == 0);

    ASSERT(cstr_first(strA) == 'a');
    ASSERT(cstr_last(strA) == 'c');

    cstr_copy(strA, "blable");
    ASSERT(cstr_at(strA, 2) == 'a');

    cstr_left(strA, strB, 3);
    ASSERT(strcmp(c_str(strB), "bla") == 0);

    cstr_mid(strA, strB, 2, 2);
    ASSERT(strcmp(c_str(strB), "ab") == 0);

    ASSERT(cstr_compare(strA, "blable", true) == 0);
    ASSERT(cstr_contains(strA, "a", true));
    ASSERT(cstr_contains(strA, "A", true) == false);
    ASSERT(cstr_startswith(strA, "bl", true));
    ASSERT(cstr_endswith(strA, "le", true));

    cstr_toupper(strA);
    ASSERT(strcmp(c_str(strA), "BLABLE") == 0);

    cstr_tolower(strA);
    ASSERT(strcmp(c_str(strA), "blable") == 0);

    cstr_free(strA);
    cstr_free(strB);

    #if 0
    // Untested
    char* cstr_reserve_ptr(CString *cstr, int *remain);
    #define CStringAuto GC_CLEANUP(_freeCString) CString
    void cstr_fmt(CString *cstr, const char *fmt, ...);
    void cstr_int(CString *cstr, int val);
    void cstr_uint64(CString *cstr, uint64_t val);
    void cstr_xfrm(CString *cstr, const char *str);
    void cstr_enquote(CString *cstr, const char *str);
    void cstr_unquote(CString *cstr, const char *str);
    #endif

}


