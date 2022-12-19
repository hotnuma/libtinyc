#include "libtest.h"
#include "clist.h"

#include <stdlib.h>
#include <cstring.h>
#include "print.h"

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
// untested ---------------------------------------------------------
//
//CList(int size);

int g_count = 0;

void entryDelete(int *item)
{
    if (!item)
        return;

    delete item;

    ++g_count;
}

void test_CList()
{
    CList listA;
    listA.setDeleteFunc((CDeleteFunc) entryDelete);
    ASSERT(listA.isEmpty() == true);

    for (int i = 0; i < 10; ++i)
    {
        listA.append(new int(i));
    }
    ASSERT(listA.size() == 10);

    // 0 1 2 3 4 5 6 7 8 9
    
    int *val = NULL;

    val = (int*) listA.takeAt(4);
    ASSERT(*val == 4);
    delete val;
    listA.removeAt(4);
    val = (int*) listA.at(4);
    ASSERT(*val == 6);

    // 0 1 2 3 6 7 8 9

    val = (int*) listA.takeFirst();
    ASSERT(*val == 0);
    delete val;
    listA.removeFirst();
    val = (int*) listA.first();
    ASSERT(*val == 2);

    val = (int*) listA.takeLast();
    ASSERT(*val == 9);
    delete val;
    listA.removeLast();
    val = (int*) listA.last();
    ASSERT(*val == 7);

    // 2 3 6 7
    
    int *item = new int(-1);
    listA.insert(2, item);
    ASSERT(*((int*) listA[2]) == -1);
    ASSERT(listA.find(item) == 2);

    CList *listB = new CList;
    listB->setDeleteFunc((CDeleteFunc) entryDelete);
    listB->swap(listA);
    ASSERT(listA.size() == 0);
    ASSERT(listB->size() == 5);

    int size = listB->capacity();
    listB->resize(size + 1);
    ASSERT(listB->capacity() == (size * 2));

    listB->sort(cmpfunc);
    item = (int*) listB->first();
    ASSERT(*item == -1);

    g_count = 0;
    listA.append(new int(0));
    ASSERT(listA.size() == 1);
    listA.clear();
    ASSERT(g_count == 1);

    g_count = 0;
    delete listB;
    ASSERT(g_count == 5);

}

#endif


