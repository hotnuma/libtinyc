#ifndef CLIST_H
#define CLIST_H

//#include "deletefunc.h"

#define CLIST_INITSIZE 16

typedef struct _CList CList;

struct _CList
{
    void** data;
    int capacity;
    int size;

    //CDeleteFunc _deleteFunc = nullptr;
};

// construct

#define clist_new() clist_new_size(CLIST_INITSIZE)
CList* clist_new_size(int size);
void clist_clear(CList *clist);
void clist_free(CList *clist);

#define clist_isempty(_a) (_a->_size < 1);

void clist_resize(CList *clist, int capacity);

const void* clist_at(CList *clist, int index);
#define clist_first(_a) (void*) clist_at(_a, 0)
#define clist_last(_a) (void*) clist_at(_a, _a->size - 1)

int clist_find(CList *clist, void *ptr);
void clist_append(CList *clist, void *ptr);
void clist_insert(CList *clist, int index, void *ptr);
void clist_swap(CList *clist, CList *other);

void clist_removeAt(CList *clist, int index);
#define clist_removeFirst(_a) clist_removeAt(_a, 0)
#define clist_removeLast(_a) clist_removeAt(_a, _a->size - 1)

void* clist_takeAt(CList *clist, int index);
#define clist_takeFirst(_a) clist_takeAt(_a, 0)
#define clist_takeLast(_a) clist_takeAt(_a, _a->size - 1)

#if 0

    // clear / size
    void setDeleteFunc(CDeleteFunc deleteFunc);

    // sort
    void sort(int (*compare)(const void *, const void *));

#endif

#endif // CLIST_H


