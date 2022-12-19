#ifndef CSTRINGLIST_H
#define CSTRINGLIST_H

#include "cstring.h"

#define CSTRLIST_INITSIZE 16

typedef struct _CStringList CStringList;

// allocate -------------------------------------------------------------------

#define cstrlist_new() cstrlist_new_size(CSTRLIST_INITSIZE)
CStringList* cstrlist_new_size(int size);
void cstrlist_resize(CStringList *cslist, int capacity);
void cstrlist_free(CStringList *cslist);
#define cstrlist_isempty(_a) (cstrlist_size(_a) < 1)

// content --------------------------------------------------------------------

void cstrlist_clear(CStringList *cslist);
CString** cstrlist_data(CStringList *cslist);
int cstrlist_capacity(CStringList *cslist);
int cstrlist_size(CStringList *cslist);

// edit -----------------------------------------------------------------------

#define cstrlist_append(_a, _s) cstrlist_append_len(_a, _s, strlen(_s))
void cstrlist_append_len(CStringList *cslist, const char *str, int length);
#define cstrlist_insert(_a, _i, _s) cstrlist_insert_len(_a, _i, _s, strlen(_s))
void cstrlist_insert_len(CStringList *cslist, int index, const char *str, int length);
void cstrlist_move(CStringList *cslist, int from, int index);
void cstrlist_split(CStringList *cslist, CString *cstr, const char *sep,
                    bool keepEmptyParts, bool sensitive);
void cstrlist_join(CStringList *cslist, CString *result, const char *sep);

// parse ----------------------------------------------------------------------

CString* cstrlist_at(CStringList *cslist, int index);
int cstrlist_find(CStringList *cslist, const char *str, bool sensitive);
CString* cstrlist_take_at(CStringList *cslist, int index);
#define cstrlist_take_first(_a) cstrlist_take_at(_a, 0)
#define cstrlist_take_last(_a) cstrlist_take_at(_a, cstrlist_size(_a) -  1)
void cstrlist_remove_at(CStringList *cslist, int index);
#define cstrlist_remove_first(_a) cstrlist_remove_at(_a, 0)
#define cstrlist_remove_last(_a) cstrlist_remove_at(_a, cstrlist_size(_a) -  1)

// sort -----------------------------------------------------------------------

void cstrlist_sort(CStringList *cslist, bool sensitive);
void cstrlist_sort_func(CStringList *cslist, CCompareFunc compare);

#endif // CSTRINGLIST_H


