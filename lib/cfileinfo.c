#include "cfileinfo.h"

#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

struct _CFileInfo
{
    struct stat sb;
    bool valid;
};

CFileInfo* cfileinfo_new_path(const char *filepath)
{
    CFileInfo *cinfo = (CFileInfo*) malloc(sizeof(CFileInfo));
    cinfo->valid = false;

    if (filepath)
        cfileinfo_read(cinfo, filepath);

    return cinfo;
}

void cfileinfo_free(CFileInfo *cinfo)
{
    if (cinfo)
        free(cinfo);
}

bool cfileinfo_read(CFileInfo *cinfo, const char *filepath)
{
    cinfo->valid = false;

    if (stat(filepath, &cinfo->sb) == -1)
        return cinfo->valid;

    cinfo->valid = true;

    return cinfo->valid;
}

long cfileinfo_size(CFileInfo *cinfo)
{
    if (!cinfo->valid)
        return 0;

    return cinfo->sb.st_size;
}

uint64_t cfileinfo_mtime(CFileInfo *cinfo)
{
    if (!cinfo->valid)
        return 0;

    struct timespec ts = cinfo->sb.st_mtim;

    return (uint64_t) (ts.tv_sec * 1000) + (ts.tv_nsec / 1000000);
}


