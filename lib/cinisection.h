#ifndef CINISECTION_H
#define CINISECTION_H

#include "ciniline.h"
#include "clist.h"

#if 0
char* getSection(char *line, int length);

class CIniSection
{
public:

    CIniSection(const char *name = nullptr);
    void append(char *line);

    //void setValue(const char *key, const char *value);
    CString value(const char *key, const char *value = nullptr);

    CIniLine* find(const char *key);

    CString name() const {return _name;}

private:

    CString _name;
    CList   _linesList;

};

#endif

#endif // CINISECTION_H


