#ifndef CREGEXP_H
#define CREGEXP_H

#include "CString.h"
#include "pcre.h"

#define OVECCOUNT 30

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

    pcre *_re = nullptr;
    int _flags = PCRE_MULTILINE | PCRE_UTF8 | PCRE_UCP
                 | PCRE_DOTALL | PCRE_NEWLINE_ANYCRLF;

    const char *_instr = nullptr;
    int _rc = 0;
    int _ovector[OVECCOUNT];

    const char *_error = nullptr;
    int _erroroffset = -1;

};

#endif // CREGEXP_H


