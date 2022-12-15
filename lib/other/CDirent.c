#include "CDirent.h"

#include "libpath.h"
#include <string.h>

CDirent::CDirent()
{
}

CDirent::CDirent(const char *basedir)
{
    open(basedir);
}

CDirent::~CDirent()
{
    close();
}

bool CDirent::open(const char *basedir)
{
    if (_dir)
        close();

    _basedir = basedir;

    if (_basedir.last() == '/')
        _basedir.chop(1);

    _dir = opendir(basedir);

    return (_dir != nullptr);
}

void CDirent::close()
{
    if (_dir)
    {
        closedir((DIR*) _dir);
        _dir = nullptr;
    }
}

bool CDirent::read(CString &result, int *type)
{
    if (!_dir)
        return false;

    readnext:
    struct dirent *nextp = readdir((DIR*) _dir);

    if (!nextp)
        return false;

    if (_skipdot
        && (strcmp(nextp->d_name, ".") == 0
            || strcmp(nextp->d_name, "..") == 0))
        goto readnext;

    result = nextp->d_name;

    if (type)
        *type = (int) nextp->d_type;

    return true;
}


