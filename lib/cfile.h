#ifndef CFILE_H
#define CFILE_H

#include "cstring.h"

typedef struct _CFile CFile;

CFile* cfile_new();
void cfile_free(CFile *cfile);

// auto free ------------------------------------------------------------------

#define CFileAuto _CCLEANUP(_freeCFile) CFile
static inline void _freeCFile(CFile **cfile)
{
    cfile_free(*cfile);
}

bool cfile_open(CFile *cfile, const char *filepath, const char *mode);
void cfile_flush(CFile *cfile);
void cfile_close(CFile *cfile);

bool cfile_read(CFile *cfile, const char *filepath);
bool cfile_getline(CFile *cfile, CString *result);
void cfile_write(CFile *cfile, const char *str);

bool write_len(const char *filepath, const char *str, int len);

CString* cfile_buffer(CFile *cfile);
char* cfile_data(CFile *cfile);
int cfile_size(CFile *cfile);

#endif // CFILE_H


