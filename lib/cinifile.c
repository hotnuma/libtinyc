#include "cinifile.h"

#include <ctype.h>
//#include <string.h>
//#include <assert.h>
//#include "cfile.h"
//#include "libstr.h"
//#include "print.h"

//void _deleteSection(CIniSection *section)
//{
//    if (section)
//        delete section;
//}

static void _ciniline_setLine(CIniLine *cline, char *line);

struct _CIniLine
{
    CString *line;
    CString *value;

    CLineType type;
    CValueType vtype;
};

CIniLine* ciniline_new(char *line)
{
    CIniLine *cline = (CIniLine*) malloc(sizeof(CIniLine));

    cline->line = cstr_new_size(32);
    cline->value = cstr_new_size(16);

    _ciniline_setLine(cline, line);

    return cline;
}

void ciniline_free(CIniLine *cline)
{
    if (!cline)
        return;

    cstr_free(cline->line);
    cstr_free(cline->value);

    free(cline);
}

static void _ciniline_setLine(CIniLine *cline, char *line)
{
    cline->type = CLineTypeUndefined;
    cline->vtype = CValueTypeUndefined;

    cstr_clear(cline->line);
    cstr_clear(cline->value);

    char *p = line;

    // skip blanks.
    while (isspace(*p)) ++p;

    // comments.
    if (*p == ';' || *p == '#')
    {
        cline->type = CLineTypeComment;
        cstr_append(cline->line, p);

        return;
    }

    // empty.
    else if (*p == '\0')
    {
        cline->type = CLineTypeEmpty;

        //cline->line.clear();
        return;
    }

    // search a key.
    char *key = p;

    while (isalnum(*p) || *p == '_' || *p == '.') ++p;

    // no alphanum or nothing after key.
    if (p == key || *p == '\0')
        goto error;

    // set key.
    cstr_append_len(cline->line, key, p - key);

    // search '=' char.

    while (isspace(*p)) ++p;

    if (*p != '=' || *p == '\0')
        goto error;

    // skip '='.
    ++p;

    while (isspace(*p)) ++p;

    // unquote.
    int len = strlen(p);

    if (len > 1)
    {
        char first = p[0];

        if ((first == '\'' || first == '"')
            && p[len-1] == first)
        {
            cstr_append_len(cline->value, p + 1, len - 2);

            cline->type = CLineTypeKey;
            cline->vtype = (first == '\"') ? CValueTypeDoubleQuoted
                                           : CValueTypeSingleQuoted;

            return;
        }
    }

    cstr_append(cline->value, p);
    cline->type = CLineTypeKey;

    return;

error:;

    cline->type = CLineTypeError;
    cstr_append(cline->line, line);
}

void ciniline_setValue(CIniLine *cline, const char *value)
{
    if (cline->type == CLineTypeKey)
        cstr_copy(cline->value, value);
}

CString* ciniline_line(CIniLine *cline)
{
    return cline->line;
}

CString* ciniline_value(CIniLine *cline)
{
    return cline->value;
}

CLineType ciniline_type(CIniLine *cline)
{
    return cline->type;
}


#if 0
char* getSection(char *line, int length)
{
    if (length < 3)
        return NULL;

    // search start of section.
    char *p = line;

    while (isspace(*p)) ++p;

    if (*p != '[')
        return NULL;

    ++p;

    char *start = p;

    while (isalnum(*p) || *p == ' ') ++p;

    if (p == start || *p != ']')
        return NULL;

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

    return NULL;
}




DELETEFUNC(CIniSection)

CIniFile::CIniFile()
{
    SETDELETEFUNC(&_sectionList, CIniSection);

    //_sectionList.setDeleteFunc((CDeleteFunc) _deleteSection);
}

bool CIniFile::open(const char *filepath)
{
    CFile file;
    if (!file.read(filepath))
        return false;

    _filepath = filepath;

    char *ptr = file.data();
    char *result = NULL;
    int length = 0;

    CIniSection *section = NULL;
    int count = 0;

    while (strGetLinePtr(&ptr, &result, &length))
    {
        // skip empty lines.
        //if (length == 0)
        //    continue;

        ++count;

        result[length] = '\0';

        char *sec = getSection(result, length);
        if (count == 1 && !sec)
        {
            // default section.
            section = new CIniSection();
            _sectionList.append(section);
            continue;
        }
        else if (sec)
        {
            section = new CIniSection(sec);
            _sectionList.append(section);
            continue;
        }

        assert(section);

        // append line in current section.
        section->append(result);
    }

    return true;
}

CIniSection* CIniFile::section(const char *section)
{
    int size = _sectionList.size();
    for (int i = 0; i < size; ++i)
    {
        CIniSection *iniSection = (CIniSection*) _sectionList[i];
        if (iniSection->name() == section)
            return iniSection;
    }

    return NULL;
}
#endif


#if 0

bool CIniFile::save()
{
    // Output file.
    CFile outFile;
    if (!outFile.open(_filepath, "wb"))
        return false;

    int size = _sectionList.size();
    for (int i = 0; i < size; ++i)
    {
        CIniSection *section = (CIniSection*) _sectionList[i];
        section->writeSectionTxt(outFile);
    }

    return true;
}

bool CIniFile::saveAs(const char *filepath)
{
    _filepath = filepath;

    return save();
}

CStringList CIniFile::allSections()
{
    CStringList result;

    int size = _sectionList.size();
    for (int i = 0; i < size; ++i)
    {
        CIniSection *section = (CIniSection*) _sectionList[i];
        result.append(section->name());
    }

    return result;
}

#endif


