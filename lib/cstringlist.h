#ifndef CSTRINGLIST_H
#define CSTRINGLIST_H

#include "cstring.h"

#define CSTRLIST_INITSIZE 16

typedef struct _CStringList CStringList;

// allocation
#define cslist_new() cslist_new_size(CSTRLIST_INITSIZE)
CStringList* cslist_new_size(int size);
void cslist_resize(CStringList *cslist, int capacity);
void cslist_clear(CStringList *cslist);
void cslist_free(CStringList *cslist);
#define cslist_isempty(_a) (cslist_size(_a) < 1)

CString** cslist_data(CStringList *cslist);
int cslist_capacity(CStringList *cslist);
int cslist_size(CStringList *cslist);

// modify
#define cslist_append(_a, _s) cslist_append_len(_a, _s, strlen(_s))
void cslist_append_len(CStringList *cslist, const char *str, int length);
#define cslist_insert(_a, _i, _s) cslist_insert_len(_a, _i, _s, strlen(_s))
void cslist_insert_len(CStringList *cslist, int index, const char *str, int length);
void cslist_move(CStringList *cslist, int from, int index);

// read
CString* cslist_at(CStringList *cslist, int index);
int cslist_find(CStringList *cslist, const char *str, bool sensitive);
CString* cslist_takeAt(CStringList *cslist, int index);
CString* cslist_takeFirst(CStringList *cslist);
CString* cslist_takeLast(CStringList *cslist);
void cslist_removeAt(CStringList *cslist, int index);
void cslist_removeFirst(CStringList *cslist);
void cslist_removeLast(CStringList *cslist);

// transform
CString* cslist_join(CStringList *cslist, const char *sep);

void cslist_sort_func(CStringList *cslist,
                      int (*compare)(const void *, const void *));
//void cslist_sort(CStringList *cslist, bool sensitive);

#endif // CSTRINGLIST_H


