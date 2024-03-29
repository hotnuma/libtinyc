#include "libtest.h"
#include "libapp.h"

#include "cfile.h"
#include <fcntl.h>
#include <unistd.h>

#define _TESTFILE "/tmp/tinyc_test.txt"

void test_libapp()
{
    CString *result = cstr_new_size(256);

    get_apppath(result);
    ASSERT(cstr_endswith(result, "/tinytest", true));

    int fd = open(_TESTFILE, O_RDWR|O_CREAT, 0777);
    if (fd != -1)
        close(fd);

    ASSERT(file_exists(_TESTFILE));

    file_remove(_TESTFILE);
    ASSERT(!file_exists(_TESTFILE));

    cstr_free(result);
}


