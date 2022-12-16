#include "libtest.h"
#include "cstringlist.h"

#include <stdlib.h>

#include "print.h"

void test_cstringlist()
{
    CStringList *listA = cslist_new_size(10);
    ASSERT(cslist_capacity(listA) == 10);
    ASSERT(cslist_isempty(listA));

    cslist_resize(listA, 20);
    ASSERT(cslist_capacity(listA) == 20);

    cslist_append(listA, "bla");
    cslist_append(listA, "ble");
    cslist_append(listA, "blo");
    cslist_append(listA, "blu");
    ASSERT(cslist_size(listA) == 4);

    cslist_insert(listA, 1, "blie");
    CString *item = cslist_at(listA, 1);
    ASSERT(strcmp(c_str(item), "blie") == 0);

    cslist_move(listA, 1, 2);
    item = cslist_at(listA, 2);
    ASSERT(strcmp(c_str(item), "blie") == 0);

    item = cslist_takeAt(listA, 3);
    ASSERT(strcmp(c_str(item), "blo") == 0);
    ASSERT(cslist_size(listA) == 4);
    cstr_free(item);

    item = cslist_takeFirst(listA);
    ASSERT(strcmp(c_str(item), "bla") == 0);
    cstr_free(item);

    item = cslist_takeLast(listA);
    ASSERT(strcmp(c_str(item), "blu") == 0);
    cstr_free(item);

    cslist_clear(listA);
    ASSERT(cslist_size(listA) == 0);

    cslist_append(listA, "bla");
    cslist_append(listA, "ble");
    cslist_append(listA, "blie");
    cslist_append(listA, "blo");
    cslist_append(listA, "blu");
    ASSERT(cslist_size(listA) == 5);

    cslist_removeAt(listA, 2);
    ASSERT(cslist_size(listA) == 4);

    cslist_removeFirst(listA);
    cslist_removeLast(listA);
    ASSERT(cslist_size(listA) == 2);

    item = cslist_at(listA, 0);
    ASSERT(strcmp(c_str(item), "ble") == 0);

    item = cslist_join(listA, ",");
    ASSERT(strcmp(c_str(item), "ble,blo") == 0);
    cstr_free(item);

    cslist_free(listA);
}


