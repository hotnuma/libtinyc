#ifndef CLIST_H
#define CLIST_H

#include "macros.h"

#define CLIST_INITSIZE 16

typedef struct _CList CList;

// allocate -------------------------------------------------------------------

#define clist_new() clist_new_func(CLIST_INITSIZE, NULL)
CList* clist_new_func(int size, CDeleteFunc deleteFunc);
void clist_set_deletefunc(CList *clist, CDeleteFunc deleteFunc);
void clist_resize(CList *clist, int capacity);
void clist_free(CList *clist);

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


