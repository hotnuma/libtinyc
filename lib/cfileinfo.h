#ifndef CFILEINFO_H
#define CFILEINFO_H

#include <stdbool.h>
#include <inttypes.h>
#include <sys/stat.h>

typedef struct _CFileInfo CFileInfo;

#define cfileinfo_new() cfileinfo_new_path(NULL)
CFileInfo* cfileinfo_new_path(const char *filepath);
void cfileinfo_free(CFileInfo *cinfo);

bool cfileinfo_read(CFileInfo *cinfo, const char *filepath);
long cfileinfo_size(CFileInfo *cinfo);
uint64_t cfileinfo_mtime(CFileInfo *cinfo);

bool cfileinfo_exists(CFileInfo *cinfo);

#endif // CFILEINFO_H


