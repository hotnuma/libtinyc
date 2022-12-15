#include "cstringlist.h"

#include <stdlib.h>
#include <string.h>

// allocation

struct _CStringList
{
    CString **data;
    int capacity;
    int size;
};

CStringList* cslist_new_size(int size)
{
    CStringList *pslist = (CStringList*) malloc(sizeof(CStringList));

    pslist->capacity = size;
    pslist->data = (CString**) malloc(pslist->capacity * sizeof(void*));

    return pslist;
}

void cslist_resize(CStringList *cslist, int capacity)
{
    if (capacity < 1 || capacity <= cslist->capacity)
        return;

    if (cslist->capacity < 1)
    {
        cslist->capacity = capacity;
    }
    else
    {
        //while (cslist->capacity <= capacity)
        //    cslist->capacity *= 2;

        while (cslist->capacity < capacity)
            cslist->capacity *= 2;
    }

    cslist->data = (CString**) realloc(cslist->data, cslist->capacity * sizeof(void*));
}

void cslist_clear(CStringList *cslist)
{
    if (cslist->data)
    {
        for (int i = 0; i < cslist->size; ++i)
            free(cslist->data[i]);
    }

    cslist->size = 0;
}

void cslist_free_data(CStringList *cslist)
{
    if (cslist == NULL)
        return;

    cslist_clear(cslist);

    if (cslist->data)
        free(cslist->data);

    cslist->data = NULL;
}

void cslist_free(CStringList *cslist)
{
    if (cslist == NULL)
        return;

    cslist_clear(cslist);

    if (cslist->data)
        free(cslist->data);

    cslist->data = NULL;

    free(cslist);
}

// accessors

CString** cslist_data(CStringList *cslist)
{
    return cslist->data;
}

int cslist_capacity(CStringList *cslist)
{
    return cslist->capacity;
}

int cslist_size(CStringList *cslist)
{
    return cslist->size;
}

// modify.

void cslist_append_len(CStringList *cslist, const char *str, int length)
{
    if (!str)
        return;

    cslist_resize(cslist, cslist->size + 1);

    cslist->data[cslist->size++] = cstr_new_len(str, length);
}

void cslist_insert_len(CStringList *cslist, int index, const char *str, int length)
{
    if (!cslist->data || cslist->size < 1 || index < 0 || index >= cslist->size)
    {
        cslist_append_len(cslist, str, length);

        return;
    }

    cslist_resize(cslist, cslist->size + 1);

    memmove(cslist->data + index + 1, cslist->data + index,
            (cslist->size - index) * sizeof(CString*));

    cslist->data[index] = cstr_new_len(str, length);

    ++cslist->size;
}

void cslist_move(CStringList *cslist, int from, int index)
{
    if (!cslist->data || cslist->size < 2)
        return;

    if (index < 0)
        index = 0;

    CString *str = cslist_takeAt(cslist, from);
    if (!str)
        return;

    if (index >= cslist->size)
    {
        // append.
        cslist_resize(cslist, cslist->size + 1);
        cslist->data[cslist->size++] = str;

        return;
    }

    cslist_resize(cslist, cslist->size + 1);

    memmove(cslist->data + index + 1, cslist->data + index,
            (cslist->size - index) * sizeof(CString*));

    cslist->data[index] = str;

    ++cslist->size;
}

// read
CString* cslist_takeAt(CStringList *cslist, int index)
{
    if (!cslist->data || cslist->size < 1 || index < 0 || index >= cslist->size)
        return NULL;

    CString *ptr = cslist->data[index];

    --cslist->size;

    int num = cslist->size - index;

    if (num > 0)
    {
        memmove(cslist->data + index, cslist->data + index + 1,
                num * sizeof(void*));
    }

    return ptr;
}

CString* cslist_takeFirst(CStringList *cslist)
{
    return cslist_takeAt(cslist, 0);
}

CString* cslist_takeLast(CStringList *cslist)
{
    return cslist_takeAt(cslist, cslist->size - 1);
}

void cslist_removeAt(CStringList *cslist, int index)
{
    if (!cslist->data || cslist->size < 1 || index < 0 || index >= cslist->size)
        return;

    CString *str = cslist->data[index];
    if (str)
        free(str);

    --cslist->size;

    int num = cslist->size - index;

    if (num > 0)
    {
        memmove(cslist->data + index, cslist->data + index + 1,
                num * sizeof(void*));
    }
}

void cslist_removeFirst(CStringList *cslist)
{
    cslist_removeAt(cslist, 0);
}

void cslist_removeLast(CStringList *cslist)
{
    cslist_removeAt(cslist, cslist->size - 1);
}

CString* cslist_at(CStringList *cslist, int index)
{
    if (!cslist->data || index < 0 || index >= cslist->size)
        return NULL;

    return cslist->data[index];
}

int cslist_find(CStringList *cslist, const char *str, bool sensitive)
{
    for (int i = 0; i < cslist->size; ++i)
    {
        CString *current = cslist->data[i];

        if (cstr_compare(current, str, sensitive) == 0)
            return i;
    }

    return -1;
}

// transform
CString* cslist_join(CStringList *cslist, const char *sep)
{
    CString *result = cstr_new_size(32);

    //int nsize = size();

    if (cslist->size < 1)
        return result;

    int len = strlen(sep);

    for (int i = 0; i < cslist->size; ++i)
    {
        if (i > 0)
            cstr_append_len(result, sep, len);

        cstr_append(result, c_str(cslist->data[i]));
    }

    return result;
}

// sort

int _compare(void *entry1, void *entry2)
{
    CString *e1 = *((CString**) entry1);
    CString *e2 = *((CString**) entry2);

    return cstr_compare(e1, c_str(e2), true);
}

int _icompare(void *entry1, void *entry2)
{
    CString *e1 = *((CString**) entry1);
    CString *e2 = *((CString**) entry2);

    return cstr_compare(e1, c_str(e2), false);
}

void cslist_sort_func(CStringList *cslist,
                      int (*compare)(const void *, const void *))
{
    qsort(cslist->data, cslist->size, sizeof(void*), compare);
}

//void cslist_sort(CStringList *cslist, bool sensitive)
//{
//    if (sensitive)
//        qsort(cslist->data, cslist->size, sizeof(void*), _compare);
//    else
//        qsort(cslist->data, cslist->size, sizeof(void*), _icompare);
//}


