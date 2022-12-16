#include "libtest.h"

#include "libapp.h"
#include "cstring.h"
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define filepath "/tmp/tinycpp_test.txt"

void test_libapp()
{
    CString *result = cstr_new_size(256);

    get_apppath(result);
    ASSERT(cstr_endswith(result, "/tinytest", true));

    ASSERT(dir_exists("/tmp"));

    int fd = open(filepath, O_RDWR|O_CREAT, 0777);
    if (fd != -1)
        close(fd);

    ASSERT(file_exists(filepath));

    file_remove(filepath);
    ASSERT(!file_exists(filepath));

    cstr_free(result);
}


