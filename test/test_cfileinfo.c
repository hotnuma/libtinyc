#include "libtest.h"

#include "cstring.h"
#include "cfile.h"
#include "cfileinfo.h"

#include "libapp.h"

#include <string.h>

#include "lib/print.h"

//CFileInfo(const char *filepath);
//~CFileInfo();
//void close();

#if 0

#define _testfile "/tmp/tinycpp_cfileinfo.txt"

void test_cfileinfo()
{
    fileRemove(_testfile);

    CFile file;
    file.open(_testfile, "wb");
    file << "bla";
    file.flush();
    file.close();

    //CString fullpath = _testfile;
    //fullpath += "\\abc.txt";

    CFileInfo fileinfo;
    bool ret = fileinfo.read(_testfile);
    ASSERT(ret);
    ASSERT(fileinfo.exists());
    //ASSERT(strcmp(fullpath.c_str(), file.path()) == 0);

    size_t size = fileinfo.size();
    ASSERT(size == 3);

//    uint64_t time = fileinfo.mtime();
//    ASSERT(time > 1514761200000);

}

#endif


