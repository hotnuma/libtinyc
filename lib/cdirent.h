#ifndef CDIRENT_H
#define CDIRENT_H

#include "cstring.h"
#include <dirent.h>

typedef struct _CDirent CDirent;

#define cdirent_new() cdirent_new_path(NULL)
CDirent* cdirent_new_path(const char *basedir);
void cdirent_free(CDirent *cdir);

// auto free ------------------------------------------------------------------

#define CDirentAuto GC_CLEANUP(_freeCDirent) CDirent
GC_UNUSED static inline void _freeCDirent(CDirent **dirent)
{
    cdirent_free(*dirent);
}

CString* cdirent_directory(CDirent *cdir);

bool cdirent_open(CDirent *cdir, const char *basedir);
void cdirent_close(CDirent *cdir);

bool cdirent_read(CDirent *cdir, CString *result, int *type);

#endif // CDIRENT_H


