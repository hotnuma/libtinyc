#include "cinifile.h"

#include <ctype.h>
#include "clist.h"
#include "cfile.h"
#include "libstr.h"

// CIniFile -------------------------------------------------------------------

struct _CIniFile
{
    CList *sectionList;
};

struct _CIniSection
{
    CString *name;
    CList   *linesList;
    CString *value;
};

struct _CIniLine
{
    CString *line;
    CString *value;

    CLineType type;
    CValueType vtype;
};


CIniFile* cinifile_new()
{
    CIniFile *inifile = (CIniFile*) malloc(sizeof(CIniFile));

    inifile->sectionList = clist_new(32, (CDeleteFunc) cinisection_free);

    return inifile;
}

void cinifile_free(CIniFile *inifile)
{
    if (inifile == NULL)
        return;

    clist_free(inifile->sectionList);
    free(inifile);
}

static char* _cinifile_get_section(char *line, int length)
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

bool cinifile_read(CIniFile *inifile, const char *filepath)
{
    CFile *file = cfile_new();
    if (!cfile_read(file, filepath))
    {
        cfile_free(file);
        return false;
    }

    char *ptr = cfile_data(file);
    char *result = NULL;
    int length = 0;

    CIniSection *section = NULL;

    while (str_getlineptr(&ptr, &result, &length))
    {
        result[length] = '\0';

        char *secfound = _cinifile_get_section(result, length);

        if (secfound)
        {
            section = cinisection_new_name(secfound);
            clist_append(inifile->sectionList, section);
            continue;
        }

        if (section == NULL)
        {
            section = cinisection_new();
            clist_append(inifile->sectionList, section);
            continue;
        }

        // append line in current section.
        //_cinisection_append(section, result);

        clist_append(section->linesList, ciniline_new(result));
    }

    cfile_free(file);

    return true;
}

CIniSection* cinifile_section(CIniFile *inifile, const char *section)
{
    int size = clist_size(inifile->sectionList);

    for (int i = 0; i < size; ++i)
    {
        CIniSection *iniSection = (CIniSection*) clist_at(inifile->sectionList, i);

        if (cstr_compare(cinisection_name(iniSection), section, true) == 0)
            return iniSection;
    }

    return NULL;
}

int cinifile_size(CIniFile *inifile)
{
    return clist_size(inifile->sectionList);
}

CIniSection* cinifile_section_at(CIniFile *inifile, int i)
{
    return (CIniSection*) clist_at(inifile->sectionList, i);
}

// CIniSection ----------------------------------------------------------------

CIniSection* cinisection_new_name(const char *name)
{
    CIniSection *section = (CIniSection*) malloc(sizeof(CIniSection));

    if (name)
        section->name = cstr_new(name);
    else
        section->name = cstr_new_size(32);

    section->linesList = clist_new(32, (CDeleteFunc) ciniline_free);

    section->value = cstr_new_size(32);

    return section;
}

void cinisection_free(CIniSection *section)
{
    if (section == NULL)
        return;

    cstr_free(section->name);
    clist_free(section->linesList);
    cstr_free(section->value);

    free(section);
}

CIniLine* cinisection_find(CIniSection *section, const char *key)
{
    if (!section)
        return NULL;

    CList *lines = section->linesList;

    int size = clist_size(lines);

    for (int i = 0; i < size; ++i)
    {
        CIniLine *iniLine = (CIniLine*) clist_at(lines, i);

        CString *line = ciniline_line(iniLine);

        if (ciniline_type(iniLine) == CLineTypeKey
            && cstr_compare(line, key, true) == 0)
        {
            return iniLine;
        }
    }

    return NULL;
}

CString* cinisection_name(CIniSection *section)
{
    return section->name;
}

bool cinisection_value(CIniSection *section, CString *result,
                       const char *key, const char *defvalue)
{
    cstr_clear(result);

    CIniLine *iniLine = cinisection_find(section, key);

    if (!iniLine)
    {
        if (defvalue)
            cstr_copy(result, defvalue);

        return false;
    }

    cstr_copy(result, c_str(ciniline_value(iniLine)));

    return true;
}

bool cinisection_int(CIniSection *section, int *result,
                       const char *key, int defvalue)
{
    if (!section || !cinisection_value(section, section->value, key, ""))
    {
        *result = defvalue;
        return false;
    }

    *result = atoi(c_str(section->value));

    return true;
}

// CIniLine -------------------------------------------------------------------

static void _ciniline_set_line(CIniLine *cline, char *line);

CIniLine* ciniline_new(char *line)
{
    CIniLine *cline = (CIniLine*) malloc(sizeof(CIniLine));

    cline->line = cstr_new_size(32);
    cline->value = cstr_new_size(16);

    _ciniline_set_line(cline, line);

    return cline;
}

void ciniline_free(CIniLine *cline)
{
    if (cline == NULL)
        return;

    cstr_free(cline->line);
    cstr_free(cline->value);

    free(cline);
}

static void _ciniline_set_line(CIniLine *cline, char *line)
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

void ciniline_set_value(CIniLine *cline, const char *value)
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


