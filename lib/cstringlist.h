#ifndef CSTRINGLIST_H
#define CSTRINGLIST_H

#include "cstring.h"

#define CSTRLIST_INITSIZE 16

typedef struct _CStringList CStringList;

struct _CStringList
{
    CString **data;
    int capacity;
    int size;
};

// allocation
CStringList* cslist_new_size(int size);
#define cslist_new() cslist_new_size(CSTRLIST_INITSIZE)
void cslist_clear(CStringList *cslist);
void cslist_free_data(CStringList *cslist);
void cslist_free(CStringList *cslist);

// size
void cslist_resize(CStringList *cslist, int capacity);
#define cslist_isempty(_a) (_a->size < 1);

// modify
void cslist_append(CStringList *cslist, const char *str, int length);
void cslist_insert(CStringList *cslist, int index, const char *str, int length);
void cslist_move(CStringList *cslist, int from, int index);

// read
CString* cslist_takeAt(CStringList *cslist, int index);
CString* cslist_takeFirst(CStringList *cslist);
CString* cslist_takeLast(CStringList *cslist);
void cslist_removeAt(CStringList *cslist, int index);
void cslist_removeFirst(CStringList *cslist);
void cslist_removeLast(CStringList *cslist);
CString* cslist_at(CStringList *cslist, int index);
int cslist_find(CStringList *cslist, const char *str, bool sensitive);

// transform
CString* cslist_join(CStringList *cslist, const char *sep);

void cslist_sort_func(CStringList *cslist,
                      int (*compare)(const void *, const void *));
void cslist_sort(CStringList *cslist, bool sensitive);

#endif // CSTRINGLIST_H


