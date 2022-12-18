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

typedef struct _CIniLine CIniLine;

CIniLine* ciniline_new(char *line);
void ciniline_free(CIniLine *cline);

void ciniline_setValue(CIniLine *cline, const char *value);

CString* ciniline_line(CIniLine *cline);
CString* ciniline_value(CIniLine *cline);
CLineType ciniline_type(CIniLine *cline);

#if 0

char* getSection(char *line, int length);

class CIniSection
{
public:

    CIniSection(const char *name = NULL);
    void append(char *line);

    //void setValue(const char *key, const char *value);
    CString value(const char *key, const char *value = NULL);

    CIniLine* find(const char *key);

    CString name() const {return _name;}

private:

    CString _name;
    CList   _linesList;

};



class CIniFile
{
public:

    CIniFile();

    bool open(const char *filepath);

    int size() {return _sectionList.size();}
    CIniSection* section(const char *section);
    CIniSection* sectionAt(int i) {return (CIniSection*) _sectionList.at(i);}


private:

    CString _filepath;
    CList _sectionList;

};

#endif

#if 0
void clear();
bool save();
bool saveAs(const char *filepath);
int _addSectionTxt(CStringList &allLines, int fromline, int toline);
CStringList allSections();
#endif

#endif // CINIFILE_H


