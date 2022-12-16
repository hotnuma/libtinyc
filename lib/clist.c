#include "clist.h"

#include <stdlib.h>
#include <string.h>

struct _CList
{
    void **data;
    int capacity;
    int size;

    CDeleteFunc deleteFunc;
};

// construct

CList* clist_new_size(int size)
{
    CList *clist = (CList*) malloc(sizeof(CList));

    clist->data = malloc(size * sizeof(void*));
    clist->capacity = size;
    clist->size = 0;

    clist->deleteFunc = NULL;

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
        //while (clist->capacity <= capacity)
        //    clist->capacity *= 2;

        while (clist->capacity < capacity)
            clist->capacity *= 2;
    }

    clist->data = (void**) realloc(clist->data, clist->capacity * sizeof(void*));
}

void clist_set_deletefunc(CList *clist, CDeleteFunc deleteFunc)
{
    clist->deleteFunc = deleteFunc;
}

void clist_clear(CList *clist)
{
    if (clist->data && clist->deleteFunc)
    {
        for (int i = 0; i < clist->size; ++i)
            clist->deleteFunc(clist->data[i]);
    }

    clist->size = 0;
}

void clist_free(CList *clist)
{
    clist_clear(clist);

    if (clist->data)
        free(clist->data);

    free(clist);
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

// read

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

// modify

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

void clist_removeAt(CList *clist, int index)
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

void* clist_takeAt(CList *clist, int index)
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

// sort

//void clist_sort(int (*compare) (const void *, const void *))
//{
//    qsort(clist->data, size(), sizeof(void*), compare);
//}


