#include "CDirParser.h"
#include "libtest.h"

#include "print.h"

// untested
//CDirIterator(const char *directory, int flags = DF_DIRS | DF_FILES);
//void close();

//extern char _testroot[];

#define _testroot "/tmp/tinycpp_tests"

void test_CDirParser()
{
    CDirParser dir;
    CString filepath;

    bool ret = dir.open(_testroot, CDP_SUBDIRS | CDP_DIRS);
    ASSERT(ret);

    int count = 0;
    while (dir.read(filepath))
    {
        //print(filepath.c_str());
        ++count;
    }
    ASSERT(count == 6);

    ret = dir.open(_testroot, CDP_SUBDIRS | CDP_FILES);
    ASSERT(ret);

    count = 0;
    while (dir.read(filepath))
    {
        //print(filepath.c_str());
        ++count;
    }
    ASSERT(count == 4);

}


