#ifndef CFILEINFO_H
#define CFILEINFO_H

#include <inttypes.h>
#include <sys/stat.h>

class CFileInfo
{
public:

    CFileInfo(const char *filepath);
    CFileInfo();
    ~CFileInfo();

    bool read(const char *filepath);

    bool exists() {return _valid;}

    long size();
    uint64_t mtime();

private:

    struct stat _sb;
    bool _valid = false;

};

#endif // CFILEINFO_H


