#include "libtest.h"
#include "libpath.h"

#include <string.h>

// untested
//bool pathCanonicalize(char *path, int *len)

void test_libpath()
{
    CString filepath(64);
    const char *pstr = nullptr;

    filepath = "/a/b.c/.file";

    pstr = pathExt(filepath);
    ASSERT(!pstr);

    filepath = "/a/b.c/.file.tar.gz";

    pstr = pathSep(filepath);
    ASSERT(strcmp(pstr, "/.file.tar.gz") == 0);

    pstr = pathExt(filepath);
    ASSERT(strcmp(pstr, ".tar.gz") == 0);

    pstr = pathExt(filepath, false);
    ASSERT(strcmp(pstr, ".gz") == 0);

    CString result;

    result = pathDirName(filepath);
    ASSERT(result.compare("/a/b.c") == 0);

    result = pathBaseName(filepath);
    ASSERT(result.compare(".file.tar.gz") == 0);

    result = filepath;
    pathStripExt(result);
    ASSERT(result.compare("/a/b.c/.file") == 0);

    result = ".file.txt";
    pathStripExt(result);
    ASSERT(result.compare(".file") == 0);

    // join.
    result = pathJoin("/", "bla");
    ASSERT(result.compare("/bla") == 0);

    result = pathJoin("", "bla");
    ASSERT(result.compare("bla") == 0);

    result = pathJoin("/bla", "blie");
    ASSERT(result.compare("/bla/blie") == 0);

    result = pathJoin("/bla///", "///blie");
    ASSERT(result.compare("/bla/blie") == 0);

}


