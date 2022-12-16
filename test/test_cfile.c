#include "libtest.h"

#include "cfile.h"
#include "libapp.h"
#include "print.h"

#define _TESTFILE "/tmp/tinyc_cfile.txt"

void test_cfile()
{
    file_remove(_TESTFILE);

    CFile *file = cfile_new();
    cfile_open(file, _TESTFILE, "wb");

    cfile_write(file, "bla");
    cfile_write(file, "\n");
    cfile_write(file, "ble");
    cfile_write(file, "\n");
    cfile_write(file, "blie");
    cfile_write(file, "\n");

    cfile_flush(file);
    cfile_close(file);

    cfile_read(file, _TESTFILE);
    ASSERT(cstr_compare(cfile_buffer(file), "bla\nble\nblie\n", true) == 0);

    file_remove(_TESTFILE);

    cfile_free(file);
}


