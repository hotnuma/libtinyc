#ifndef CREGEXP_H
#define CREGEXP_H

#include "cstring.h"
#include "pcre.h"

#define OVECCOUNT 30

typedef struct _CRegExp CRegExp;

CRegExp* cregexp_new_pattern(const char *pattern);
void cregexp_clear(CRegExp *regexp);

#if 0

CRegExp();
~CRegExp();

void setPattern(const char *pattern);

int indexIn(const char *str, int pos = 0);
int captureCount() const;
CString cap(int num);
int matchedLength();

#endif

#endif // CREGEXP_H


