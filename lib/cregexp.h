#ifndef CREGEXP_H
#define CREGEXP_H

#include "cstring.h"
#include "pcre.h"

#define OVECCOUNT 30

typedef struct _CRegExp CRegExp;

CRegExp* cregexp_new_pattern(const char *pattern);
void cregexp_free(CRegExp *regexp);

// auto free ------------------------------------------------------------------

#define CRegExpAuto _CCLEANUP(_freeCRegExp) CRegExp
static inline void _freeCRegExp(CRegExp **regexp)
{
    cregexp_free(*regexp);
}

void cregexp_clear(CRegExp *regexp);
void cregexp_set_pattern(CRegExp *regexp, const char *pattern);
int cregexp_indexin(CRegExp *regexp, const char *str, int pos);
int cregexp_capturecount(CRegExp *regexp);
bool cregexp_cap(CRegExp *regexp, CString *result, int index);
int cregexp_matchedlength(CRegExp *regexp);

#endif // CREGEXP_H


