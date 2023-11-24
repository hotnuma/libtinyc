#include "libtest.h"
#include "clist.h"
#include <cstring.h>

int cmpfunc(const void *a, const void *b)
{
    int va = **(const int**) a;
    int vb = **(const int**) b;
    return (va > vb);
}

void test_clist()
{
    CList *listA = clist_new(10, (CDeleteFunc) cstr_free);
    ASSERT(clist_capacity(listA) == 10);
    ASSERT(clist_isempty(listA));

    clist_resize(listA, 20);
    ASSERT(clist_capacity(listA) == 20);

    clist_append(listA, cstr_new("bla"));
    clist_append(listA, cstr_new("ble"));
    clist_append(listA, cstr_new("blo"));
    clist_append(listA, cstr_new("blu"));
    ASSERT(clist_size(listA) == 4);

    clist_insert(listA, 2, cstr_new("blie"));
    CString *item = (CString*) clist_at(listA, 2);
    ASSERT(strcmp(c_str(item), "blie") == 0);

    item = clist_take_at(listA, 3);
    ASSERT(strcmp(c_str(item), "blo") == 0);
    ASSERT(clist_size(listA) == 4);
    cstr_free(item);

    item = clist_take_first(listA);
    ASSERT(strcmp(c_str(item), "bla") == 0);
    cstr_free(item);

    item = clist_take_last(listA);
    ASSERT(strcmp(c_str(item), "blu") == 0);
    cstr_free(item);

    clist_clear(listA);
    ASSERT(clist_size(listA) == 0);

    clist_append(listA, cstr_new("bla"));
    clist_append(listA, cstr_new("ble"));
    clist_append(listA, cstr_new("blie"));
    clist_append(listA, cstr_new("blo"));
    clist_append(listA, cstr_new("blu"));
    ASSERT(clist_size(listA) == 5);

    clist_remove_at(listA, 2);
    ASSERT(clist_size(listA) == 4);

    clist_remove_first(listA);
    clist_remove_last(listA);
    ASSERT(clist_size(listA) == 2);

    item = (CString*) clist_at(listA, 0);
    ASSERT(strcmp(c_str(item), "ble") == 0);

    clist_free(listA);
}

#if 0
#define CListAuto GC_CLEANUP(_freeCList) CList
void clist_swap(CList *clist, CList *other);
void** clist_data(CList *clist);
int clist_find(CList *clist, void *ptr);
#define clist_first(_a) (void*) clist_at(_a, 0)
#define clist_last(_a) (void*) clist_at(_a, clist_size(_a) - 1)
void clist_sort(CList *clist, CCompareFunc compare);
#endif


