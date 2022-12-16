#ifndef CREGEXP_H
#define CREGEXP_H

#include "cstring.h"
#include "pcre.h"

#define OVECCOUNT 30

#if 0
class CRegExp
{
public:

    CRegExp();
    CRegExp(const char *pattern);
    ~CRegExp();

    void clear();
    void setPattern(const char *pattern);

    int indexIn(const char *str, int pos = 0);
    int captureCount() const;
    CString cap(int num);
    int matchedLength();

private:

    pcre *_re = NULL;
    int _flags = PCRE_MULTILINE | PCRE_UTF8 | PCRE_UCP
                 | PCRE_DOTALL | PCRE_NEWLINE_ANYCRLF;

    const char *_instr = NULL;
    int _rc = 0;
    int _ovector[OVECCOUNT];

    const char *_error = NULL;
    int _erroroffset = -1;

};

#endif

#endif // CREGEXP_H


