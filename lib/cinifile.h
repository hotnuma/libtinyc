#ifndef CINIFILE_H
#define CINIFILE_H

#include "cinisection.h"

#if 0
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


