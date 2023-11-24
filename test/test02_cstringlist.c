#include "libtest.h"
#include "cstringlist.h"

#include <stdlib.h>

#include "print.h"

void test_cstringlist()
{
    CStringList *listA = cstrlist_new_size(10);
    ASSERT(cstrlist_capacity(listA) == 10);
    ASSERT(cstrlist_isempty(listA));

    cstrlist_resize(listA, 20);
    ASSERT(cstrlist_capacity(listA) == 20);

    cstrlist_append(listA, "bla");
    cstrlist_append(listA, "ble");
    cstrlist_append(listA, "blo");
    cstrlist_append(listA, "blu");
    ASSERT(cstrlist_size(listA) == 4);

    cstrlist_insert(listA, 1, "blie");
    CString *item = cstrlist_at(listA, 1);
    ASSERT(strcmp(c_str(item), "blie") == 0);

    cstrlist_move(listA, 1, 2);
    ASSERT(strcmp(cstrlist_at_str(listA, 2), "blie") == 0);

    item = cstrlist_take_at(listA, 3);
    ASSERT(strcmp(c_str(item), "blo") == 0);
    ASSERT(cstrlist_size(listA) == 4);
    cstr_free(item);

    item = cstrlist_take_first(listA);
    ASSERT(strcmp(c_str(item), "bla") == 0);
    cstr_free(item);

    item = cstrlist_take_last(listA);
    ASSERT(strcmp(c_str(item), "blu") == 0);
    cstr_free(item);

    cstrlist_clear(listA);
    ASSERT(cstrlist_size(listA) == 0);

    cstrlist_append(listA, "bla");
    cstrlist_append(listA, "ble");
    cstrlist_append(listA, "blie");
    cstrlist_append(listA, "blo");
    cstrlist_append(listA, "blu");
    ASSERT(cstrlist_size(listA) == 5);

    cstrlist_remove_at(listA, 2);
    ASSERT(cstrlist_size(listA) == 4);

    cstrlist_remove_first(listA);
    cstrlist_remove_last(listA);
    ASSERT(cstrlist_size(listA) == 2);

    item = cstrlist_at(listA, 0);
    ASSERT(strcmp(c_str(item), "ble") == 0);

    item = cstr_new_size(32);
    cstrlist_join(listA, item, ",");
    ASSERT(strcmp(c_str(item), "ble,blo") == 0);
    cstr_free(item);

    cstrlist_free(listA);
}

#if 0
#define cstrlist_new() cstrlist_new_size(CSTRLIST_INITSIZE)
#define CStringListAuto GC_CLEANUP(_freeCStringList) CStringList
CString** cstrlist_data(CStringList *cslist);
void cstrlist_split(CStringList *cslist, const char *str, const char *sep,
int cstrlist_find(CStringList *cslist, const char *str, bool sensitive);
void cstrlist_sort(CStringList *cslist, bool sensitive);
void cstrlist_sort_func(CStringList *cslist, CCompareFunc compare);
#endif


