#include "test_main.h"

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

void test_cstring();
void test_clist();
void test_cstringlist();

void test_libapp();
void test_libhtml();
void test_libpath();
void test_libstr();

void test_cdirent();
void test_cdirparser();
void test_cfile();
void test_cfileinfo();
void test_cprocess();

static void _create_file(const char *filepath)
{
    int fd2 = creat(filepath, 0777);

    if (fd2 != -1)
        close(fd2);
}

static void _create_dirs()
{
    struct stat st;

    if (stat(_TMPROOTDIR, &st) != -1)
        return;

    mkdir(_TMPROOTDIR, 0700);
    _create_file(_TMPROOTDIR"/file.txt");

    mkdir(_TMPROOTDIR"/dirAA", 0700);
    mkdir(_TMPROOTDIR"/dirAA/dirA", 0700);
    _create_file(_TMPROOTDIR"/dirAA/dirA/fileA.txt");

    mkdir(_TMPROOTDIR"/dirBB", 0700);
    mkdir(_TMPROOTDIR"/dirBB/dirB", 0700);
    _create_file(_TMPROOTDIR"/dirBB/dirB/fileB.txt");

    mkdir(_TMPROOTDIR"/dirCC", 0700);
    mkdir(_TMPROOTDIR"/dirCC/dirC", 0700);
    _create_file(_TMPROOTDIR"/dirCC/dirC/fileC.txt");
}

int main()
{
    _create_dirs();

    RUN(test_cstring);
    RUN(test_clist);
    RUN(test_cstringlist);

    RUN(test_libapp);
    RUN(test_libhtml);
    RUN(test_libpath);
    RUN(test_libstr);

    RUN(test_cdirent);
    RUN(test_cdirparser);
    RUN(test_cfile);
    RUN(test_cfileinfo);
    RUN(test_cprocess);

    return TEST_REPORT();
}


