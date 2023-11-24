#include "test_main.h"
#include "cdirparser.h"

void test_cdirparser()
{
    CDirParser *dir = cdirparser_new();
    cdirparser_open(dir, _TMPROOTDIR, CDP_SUBDIRS | CDP_DIRS);

    CString *filepath = cstr_new_size(64);
    bool ret = false;

    int count = 0;
    while (cdirparser_read(dir, filepath, NULL))
    {
        //print(c_str(filepath));
        ++count;
    }
    ASSERT(count == 6);

    ret = cdirparser_open(dir, _TMPROOTDIR, CDP_SUBDIRS | CDP_FILES);
    ASSERT(ret);

    count = 0;
    while (cdirparser_read(dir, filepath, NULL))
    {
        //print(c_str(filepath));
        ++count;
    }
    ASSERT(count == 4);

    cdirparser_free(dir);
    cstr_free(filepath);
}


