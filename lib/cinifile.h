#ifndef CINIFILE_H
#define CINIFILE_H

#include "cstring.h"

typedef enum
{
    CLineTypeUndefined,
    CLineTypeComment,
    CLineTypeKey,
    CLineTypeEmpty,
    CLineTypeError,

} CLineType;

typedef enum
{
    CValueTypeUndefined,
    CValueTypeDoubleQuoted,
    CValueTypeSingleQuoted,

} CValueType;

typedef struct _CIniFile CIniFile;
typedef struct _CIniSection CIniSection;
typedef struct _CIniLine CIniLine;

// CIniFile ===================================================================

CIniFile* cinifile_new();
void cinifile_free(CIniFile *inifile);

// auto free ------------------------------------------------------------------

#define CIniFileAuto GC_CLEANUP(_freeCIniFile) CIniFile
GC_UNUSED static inline void _freeCIniFile(CIniFile **inifile)
{
    cinifile_free(*inifile);
}

bool cinifile_read(CIniFile *inifile, const char *filepath);
CIniSection* cinifile_section(CIniFile *inifile, const char *section);
int cinifile_size(CIniFile *inifile);
CIniSection* cinifile_section_at(CIniFile *inifile, int i);

// CIniSection ================================================================

#define cinisection_new() cinisection_new_name(NULL)
CIniSection* cinisection_new_name(const char *name);
void cinisection_free(CIniSection *section);

CIniLine* cinisection_find(CIniSection *section, const char *key);
CString* cinisection_name(CIniSection *section);
bool cinisection_value(CIniSection *section, CString *result,
                       const char *key, const char *defvalue);
bool cinisection_int(CIniSection *section, int *result,
                       const char *key, int defvalue);

// CIniLine ===================================================================

CIniLine* ciniline_new(char *line);
void ciniline_free(CIniLine *cline);

void ciniline_set_value(CIniLine *cline, const char *value);

CString* ciniline_line(CIniLine *cline);
CString* ciniline_value(CIniLine *cline);
CLineType ciniline_type(CIniLine *cline);

#endif // CINIFILE_H


