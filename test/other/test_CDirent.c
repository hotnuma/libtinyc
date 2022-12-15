#include "CDirent.h"
#include "libtest.h"

#include "print.h"

// untested
// ~CDirent();
// const char* directory() {return _basedir.c_str();}

#define _testroot "/tmp/tinycpp_tests"

void test_CDirent()
{
    bool found = false;
    int count = 0;
    int numdirs = 0;

    CDirent dirA(_testroot);
    CString result;

    int type = 0;

    while (dirA.read(result, &type))
    {
        //print("%s %d", result.c_str(), type);

        if (type == DT_DIR)
        {
            if (result == "dirBB")
                found = true;

            ++numdirs;
        }
        ++count;
    }

    ASSERT(found);
    ASSERT(numdirs == 3);
    ASSERT(count == 4);

//    CDirent dirB;
//    ASSERT(dirB.open(_testroot));

//    found = false;
//    count = 0;
//    int numfiles = 0;
//    while (dirB.read(result, &type))
//    {
//        if (S_ISREG(type))
//        {
//            if (result == "abc.txt")
//                found = true;
//            ++numfiles;
//        }
//        ++count;
//    }

//    ASSERT(found);
//    ASSERT(numfiles == 4);
//    ASSERT(count == 7);

//    dirB.close();

}


