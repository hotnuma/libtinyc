#include "test_main.h"

#include "cdirent.h"
#include <dirent.h>
#include "print.h"

void test_cdirent()
{
    bool found = false;
    int count = 0;
    int numdirs = 0;

    CDirent *dirA = cdirent_new_path(_TMPROOTDIR);
    CString *result = cstr_new_size(32);

    int type = 0;

    while (cdirent_read(dirA, result, &type))
    {
        //print("%s %d", c_str(result), type);

        if (type == DT_DIR)
        {
            if (cstr_compare(result, "dirBB", true) == 0)
                found = true;

            ++numdirs;
        }

        ++count;
    }

    ASSERT(found);
    ASSERT(numdirs == 3);
    ASSERT(count == 4);

    cstr_free(result);
    cdirent_free(dirA);
}


