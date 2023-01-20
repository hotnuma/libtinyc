#ifndef CLIST_H
#define CLIST_H

#include "macros.h"

typedef struct _CList CList;

// allocate -------------------------------------------------------------------

CList* clist_new(int size, CDeleteFunc deleteFunc);
void clist_resize(CList *clist, int capacity);
void clist_free(CList *clist);

// auto free ------------------------------------------------------------------

#define CListAuto GC_CLEANUP(_freeCList) CList
GC_UNUSED static inline void _freeCList(CList **clist)
{
    clist_free(*clist);
}

// content --------------------------------------------------------------------

#define clist_isempty(_a) (clist_size(_a) < 1)
void clist_clear(CList *clist);
void clist_swap(CList *clist, CList *other);
void** clist_data(CList *clist);
int clist_capacity(CList *clist);
int clist_size(CList *clist);

// edit -----------------------------------------------------------------------

void clist_append(CList *clist, void *ptr);
void clist_insert(CList *clist, int index, void *ptr);

// parse ----------------------------------------------------------------------

const void* clist_at(CList *clist, int index);
int clist_find(CList *clist, void *ptr);
#define clist_first(_a) (void*) clist_at(_a, 0)
#define clist_last(_a) (void*) clist_at(_a, clist_size(_a) - 1)
void* clist_take_at(CList *clist, int index);
#define clist_take_first(_a) clist_take_at(_a, 0)
#define clist_take_last(_a) clist_take_at(_a, clist_size(_a) - 1)
void clist_remove_at(CList *clist, int index);
#define clist_remove_first(_a) clist_remove_at(_a, 0)
#define clist_remove_last(_a) clist_remove_at(_a, clist_size(_a) - 1)

// sort -----------------------------------------------------------------------

void clist_sort(CList *clist, CCompareFunc compare);

#endif // CLIST_H


