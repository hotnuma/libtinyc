#include "cstring.h"
#include <assert.h>

#include <ctype.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>

#include "print.h"

// allocate -------------------------------------------------------------------

struct _CString
{
    char *buffer;
    int capacity;
    int length;
};

CString* cstr_alloc(int capacity, const char *str, int length)
{
    if (length < 0)
        length = strlen(str);

    CString *pstr = (CString*) malloc(sizeof(CString));

    pstr->capacity = capacity > length ? capacity : length + 1;
    pstr->buffer = (char*) malloc(pstr->capacity * sizeof(char));
    pstr->length = length;

    if (pstr->length > 0)
        memcpy(pstr->buffer, str, pstr->length);

    pstr->buffer[pstr->length] = '\0';

    return pstr;
}

void cstr_resize(CString *cstr, int capacity)
{
    if (capacity < 1 || capacity <= cstr->capacity)
        return;

    if (cstr->capacity < 1)
    {
        cstr->capacity = capacity;
    }
    else
    {
        while (cstr->capacity < capacity)
            cstr->capacity *= 2;
    }

    cstr->buffer = (char*) realloc(cstr->buffer, cstr->capacity * sizeof(char));
}

void cstr_free(CString *cstr)
{
    if (cstr == NULL)
        return;

    if (cstr->buffer)
        free(cstr->buffer);

    cstr->buffer = NULL;

    free(cstr);
}

void _freeCString(CString **cstr)
{
    print("_freeCString");

    cstr_free(*cstr);
}

// content --------------------------------------------------------------------

void cstr_clear(CString *cstr)
{
    cstr->length = 0;
    cstr->buffer[0] = '\0';
}

void cstr_swap(CString *cstr, CString *other)
{
    if (cstr == other)
        return;

    char *str = cstr->buffer;
    int cap = cstr->capacity;
    int len = cstr->length;

    cstr->buffer = other->buffer;
    cstr->capacity = other->capacity;
    cstr->length = other->length;

    other->buffer = str;
    other->capacity = cap;
    other->length = len;
}

void cstr_terminate(CString *cstr, int length)
{
    if (length == -1)
        length = strlen(cstr->buffer);

    if (length < 0 || length + 1 > cstr->capacity)
        return;

    cstr->length = length;
    cstr->buffer[cstr->length] = '\0';
}

char *cstr_data(CString *cstr)
{
    return cstr->buffer;
}

int cstr_capacity(CString *cstr)
{
    return cstr->capacity;
}

int cstr_size(CString *cstr)
{
    return cstr->length;
}

// edit -----------------------------------------------------------------------

void cstr_copy_len(CString *cstr, const char *str, int length)
{
    cstr_clear(cstr);
    cstr_append_len(cstr, str, length);
}

void cstr_append_len(CString *cstr, const char *str, int length)
{
    if (!str || !length)
        return;

    if (length < 0)
    {
        if ((length = strlen(str)) == 0)
            return;
    }

    if (cstr->length + length + 1 > cstr->capacity)
        cstr_resize(cstr, cstr->length + length + 1);

    memcpy(cstr->buffer + cstr->length, str, length);

    cstr->length += length;
    cstr->buffer[cstr->length] = '\0';
}

void cstr_append_c(CString *cstr, char c)
{
    cstr_append_len(cstr, &c, 1);
}

void cstr_remove(CString *cstr, int position, int n)
{
    if (position < 0 || position >= cstr->length || n < 1)
        return;

    int endpos = position + n;
    if (endpos >= cstr->length)
    {
        cstr_terminate(cstr, position);
        return;
    }

    char *src = cstr->buffer + endpos;
    char *dest = cstr->buffer + position;
    int num = cstr->length - endpos;

    memmove(dest, src, num);
    cstr_terminate(cstr, cstr->length - n);

    return;
}

void cstr_replace(CString *cstr, const char *find, const char *repl, bool sensitive)
{
    int flen = strlen(find);
    int rlen = strlen(repl);

    if (flen < 1)
        return;

    CString *result = cstr_new_size(cstr->length + 1);
    const char *p = cstr->buffer;

    bool found = false;

    while (*p)
    {
        if (sensitive)
        {
            found = (strncmp(p, find, flen) == 0);
        }
        else
        {
            found = (strncasecmp(p, find, flen) == 0);
        }

        if (found)
        {
            if (rlen > 0)
                cstr_append_len(result, repl, rlen);

            p += flen;
        }
        else
        {
            cstr_append_c(result, *p);

            ++p;
        }
    }

    free(cstr->buffer);

    cstr->buffer = result->buffer;
    cstr->length = result->length;
    cstr->capacity = result->capacity;

    free(result);
}

