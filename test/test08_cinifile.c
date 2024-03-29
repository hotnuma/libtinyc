#include "libtest.h"
#include "cinifile.h"
#include "cfile.h"

#define _TESTFILE "/tmp/tinyc_cinifile.txt"

void test_cinifile()
{
    CFileAuto *file = cfile_new();
    cfile_open(file, _TESTFILE, "wb");

    cfile_write(file, "[Section1]\n");
    cfile_write(file, "key1=a\n");
    cfile_write(file, "key2=b\n");
    cfile_write(file, "key3=c\n");

    cfile_write(file, "[Section2]\n");
    cfile_write(file, "key1=1\n");
    cfile_write(file, "key2=2\n");
    cfile_write(file, "key3=3\n");

    cfile_flush(file);
    cfile_close(file);

    CIniFileAuto *inifile = cinifile_new();
    bool ret = cinifile_read(inifile, _TESTFILE);
    ASSERT(ret);

    int count = cinifile_size(inifile);
    ASSERT(count == 2);

    CIniSection *section = cinifile_section_at(inifile, 0);
    ASSERT(section);

    CStringAuto *value = cstr_new_size(32);
    cinisection_value(section, value, "key2", "-1");
    ASSERT(cstr_compare(value, "b", true) == 0);

    section = cinifile_section(inifile, "Section2");
    ASSERT(section);

    int intval = -1;
    cinisection_int(section, &intval, "key2", -1);
    ASSERT(intval == 2);
}


