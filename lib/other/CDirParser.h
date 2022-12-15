#ifndef CDIRPARSER_H
#define CDIRPARSER_H

#include "CString.h"
#include "CList.h"

#define CDP_NODE        0
#define CDP_RELATIVE    (1 << 0)
#define CDP_SUBDIRS     (1 << 1)
#define CDP_DIRS        (1 << 2)
#define CDP_FILES       (1 << 3)

class CDirParser
{
public:

    CDirParser();
    CDirParser(const char *directory,
               int flags = CDP_DIRS | CDP_FILES);
    void close();

    bool open(const char *directory,
              int flags = CDP_DIRS | CDP_FILES);
    bool read(CString &filepath, int *type = nullptr);

private:

    CList _list;

    int _flags = CDP_NODE;
    int _dirlen = 0;

};

#endif // CDIRPARSER_H