void cstr_chop(CString *cstr, int n)
{
    if (n < 1)
        return;

    int len = cstr->length - n;

    if (len < 0)
        len = 0;

    cstr_terminate(cstr, len);
}

bool cstr_trim(CString *cstr, CString *result)
{
    char *src = cstr->buffer;
    bool start = true;
    int skip = 0;
    int total = 0;

    cstr_clear(result);
    cstr_resize(result, cstr->length + 1);

    while (*src)
    {
        unsigned char c = *src;

        if (isspace(c))
        {
            if (start)
            {
                ++skip;
                ++src;
                continue;
            }
        }
        else
        {
            start = false;
            total = src + 1 - cstr->buffer;
        }

        ++src;
    }

    cstr_append_len(result, cstr->buffer + skip, total - skip);

    return true;
}

// https://stackoverflow.com/questions/4785381/replacement-for-ms-vscprintf-on-macos-linux

int _vscprintf(const char *format, va_list pargs)
{
    int retval;
    va_list argcopy;

    va_copy(argcopy, pargs);
    retval = vsnprintf(NULL, 0, format, argcopy);
    va_end(argcopy);

    return retval;
}

void cstr_fmt(CString *cstr, const char *fmt, ...)
{
    va_list va;
    va_start(va, fmt);

    int length = _vscprintf(fmt, va);

    cstr_clear(cstr);
    cstr_resize(cstr, length + 1);

    if (length > 0)
    {
        length = vsnprintf(cstr->buffer, length + 1, fmt, va);
        cstr_terminate(cstr, length);
    }

    va_end(va);
}

// extract --------------------------------------------------------------------

char cstr_first(CString *cstr)
{
    if (cstr->length < 1)
        return '\0';

    return cstr->buffer[0];
}

char cstr_last(CString *cstr)
{
    if (cstr->length < 1)
        return '\0';

    return cstr->buffer[cstr->length - 1];
}

bool cstr_left(CString *cstr, CString *result, int length)
{
    cstr_clear(result);

    if (length < 0 || length > cstr->length)
        return false;

    cstr_resize(result, length + 1);
    cstr_append_len(result, c_str(cstr), length);

    return true;
}

bool cstr_mid(CString *cstr, CString *result, int index, int length)
{
    cstr_clear(result);

    if (index < 0 || index > cstr->length)
        return false;

    if (length < 0)
        length = cstr->length - index;

    if ((index + length) > cstr->length)
        return false;

    cstr_resize(result, length + 1);
    cstr_append_len(result, cstr->buffer + index, length);

    return true;
}

// tests ----------------------------------------------------------------------

int cstr_compare(CString *cstr, const char *str, bool sensitive)
{
    assert(cstr->buffer);

    if (!str)
        return -1;

    if (sensitive)
        return strcmp(cstr->buffer, str);
    else
        return strcasecmp(cstr->buffer, str);
}

bool cstr_contains(CString *cstr, const char *str, bool sensitive)
{
    if (!str)
        return false;

    int len = strlen(str);

    const char *p = cstr->buffer;

    while (*p)
    {
        if (sensitive)
        {
            if (strncmp(p, str, len) == 0)
                return true;
        }
        else
        {
            if (strncasecmp(p, str, len) == 0)
                return true;
        }

        ++p;
    }

    return false;
}

bool cstr_startswith(CString *cstr, const char *str, bool sensitive)
{
    int len = strlen(str);

    if (sensitive)
        return (strncmp(cstr->buffer, str, len) == 0);
    else
        return (strncasecmp(cstr->buffer, str, len) == 0);
}

bool cstr_endswith(CString *cstr, const char *str, bool sensitive)
{
    int len = strlen(str);

    if (len < 1 || len > cstr->length)
        return false;

    const char *part = cstr->buffer + cstr->length - len;

    if (sensitive)
        return (strncmp(part, str, len) == 0);
    else
        return (strncasecmp(part, str, len) == 0);
}

// convert --------------------------------------------------------------------

int cstr_toint(CString *cstr)
{
    return atoi(cstr->buffer);
}

void cstr_tolower(CString *cstr)
{
    for (int i = 0; i < cstr->length; ++i)
        cstr->buffer[i] = tolower(cstr->buffer[i]);
}

void cstr_toupper(CString *cstr)
{
    for (int i = 0; i < cstr->length; ++i)
        cstr->buffer[i] = toupper(cstr->buffer[i]);
}

