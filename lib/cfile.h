#ifndef CFILE_H
#define CFILE_H

#include "cstring.h"
#include <stdio.h>

typedef struct _CFile CFile;

CFile* cfile_new();
void cfile_free(CFile *cfile);

// auto free -------------------------------------------------------------------

#define CFileAuto GC_CLEANUP(_freeCFile) CFile
GC_UNUSED static inline void _freeCFile(CFile **cfile)
{
    cfile_free(*cfile);
}

// CFile -----------------------------------------------------------------------

bool cfile_open(CFile *cfile, const char *filepath, const char *mode);
void cfile_flush(CFile *cfile);
void cfile_close(CFile *cfile);

bool cfile_read(CFile *cfile, const char *filepath);
bool cfile_getline(CFile *cfile, CString *result);

void cfile_write(CFile *cfile, const char *str);
void cfile_writefmt(CFile *cfile, const char *fmt, ...);

CString* cfile_buffer(CFile *cfile);
char* cfile_data(CFile *cfile);
int cfile_size(CFile *cfile);
FILE* cfile_fp(CFile *cfile);
int cfile_fd(CFile *cfile);

// File ------------------------------------------------------------------------

bool file_exists(const char *fileName);
bool file_remove(const char *fileName);
bool file_read(CString *cstr, const char *filepath);
bool file_getline(const char **start, CString *result);
bool file_write_len(const char *filepath, const char *str, int len);

#endif // CFILE_H


