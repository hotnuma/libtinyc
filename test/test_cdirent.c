#include "test_main.h"

#include "cdirent.h"
#include "print.h"

void test_cdirent()
{
    bool dirfound = false;
    int numdirs = 0;
    bool filefound = false;
    int numfiles = 0;
    int count = 0;

    CDirent *dirA = cdirent_new_path(_TMPROOTDIR);
    CString *result = cstr_new_size(32);

    int type = 0;

    while (cdirent_read(dirA, result, &type))
    {
        //print("%s %d", c_str(result), type);

        if (type == DT_DIR)
        {
            if (cstr_compare(result, "dirBB", true) == 0)
                dirfound = true;

            ++numdirs;
        }
        else if (type == DT_REG)
        {
            if (cstr_compare(result, "file.txt", true) == 0)
                filefound = true;

            ++numfiles;
        }

        ++count;
    }

    ASSERT(dirfound);
    ASSERT(numdirs == 3);
    ASSERT(filefound);
    ASSERT(numfiles == 1);
    ASSERT(count == 4);

    cdirent_free(dirA);
    cstr_free(result);
}


