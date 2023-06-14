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

CFileInfo* cfileinfo_new()
{
    CFileInfo *info = (CFileInfo*) malloc(sizeof(CFileInfo));

    info->valid = false;

    return info;
}

void cfileinfo_free(CFileInfo *info)
{
    if (info == NULL)
        return;

    free(info);
}

bool cfileinfo_read(CFileInfo *info, const char *filepath)
{
    // https://man7.org/linux/man-pages/man2/lstat.2.html

    info->valid = (stat(filepath, &info->sb) == 0);

    return info->valid;
}

long cfileinfo_size(CFileInfo *info)
{
    if (!info->valid)
        return 0;

    return info->sb.st_size;
}

uint64_t cfileinfo_mtime(CFileInfo *info)
{
    if (!info->valid)
        return 0;

    struct timespec ts = info->sb.st_mtim;

    return (uint64_t) (ts.tv_sec * 1000) + (ts.tv_nsec / 1000000);
}

bool cfileinfo_exists(CFileInfo *info)
{
    return info->valid;
}


