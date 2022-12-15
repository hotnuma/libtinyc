#include "libtest.h"

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define _testroot "/tmp/tinycpp_tests"

void test_clist();
void test_cstring();
void test_cstringlist();

int main()
{
    RUN(test_cstring);
    RUN(test_clist);
    RUN(test_cstringlist);

    return TEST_REPORT();
}


