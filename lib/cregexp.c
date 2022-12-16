#include "cregexp.h"
#include <string.h>

#if 0
CRegExp::CRegExp()
{
}

CRegExp::CRegExp(const char *pattern)
{
    _re = pcre_compile(pattern,
                       _flags,
                       &_error,
                       &_erroroffset,
                       nullptr);
}

CRegExp::~CRegExp()
{
    if (_re)
        pcre_free(_re);
}

void CRegExp::clear()
{
    if (_re)
        pcre_free(_re);

    _re = nullptr;
    _instr = nullptr;
    _rc = 0;
    _ovector[1] = 0;
    _error = nullptr;
    _erroroffset = -1;
}

void CRegExp::setPattern(const char *pattern)
{
    clear();

    _re = pcre_compile(pattern,
                       _flags,
                       &_error,
                       &_erroroffset,
                       nullptr);
}

int CRegExp::indexIn(const char *str, int pos)
{
    if (!_re || !str)
        return -1;

    _instr = str;
    _rc = pcre_exec(_re, NULL, _instr, strlen(str),
                    pos, 0, _ovector, OVECCOUNT);

    if (_rc < 1)
        return -1;

    return _ovector[0];
}

int CRegExp::captureCount() const
{
    if (!_re)
        return 0;

    return _rc;
}

CString CRegExp::cap(int index)
{
    if (!_re || index < 0 || index >= _rc)
        return CString();

    const char *substart = _instr + _ovector[2*index];
    int sublength = _ovector[2*index+1] - _ovector[2*index];

    CString result(sublength + 1);
    result.append(substart, sublength);

    return result;
}

int CRegExp::matchedLength()
{
    if (!_re || _rc < 1)
        return -1;

    return _ovector[1] - _ovector[0];
}

#endif


