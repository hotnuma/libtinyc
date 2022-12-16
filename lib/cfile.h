#ifndef CFILE_H
#define CFILE_H

#include "cstring.h"

typedef struct _CFile CFile;

CFile* cfile_new();
void cfile_free(CFile *cfile);

CString* cfile_buffer();
char* cfile_data();
int cfile_size();

bool cfile_open(CFile *cfile, const char *filepath, const char *mode);
void cfile_flush(CFile *cfile);
void cfile_close(CFile *cfile);

bool cfile_read(CFile *cfile, const char *filepath);
void cfile_write(CFile *cfile, const char *str);

bool write_len(const char *filepath, const char *str, int len);
//bool cfile_getLine(CFile *cfile, CString *result);

#endif // CFILE_H