void cstr_int(CString *cstr, int val)
{
    int length = snprintf(NULL, 0, "%d", val);

    cstr_clear(cstr);
    cstr_resize(cstr, length + 1);

    snprintf(cstr->buffer, length + 1, "%d", val);

    cstr_terminate(cstr, length);
}

void cstr_uint64(CString *cstr, uint64_t val)
{
    int length = snprintf(NULL, 0, "%" PRIu64, val);

    cstr_clear(cstr);
    cstr_resize(cstr, length + 1);

    snprintf(cstr->buffer, length + 1, "%" PRIu64, val);

    cstr_terminate(cstr, length);
}

// read file ------------------------------------------------------------------

bool cstr_fileread(CString *cstr, const char *filepath)
{
    if (cstr->capacity < 1)
        return false;

    int fd = open(filepath, O_RDONLY);
    if (fd < 0)
        return false;

    cstr_clear(cstr);

    while (1)
    {
        int capacity = cstr->capacity;
        int length = cstr->length;
        int remain = capacity - length;

        while (remain < 2)
        {
            capacity *= 2;
            remain = capacity - length;
        }

        cstr_resize(cstr, capacity);

        char *buff = cstr->buffer + cstr->length;

        int nb_read = read(fd, buff, remain - 1);

        if (nb_read < 1)
            break;

        cstr_terminate(cstr, cstr->length + nb_read);
    }

    close(fd);

    return true;
}

bool str_getline(char **start, CString *result)
{
    // start of line.
    char *first = *start;

    // end of buffer ?
    if (*first == '\0')
        return false;

    // search end of line.
    char *p = first;

    while (1)
    {
        if (*p == '\r')
        {
            cstr_clear(result);
            cstr_append_len(result, first, p - first);
            //*result = first;
            //*length = p - first;

            // skip.
            if (*(p + 1) == '\n')
                ++p;

            // move to next line.
            *start = ++p;

            return true;
        }
        else if (*p == '\n')
        {
            cstr_clear(result);
            cstr_append_len(result, first, p - first);
            //*result = first;
            //*length = p - first;

            // move to next line.
            *start = ++p;

            return true;
        }
        else if (*p == '\0')
        {
            cstr_clear(result);
            cstr_append_len(result, first, p - first);
            //*result = first;
            //*length = p - first;

            // move to the end.
            *start = p;

            return true;
        }

        ++p;
    }
}

// utils ----------------------------------------------------------------------

void cstr_repeat(CString *cstr, const char *str, int count)
{
    if (!str || count < 1)
        return;

    int len = strlen(str);

    //CString *result = cstr_new_size((len * count) + 1);
    cstr_clear(cstr);
    cstr_resize(cstr, (len * count) + 1);

    while (count > 0)
    {
        cstr_append_len(cstr, str, len);

        --count;
    }
}

void cstr_enquote(CString *cstr, const char *str)
{
    if (!str || !*str)
        return;

    int len = strlen(str);

    cstr_clear(cstr);
    cstr_resize(cstr, len + 3);

    cstr_append_c(cstr, '\"');
    cstr_append_len(cstr, str, len);
    cstr_append_c(cstr, '\"');

    return;
}

void cstr_unquote(CString *cstr, const char *str)
{
    int len = strlen(str);

    cstr_clear(cstr);
    cstr_resize(cstr, len + 1);

    if (len < 2)
        return;

    char first = str[0];

    if ((first == '\'' || first == '"') && str[len-1] == first)
    {
        cstr_append_len(cstr, str + 1, len - 2);

        return;
    }

    cstr_append_len(cstr, str, len);
}

bool cstr_ellipsize(CString *cstr, int length, const char *part)
{
    int partlen = strlen(part);

    if (length < 2 || partlen < 1 || length <= partlen)
        return false;

    int size = cstr->length;

    if (size <= length)
        return true;

    length -= partlen;

    cstr_terminate(cstr, length);
    cstr_append(cstr, part);

    return true;
}

bool cstr_padleft(CString *cstr, int length, char c)
{
    int size = cstr->length;

    if (size < 1 || length < 1 || length <= size)
        return false;

    CString *buffer = cstr_new_size(length + 1);

    int delta = length - size;

    for (int i = 0; i < delta; ++i)
    {
        cstr_append_c(buffer, c);
    }

    cstr_append(buffer, c_str(cstr));
    cstr_swap(buffer, cstr);
    cstr_free(buffer);

    return true;
}

bool cstr_padright(CString *cstr, int length, char c)
{
    int delta = length - cstr->length;

    if (length < 1 || delta < 1)
        return false;

    cstr_resize(cstr, length + 1);

    for (int i = 0; i < delta; ++i)
    {
        cstr_append_c(cstr, c);
    }

    return true;
}


