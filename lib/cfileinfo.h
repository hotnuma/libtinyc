#ifndef CFILEINFO_H
#define CFILEINFO_H

#include "libmacros.h"

#include <stdbool.h>
#include <inttypes.h>

typedef struct _CFileInfo CFileInfo;

// allocate -------------------------------------------------------------------

CFileInfo* cfileinfo_new();
void cfileinfo_free(CFileInfo *info);

#define CFileInfoAuto GC_CLEANUP(_freeCFileInfo) CFileInfo
GC_UNUSED static inline void _freeCFileInfo(CFileInfo **info)
{
    cfileinfo_free(*info);
}

// read -----------------------------------------------------------------------

bool cfileinfo_read(CFileInfo *info, const char *filepath);
long cfileinfo_size(CFileInfo *info);
uint64_t cfileinfo_btime(CFileInfo *info);
uint64_t cfileinfo_mtime(CFileInfo *info);
uint64_t cfileinfo_atime(CFileInfo *info);

bool cfileinfo_exists(CFileInfo *info);

#endif // CFILEINFO_H


