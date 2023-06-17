#include "cfileinfo.h"

#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// https://man7.org/linux/man-pages/man2/lstat.2.html
// https://man7.org/linux/man-pages/man2/statx.2.html
// https://man7.org/linux/man-pages/man7/inode.7.html

struct _CFileInfo
{
    struct stat sb;
    bool valid;
};

CFileInfo* cfileinfo_new()
{
    CFileInfo *info = (CFileInfo*) calloc(1, sizeof(CFileInfo));

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
    info->valid = (stat(filepath, &info->sb) == 0);

    return info->valid;
}

long cfileinfo_size(CFileInfo *info)
{
    if (!info->valid)
        return 0;

    return info->sb.st_size;
}

uint64_t cfileinfo_atime(CFileInfo *info)
{
    if (!info->valid)
        return 0;

    return info->sb.st_atime;
}

uint64_t cfileinfo_mtime(CFileInfo *info)
{
    if (!info->valid)
        return 0;

    return info->sb.st_mtime;
}

bool cfileinfo_exists(CFileInfo *info)
{
    return info->valid;
}


