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


#if 0
cregexp_CRegExp()
{
}

cregexp_~CRegExp()
{
    if (regexp->re)
        pcre_free(regexp->re);
}

void cregexp_setPattern(const char *pattern)
{
    clear();

    regexp->re = pcre_compile(pattern,
                       regexp->flags,
                       &regexp->error,
                       &regexp->erroroffset,
                       NULL);
}

int cregexp_indexIn(const char *str, int pos)
{
    if (!regexp->re || !str)
        return -1;

    regexp->instr = str;
    regexp->rc = pcre_exec(regexp->re, NULL, regexp->instr, strlen(str),
                    pos, 0, regexp->ovector, OVECCOUNT);

    if (regexp->rc < 1)
        return -1;

    return regexp->ovector[0];
}

int cregexp_captureCount() const
{
    if (!regexp->re)
        return 0;

    return regexp->rc;
}

CString cregexp_cap(int index)
{
    if (!regexp->re || index < 0 || index >= regexp->rc)
        return CString();

    const char *substart = regexp->instr + regexp->ovector[2*index];
    int sublength = regexp->ovector[2*index+1] - regexp->ovector[2*index];

    CString result(sublength + 1);
    result.append(substart, sublength);

    return result;
}

int cregexp_matchedLength()
{
    if (!regexp->re || regexp->rc < 1)
        return -1;

    return regexp->ovector[1] - regexp->ovector[0];
}

#endif


