#include "libtest.h"

#include "cfileinfo.h"
#include "libapp.h"
#include "cfile.h"

#define _TESTFILE "/tmp/tinyc_cfileinfo.txt"

void test_cfileinfo()
{
    file_remove(_TESTFILE);

    CFile *file = cfile_new_path(_TESTFILE, "wb");

    cfile_write(file, "bla");
    cfile_flush(file);
    cfile_free(file);

    CFileInfo *fi = cfileinfo_new_path(_TESTFILE);
    bool ret = cfileinfo_read(fi, _TESTFILE);

    ASSERT(ret);
    ASSERT(cfileinfo_exists(fi));

    size_t size = cfileinfo_size(fi);
    ASSERT(size == 3);

    uint64_t time = cfileinfo_mtime(fi);
    ASSERT(time > 1514761200000);

    cfileinfo_free(fi);
}


