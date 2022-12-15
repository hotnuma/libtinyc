#include "CIniSection.h"
#include <ctype.h>

char* getSection(char *line, int length)
{
    if (length < 3)
        return nullptr;

    // search start of section.
    char *p = line;

    while (isspace(*p)) ++p;

    if (*p != '[')
        return nullptr;

    ++p;

    char *start = p;

    while (isalnum(*p) || *p == ' ') ++p;

    if (p == start || *p != ']')
        return nullptr;

    *p = '\0';

    return start;
}

void _deleteLine(CIniLine *line)
{
    if (line)
        delete line;
}

CIniSection::CIniSection(const char *name)
{
    _linesList.setDeleteFunc((CDeleteFunc) _deleteLine);

    if (name)
        _name = name;
}

void CIniSection::append(char *line)
{
    CIniLine *iniLine = new CIniLine(line);

    _linesList.append(iniLine);
}

//void CIniSection::setValue(const char *key, const char *value)
//{
//    CIniLine *iniLine = find(key);

//    if (!iniLine)
//        return;

//    iniLine->setValue(value);
//}

CString CIniSection::value(const char *key, const char *value)
{
    CString result(128);

    CIniLine* iniLine = find(key);
    if (!iniLine)
    {
        if (value)
            result = value;

        return result;
    }

    result = iniLine->value();

    return result;
}

CIniLine* CIniSection::find(const char *key)
{
    int size = _linesList.size();

    for (int i = 0; i < size; ++i)
    {
        CIniLine *iniLine = (CIniLine*) _linesList[i];
        const CString &line = iniLine->line();

        if (iniLine->type() == CLineType::Key
            && line == key)
        {
            return iniLine;
        }
    }

    return nullptr;
}


