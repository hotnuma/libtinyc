#include "libtest.h"

#include "cfile.h"

#include "libapp.h"

#if 0

#define _testfile "/tmp/tinycpp_cfile.txt"

//~CFile();

void test_CFile()
{
    CString filepath = _testfile;
    fileRemove(filepath);

    CFile file;
    file.open(filepath, "wb");

    file << "bla";
    file << "\n";
    file << "ble";
    file << "\n";
    file << "blie";
    file << "\n";

    file.flush();
    file.close();

    file.read(filepath);
    ASSERT(file.buffer().compare("bla\nble\nblie\n") == 0);

    fileRemove(filepath);

    return;
}

#endif


