#ifndef LIBSTR_H
#define LIBSTR_H

#include <stdlib.h>
#include <stdbool.h>

#define STREOL_NONE 0
#define STREOL_LF 1
#define STREOL_CRLF 2
#define STREOL_CR 3

// Allocate -------------------------------------------------------------------

inline char* stralloc(int size)
{
    return (char*) malloc(size * sizeof(char));
}

inline wchar_t* wcsalloc(int size)
{
    return (wchar_t*) malloc(size * sizeof(wchar_t));
}

// Test -----------------------------------------------------------------------

bool str_startswith(const char *str, const char *part, bool sensitive);
bool str_endswith(const char *str, const char *part, bool sensitive);

// Browse ---------------------------------------------------------------------

bool str_getlineptr(char **start, char **result, int *length);

// Std C extensions -----------------------------------------------------------

char* stristr(const char *haystack, const char *needle);
char* strrstr(const char *haystack, const char *needle);
int streol(const char *str, int *pos);

// UTF8 -----------------------------------------------------------------------

int utf8len(const char *str);

#endif // LIBSTR_H


