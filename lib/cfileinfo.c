#include "cfileinfo.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

// https://man7.org/linux/man-pages/man2/lstat.2.html
// https://man7.org/linux/man-pages/man2/statx.2.html
// https://man7.org/linux/man-pages/man7/inode.7.html

struct _CFileInfo
{
    struct statx stx;
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
    int atflag = AT_SYMLINK_NOFOLLOW;
    unsigned int mask = STATX_BASIC_STATS | STATX_BTIME;

    memset(&info->stx, 0, sizeof(struct statx));
    info->valid = (statx(AT_FDCWD, filepath, atflag, mask, &info->stx) == 0);

    return info->valid;
}

uint64_t cfileinfo_size(CFileInfo *info)
{
    if (!info->valid)
        return 0;

    return info->stx.stx_size;
}

uint64_t cfileinfo_btime(CFileInfo *info)
{
    if (!info->valid)
        return 0;

    return info->stx.stx_btime.tv_sec;
}

uint64_t cfileinfo_mtime(CFileInfo *info)
{
    if (!info->valid)
        return 0;

    return info->stx.stx_mtime.tv_sec;
}

uint64_t cfileinfo_atime(CFileInfo *info)
{
    if (!info->valid)
        return 0;

    return info->stx.stx_atime.tv_sec;
}

bool cfileinfo_exists(CFileInfo *info)
{
    return info->valid;
}


