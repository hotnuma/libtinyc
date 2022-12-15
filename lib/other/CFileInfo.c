#include "CFileInfo.h"

#include <stdlib.h>
#include <unistd.h>
#include <string.h>

CFileInfo::CFileInfo()
{
}

CFileInfo::CFileInfo(const char *filepath)
{
    read(filepath);
}

CFileInfo::~CFileInfo()
{
}

bool CFileInfo::read(const char *filepath)
{
    _valid = false;

    if (stat(filepath, &_sb) == -1)
        return _valid;

    _valid = true;

    return _valid;
}

long CFileInfo::size()
{
    if (!_valid)
        return 0;

    return _sb.st_size;
}

uint64_t CFileInfo::mtime()
{
    if (!_valid)
        return 0;

    struct timespec ts = _sb.st_mtim;

    return (uint64_t) (ts.tv_sec * 1000) + (ts.tv_nsec / 1000000);
}


