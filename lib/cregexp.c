#include "cregexp.h"
#include <string.h>

struct _CRegExp
{
    pcre *re;

    int flags;
    const char *error;
    int erroroffset;

    const char *instr;
    int rc;
    int ovector[OVECCOUNT];
};

CRegExp* cregexp_new_pattern(const char *pattern)
{
    CRegExp *regexp = (CRegExp*) malloc(sizeof(CRegExp));

    regexp->flags = PCRE_MULTILINE | PCRE_UTF8 | PCRE_UCP
                    | PCRE_DOTALL | PCRE_NEWLINE_ANYCRLF;

    regexp->error = NULL;
    regexp->erroroffset = -1;

    regexp->re = pcre_compile(pattern,
                              regexp->flags,
                              &regexp->error,
                              &regexp->erroroffset,
                              NULL);

    regexp->instr = NULL;
    regexp->rc = 0;
    regexp->ovector[1] = 0;

    return regexp;
}

void cregexp_clear(CRegExp *regexp)
{
    regexp->flags = PCRE_MULTILINE | PCRE_UTF8 | PCRE_UCP
                    | PCRE_DOTALL | PCRE_NEWLINE_ANYCRLF;

    regexp->error = NULL;
    regexp->erroroffset = -1;

    if (regexp->re)
        pcre_free(regexp->re);

    regexp->re = NULL;

    regexp->instr = NULL;
    regexp->rc = 0;
    regexp->ovector[1] = 0;
}

void cregexp_free(CRegExp *regexp)
{
    if (regexp == NULL)
        return;

    if (regexp->re)
        pcre_free(regexp->re);
}

void cregexp_set_pattern(CRegExp *regexp, const char *pattern)
{
    cregexp_clear(regexp);

    regexp->re = pcre_compile(pattern,
                              regexp->flags,
                              &regexp->error,
                              &regexp->erroroffset,
                              NULL);
}

int cregexp_indexin(CRegExp *regexp, const char *str, int pos)
{
    if (!regexp->re || !str)
        return -1;

    regexp->instr = str;

    regexp->rc = pcre_exec(regexp->re,
                           NULL,
                           regexp->instr,
                           strlen(str),
                           pos,
                           0,
                           regexp->ovector,
                           OVECCOUNT);

    if (regexp->rc < 1)
        return -1;

    return regexp->ovector[0];
}

int cregexp_capturecount(CRegExp *regexp)
{
    if (!regexp->re)
        return 0;

    return regexp->rc;
}

bool cregexp_cap(CRegExp *regexp, CString *result, int index)
{
    if (!regexp->re || index < 0 || index >= regexp->rc)
        return false;

    const char *substart = regexp->instr + regexp->ovector[2*index];
    int sublength = regexp->ovector[2*index+1] - regexp->ovector[2*index];

    cstr_resize(result, sublength + 1);
    cstr_copy_len(result, substart, sublength);

    return true;
}

int cregexp_matchedlength(CRegExp *regexp)
{
    if (!regexp->re || regexp->rc < 1)
        return -1;

    return regexp->ovector[1] - regexp->ovector[0];
}


