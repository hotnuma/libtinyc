#ifndef CINILINE_H
#define CINILINE_H

#include "CString.h"

enum class CLineType
{
    Undefined,
    Comment,
    Key,
    Empty,
    Error
};

enum class CValueType
{
    Undefined,
    DoubleQuoted,
    SingleQuoted,
};

class CIniLine
{
public:

    CIniLine(char *line);

    const CString& line() const {return _line;}

    const CString& value() const {return _value;}
    void setValue(const char *value);

    CLineType type() const {return _type;}

private:

    void _setLine(char *line);

    CString _line;
    CString _value;

    CLineType _type = CLineType::Undefined;
    CValueType _vtype = CValueType::Undefined;

};

#endif // CINILINE_H


