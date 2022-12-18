#ifndef CSTRINGLIST_H
#define CSTRINGLIST_H

#include "cstring.h"

#define CSTRLIST_INITSIZE 16

typedef struct _CStringList CStringList;

// CString split function
void cstr_split(CString *cstr, CStringList *list, const char *sep,
                bool keepEmptyParts, bool sensitive);

// allocation
#define cstrlist_new() cstrlist_new_size(CSTRLIST_INITSIZE)
CStringList* cstrlist_new_size(int size);
void cstrlist_resize(CStringList *cslist, int capacity);
void cstrlist_clear(CStringList *cslist);
void cstrlist_free(CStringList *cslist);
#define cstrlist_isempty(_a) (cstrlist_size(_a) < 1)

CString** cstrlist_data(CStringList *cslist);
int cstrlist_capacity(CStringList *cslist);
int cstrlist_size(CStringList *cslist);

// modify
#define cstrlist_append(_a, _s) cstrlist_append_len(_a, _s, strlen(_s))
void cstrlist_append_len(CStringList *cslist, const char *str, int length);
#define cstrlist_insert(_a, _i, _s) cstrlist_insert_len(_a, _i, _s, strlen(_s))
void cstrlist_insert_len(CStringList *cslist, int index, const char *str, int length);
void cstrlist_move(CStringList *cslist, int from, int index);

// read
CString* cstrlist_at(CStringList *cslist, int index);
int cstrlist_find(CStringList *cslist, const char *str, bool sensitive);
CString* cstrlist_takeAt(CStringList *cslist, int index);
CString* cstrlist_takeFirst(CStringList *cslist);
CString* cstrlist_takeLast(CStringList *cslist);
void cstrlist_removeAt(CStringList *cslist, int index);
void cstrlist_removeFirst(CStringList *cslist);
void cstrlist_removeLast(CStringList *cslist);

// transform
CString* cstrlist_join(CStringList *cslist, const char *sep);

void cstrlist_sort_func(CStringList *cslist,
                      int (*compare)(const void *, const void *));
void cstrlist_sort(CStringList *cslist, bool sensitive);

#endif // CSTRINGLIST_H


