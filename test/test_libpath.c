#include "libtest.h"
#include "libpath.h"

#include <string.h>

// untested
//bool pathCanonicalize(char *path, int *len)

void test_libpath()
{
    CString *filepath = cstr_new_size(64);
    CString *result = cstr_new_size(64);
    const char *pstr = NULL;

    cstr_copy(filepath, "/a/b.c/.file.tar.gz");

    // sep
    path_dirname(result, c_str(filepath));
    ASSERT(cstr_compare(result, "/a/b.c", true) == 0);

    path_basename(result, c_str(filepath));
    ASSERT(cstr_compare(result, ".file.tar.gz", true) == 0);

    pstr = path_sep(c_str(filepath));
    ASSERT(strcmp(pstr, "/.file.tar.gz") == 0);

    // ext
    pstr = path_ext(c_str(filepath), true);
    ASSERT(strcmp(pstr, ".tar.gz") == 0);

    pstr = path_ext(c_str(filepath), false);
    ASSERT(strcmp(pstr, ".gz") == 0);

    path_strip_ext(filepath);
    ASSERT(cstr_compare(filepath, "/a/b.c/.file", true) == 0);

    // join.
    path_join(result, "/", "bla");
    ASSERT(cstr_compare(result, "/bla", true) == 0);

    path_join(result, "", "bla");
    ASSERT(cstr_compare(result, "bla", true) == 0);

    path_join(result, "/bla", "blie");
    ASSERT(cstr_compare(result, "/bla/blie", true) == 0);

    path_join(result, "/bla///", "///blie");
    ASSERT(cstr_compare(result, "/bla/blie", true) == 0);

    cstr_free(filepath);
    cstr_free(result);
}


