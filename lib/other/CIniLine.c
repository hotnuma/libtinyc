#include "CIniLine.h"
#include <ctype.h>
#include <string.h>

//#include "print.h"

CIniLine::CIniLine(char *line)
{
    _setLine(line);
}

void CIniLine::_setLine(char *line)
{
    _type = CLineType::Undefined;
    _vtype = CValueType::Undefined;
    _line.clear();
    _value.clear();

    char *p = line;

    // skip blanks.
    while (isspace(*p)) ++p;

    // comments.
    if (*p == ';' || *p == '#')
    {
        _type = CLineType::Comment;
        _line = p;
        return;
    }

    // empty.
    else if (*p == '\0')
    {
        _type = CLineType::Empty;
        _line.clear();
        return;
    }

    // search a key.
    char *key = p;
    int len = 0;

    while (isalnum(*p) || *p == '_' || *p == '.') ++p;

    // no alphanum or nothing after key.
    if (p == key || *p == '\0')
        goto error;

    // set key.
    _line.append(key, p - key);

    // search '=' char.

    while (isspace(*p)) ++p;

    if (*p != '=' || *p == '\0')
        goto error;

    // skip '='.
    ++p;

    while (isspace(*p)) ++p;

    // unquote.
    len = strlen(p);

    if (len > 1)
    {
        char first = p[0];
        if ((first == '\'' || first == '"')
            && p[len-1] == first)
        {
            _value.append(p + 1, len - 2);
            _type = CLineType::Key;
            _vtype = (first == '\"') ?
                     CValueType::DoubleQuoted : CValueType::SingleQuoted;

            return;
        }
    }

    _value = p;
    _type = CLineType::Key;

    return;

error:

    _type = CLineType::Error;
    _line = line;

}

void CIniLine::setValue(const char *value)
{
    if (_type == CLineType::Key)
        _value = value;
}


