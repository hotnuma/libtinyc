#ifndef CFILEINFO_H
#define CFILEINFO_H

#include "libmacros.h"

#include <stdbool.h>
#include <inttypes.h>
#include <sys/stat.h>

typedef struct _CFileInfo CFileInfo;

CFileInfo* cfileinfo_new();
void cfileinfo_free(CFileInfo *info);

// auto free ------------------------------------------------------------------

#define CFileInfoAuto GC_CLEANUP(_freeCFileInfo) CFileInfo
GC_UNUSED static inline void _freeCFileInfo(CFileInfo **info)
{
    cfileinfo_free(*info);
}

bool cfileinfo_read(CFileInfo *info, const char *filepath);
long cfileinfo_size(CFileInfo *info);
uint64_t cfileinfo_mtime(CFileInfo *info);

bool cfileinfo_exists(CFileInfo *info);

#endif // CFILEINFO_H


