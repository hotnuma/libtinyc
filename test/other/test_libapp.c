#include "libapp.h"

#include "libtest.h"
#include "CString.h"

#include <fcntl.h>
#include <unistd.h>

#include <string.h>

// untested
//CStringList getArguments(const wchar_t *cmdLine = nullptr);
//bool isFirstInstance(const wchar_t *guid);
//int uchdir(const char *directory);
//int pexec(const char *cmd);

void test_libapp()
{
    CString path = getApplicationPath();
    ASSERT(path.endsWith("/tinytest"));

    ASSERT(dirExists("/tmp"));

    CString filepath = "/tmp/tinycpp_test.txt";

    int fd = open(filepath, O_RDWR|O_CREAT, 0777);
    if (fd != -1)
        close(fd);

    ASSERT(fileExists(filepath));

    fileRemove(filepath);
    ASSERT(!fileExists(filepath));

    CString time = getCurrentTime("%Y");
    ASSERT(time.compare("1980") > 0);

//    path = getApplicationDir();
//    ASSERT(strncmp(_testroot, path.c_str(), path.size() - 6) == 0);

    //path = getWindowsDirectory();
    //ASSERT(path.endsWith("Windows"));

    //CStringList list = getLogicalDrives();
    //ASSERT(list.size() > 1);
    //ASSERT(list.at(0).compare("C:\\") == 0);

}


