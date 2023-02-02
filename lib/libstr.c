#include "libstr.h"

#include <ctype.h>
#include <string.h>

bool str_startswith(const char *str, const char *part, bool sensitive)
{
    int len = strlen(part);

    if (sensitive)
        return (strncmp(str, part, len) == 0);
    else
        return (strncasecmp(str, part, len) == 0);
}


bool str_getlineptr(char **start, char **result, int *length)
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
            *result = first;
            *length = p - first;

            // skip.
            if (*(p + 1) == '\n')
                ++p;

            // move to next line.
            *start = ++p;

            return true;
        }
        else if (*p == '\n')
        {
            *result = first;
            *length = p - first;

            // move to next line.
            *start = ++p;

            return true;
        }
        else if (*p == '\0')
        {
            *result = first;
            *length = p - first;

            // move to the end.
            *start = p;

            return true;
        }

        ++p;
    }
}

char* stristr(const char *haystack, const char *needle)
{
    char *pptr  = (char *) needle;   /* Pattern to search for    */
    char *start = (char *) haystack; /* Start with a bowl of hay */
    char *sptr;                      /* Substring pointer        */
    int   slen  = strlen (haystack); /* Total size of haystack   */
    int   plen  = strlen (needle);   /* Length of our needle     */

    /* while string length not shorter than pattern length */
    for (; slen >= plen; start++, slen--)
    {
        /* find start of pattern in string */
        while (toupper (*start) != toupper (*needle))
        {
            start++;
            slen--;

            /* if pattern longer than string */
            if (slen < plen)
            {
                return NULL;
            }
        }

        sptr = start;
        pptr = (char *) needle;
        while (toupper (*sptr) == toupper (*pptr))
        {
            sptr++;
            pptr++;

            /* if end of pattern then pattern was found */
            if ('\0' == *pptr)
            {
                return start;
            }
        }
    }

    return NULL;
}

/*
 * The strrstr() function finds the last occurrence of the substring needle
 * in the string haystack. The terminating nul characters are not compared.
 */
char* strrstr(const char *haystack, const char *needle)
{
    char *r = NULL;

    if (!needle[0])
        return (char*)haystack + strlen(haystack);

    while (1)
    {
        char *p = (char*) strstr(haystack, needle);
        if (!p)
            return r;

        r = p;
        haystack = p + 1;
    }
}

// Detect eol.
int streol(const char *str, int *pos)
{
    if (!str)
        return STREOL_NONE;

    const char *p = str;
    while (*p)
    {
        if (*p == '\r')
        {
            if (*(p + 1) == '\n')
            {
                if (pos)
                    *pos = (p - str);

                return STREOL_CRLF;
            }
            else
            {
                if (pos)
                    *pos = (p - str);

                return STREOL_CR;
            }
        }
        else if (*p == '\n')
        {
            if (pos)
                *pos = (p - str);

            return STREOL_LF;
        }

        ++p;
    }

    if (pos)
        *pos = -1;

    return STREOL_NONE;
}

int utf8len(const char *str)
{
    int count = 0;

    while (*str)
    {
        if ((*str & 0xC0) != 0x80)
            ++count;
        ++str;
    }

    return count;
}


