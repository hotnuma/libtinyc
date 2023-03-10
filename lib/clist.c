#include "clist.h"

#include <stdlib.h>
#include <string.h>

// allocate -------------------------------------------------------------------

struct _CList
{
    void **data;
    int capacity;
    int size;

    CDeleteFunc deleteFunc;
};

CList* clist_new(int size, CDeleteFunc deleteFunc)
{
    CList *clist = (CList*) malloc(sizeof(CList));

    clist->data = malloc(size * sizeof(void*));
    clist->capacity = size;
    clist->size = 0;

    clist->deleteFunc = deleteFunc;

    return clist;
}

void clist_resize(CList *clist, int capacity)
{
    if (capacity < 1 || capacity <= clist->capacity)
        return;

    if (clist->capacity < 1)
    {
        clist->capacity = capacity;
    }
    else
    {
        while (clist->capacity < capacity)
            clist->capacity *= 2;
    }

    clist->data = (void**) realloc(clist->data, clist->capacity * sizeof(void*));
}

void clist_free(CList *clist)
{
    if (clist == NULL)
        return;

    clist_clear(clist);

    if (clist->data)
        free(clist->data);

    free(clist);
}

// content --------------------------------------------------------------------

void clist_clear(CList *clist)
{
    if (clist->data && clist->deleteFunc)
    {
        for (int i = 0; i < clist->size; ++i)
            clist->deleteFunc(clist->data[i]);
    }

    clist->size = 0;
}

void clist_swap(CList *clist, CList *other)
{
    void **data = clist->data;
    int capacity = clist->capacity;
    int size = clist->size;
    CDeleteFunc deleteFunc = clist->deleteFunc;

    clist->data = other->data;
    clist->capacity = other->capacity;
    clist->size = other->size;
    clist->deleteFunc = other->deleteFunc;

    other->data = data;
    other->capacity = capacity;
    other->size = size;
    other->deleteFunc = deleteFunc;
}

void** clist_data(CList *clist)
{
    return clist->data;
}

int clist_capacity(CList *clist)
{
    return clist->capacity;
}

int clist_size(CList *clist)
{
    return clist->size;
}

// edit -----------------------------------------------------------------------

void clist_append(CList *clist, void *ptr)
{
    clist_resize(clist, clist->size+1);

    clist->data[clist->size++] = ptr;
}

void clist_insert(CList *clist, int index, void *ptr)
{
    if (!clist->data || index < 0)
        return;

    if (index >= clist->size)
    {
        clist_append(clist, ptr);
        return;
    }

    clist_resize(clist, clist->size + 1);

    memmove(clist->data + index + 1, clist->data + index,
            (clist->size - index) * sizeof (void*));

    clist->data[index] = ptr;

    ++clist->size;
}

// parse ----------------------------------------------------------------------

const void* clist_at(CList *clist, int index)
{
    if (!clist->data || index < 0 || index >= clist->size)
        return NULL;

    return clist->data[index];
}

int clist_find(CList *clist, void *ptr)
{
    //int lsize = size();

    for (int i = 0; i < clist->size; ++i)
    {
        if (ptr == clist_at(clist, i))
            return i;
    }

    return -1;
}

void* clist_take_at(CList *clist, int index)
{
    if (!clist->data || clist->size < 1 || index < 0 || index >= clist->size)
        return NULL;

    void *ptr = clist->data[index];

    --clist->size;

    int num = clist->size - index;

    if (num > 0)
    {
        memmove(clist->data + index, clist->data + index + 1,
                num * sizeof (void*));
    }

    return ptr;
}

void clist_remove_at(CList *clist, int index)
{
    if (!clist->data || clist->size < 1 || index < 0 || index >= clist->size)
        return;

    void *ptr = clist->data[index];

    if (clist->deleteFunc)
        clist->deleteFunc(ptr);

    --clist->size;

    int num = clist->size - index;

    if (num > 0)
    {
        memmove(clist->data + index, clist->data + index + 1,
                num * sizeof (void*));
   }
}

// sort -----------------------------------------------------------------------

void clist_sort(CList *clist, CCompareFunc compare)
{
    qsort(clist->data, clist->size, sizeof(void*), compare);
}


