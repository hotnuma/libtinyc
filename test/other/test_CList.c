#include "CList.h"
#include "libtest.h"

#include "print.h"

// untested ---------------------------------------------------------
//
//CList(int size);

int g_count = 0;

int cmpfunc(const void *a, const void *b)
{
    int va = **(const int**) a;
    int vb = **(const int**) b;
    return (va > vb);
}

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
    
    int *val = nullptr;

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


