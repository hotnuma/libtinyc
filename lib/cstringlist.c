#include "cstringlist.h"

#include <stdlib.h>
#include <string.h>

void cstr_split(CString *cstr, CStringList *list, const char *sep,
                bool keepEmptyParts, bool sensitive)
{
    int slen = strlen(sep);

    const char *start = cstr_data(cstr);
    const char *c = start;
    int len = 0;

    while (1)
    {
        if (*c == '\0')
        {
            len = c - start;

            if (len || keepEmptyParts)
                cstrlist_append_len(list, start, len);

            break;
        }

        if (sensitive)
        {
            if (strncmp(c, sep, slen) == 0)
            {
                len = c - start;

                if (len > 0 || keepEmptyParts)
                    cstrlist_append_len(list, start, len);

                c += slen;
                start = c;

                continue;
            }
        }
        else
        {
            if (strncasecmp(c, sep, slen) == 0)
            {
                len = c - start;

                if (len > 0 || keepEmptyParts)
                    cstrlist_append_len(list, start, len);

                c += slen;
                start = c;

                continue;
            }
        }

        ++c;
    }
}

// allocation

struct _CStringList
{
    CString **data;
    int capacity;
    int size;
};

CStringList* cstrlist_new_size(int size)
{
    CStringList *pslist = (CStringList*) malloc(sizeof(CStringList));

    pslist->data = (CString**) malloc(size * sizeof(void*));
    pslist->capacity = size;
    pslist->size = 0;

    return pslist;
}

void cstrlist_resize(CStringList *cslist, int capacity)
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

void cstrlist_clear(CStringList *cslist)
{
    if (cslist->data)
    {
        for (int i = 0; i < cslist->size; ++i)
            cstr_free(cslist->data[i]);
    }

    cslist->size = 0;
}

void cstrlist_free(CStringList *cslist)
{
    if (cslist == NULL)
        return;

    cstrlist_clear(cslist);

    if (cslist->data)
        free(cslist->data);

    cslist->data = NULL;

    free(cslist);
}

// accessors

CString** cstrlist_data(CStringList *cslist)
{
    return cslist->data;
}

int cstrlist_capacity(CStringList *cslist)
{
    return cslist->capacity;
}

int cstrlist_size(CStringList *cslist)
{
    return cslist->size;
}

// modify.

void cstrlist_append_len(CStringList *cslist, const char *str, int length)
{
    if (!str)
        return;

    cstrlist_resize(cslist, cslist->size + 1);

    cslist->data[cslist->size++] = cstr_new_len(str, length);
}

void cstrlist_insert_len(CStringList *cslist, int index, const char *str, int length)
{
    if (!cslist->data || index < 0 || index > cslist->size)
        return;

    if (cslist->size == 0 || index == cslist->size)
    {
        cstrlist_append_len(cslist, str, length);

        return;
    }

    cstrlist_resize(cslist, cslist->size + 1);

    memmove(cslist->data + index + 1,
            cslist->data + index,
            (cslist->size - index) * sizeof(CString*));

    cslist->data[index] = cstr_new_len(str, length);

    ++cslist->size;
}

void cstrlist_move(CStringList *cslist, int from, int index)
{
    if (!cslist->data || cslist->size < 2)
        return;

    if (index < 0)
        index = 0;

    CString *str = cstrlist_takeAt(cslist, from);
    if (!str)
        return;

    if (index >= cslist->size)
    {
        // append.
        cstrlist_resize(cslist, cslist->size + 1);
        cslist->data[cslist->size++] = str;

        return;
    }

    cstrlist_resize(cslist, cslist->size + 1);

    memmove(cslist->data + index + 1, cslist->data + index,
            (cslist->size - index) * sizeof(CString*));

    cslist->data[index] = str;

    ++cslist->size;
}

// read
CString* cstrlist_takeAt(CStringList *cslist, int index)
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

CString* cstrlist_takeFirst(CStringList *cslist)
{
    return cstrlist_takeAt(cslist, 0);
}

CString* cstrlist_takeLast(CStringList *cslist)
{
    return cstrlist_takeAt(cslist, cslist->size - 1);
}

void cstrlist_removeAt(CStringList *cslist, int index)
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

void cstrlist_removeFirst(CStringList *cslist)
{
    cstrlist_removeAt(cslist, 0);
}

void cstrlist_removeLast(CStringList *cslist)
{
    cstrlist_removeAt(cslist, cslist->size - 1);
}

CString* cstrlist_at(CStringList *cslist, int index)
{
    if (!cslist->data || index < 0 || index >= cslist->size)
        return NULL;

    return cslist->data[index];
}

int cstrlist_find(CStringList *cslist, const char *str, bool sensitive)
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
CString* cstrlist_join(CStringList *cslist, const char *sep)
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

void cstrlist_sort_func(CStringList *cslist,
                      int (*compare)(const void *, const void *))
{
    qsort(cslist->data, cslist->size, sizeof(void*), compare);
}

void cstrlist_sort(CStringList *cslist, bool sensitive)
{
    if (sensitive)
        qsort(cslist->data, cslist->size, sizeof(void*), _compare);
    else
        qsort(cslist->data, cslist->size, sizeof(void*), _icompare);
}


