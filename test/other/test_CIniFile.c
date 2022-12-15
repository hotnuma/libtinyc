#include "CIniFile.h"

#include "libtest.h"
#include "CString.h"
#include "CFile.h"
#include <ctype.h>

#include "print.h"

#define _testfile "/tmp/tinycpp_cinifile.txt"

void test_CIniFile()
{
    CFile file;
    file.open(_testfile, "wb");

    file << "[Section1]\n";
    file << "key1=a\n";
    file << "key2=b\n";
    file << "key3=c\n";

    file << "[Section2]\n";
    file << "key1=d\n";
    file << "key2=e\n";
    file << "key3=f\n";

    file.flush();
    file.close();

    CIniFile inifile;
    bool ret = inifile.open(_testfile);
    ASSERT(ret);

    CString value;
    int count = inifile.size();
    ASSERT(count == 2);

    CIniSection *section = inifile.sectionAt(0);
    ASSERT(section);

    value = section->value("key2", "-1");
    ASSERT(value.compare("b") == 0);

    section = inifile.section("Section2");
    ASSERT(section);

    value = section->value("key2", "-1");
    ASSERT(value.compare("e") == 0);

}


