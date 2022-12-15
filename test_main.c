#include "libtest.h"

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define _testroot "/tmp/tinycpp_tests"

void test_cstring();

int main()
{
    // core objects
    RUN(test_cstring);

    return TEST_REPORT();
}


