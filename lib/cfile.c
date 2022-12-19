#include "cfile.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct _CFile
{
    CString *buffer;
    FILE *fp;
    const char *curr;
};

CFile* cfile_new()
{
    CFile *cfile = (CFile*) malloc(sizeof(CFile));

    cfile->buffer = cstr_new_size(128);
    cfile->fp = NULL;
    cfile->curr = NULL;

    return cfile;
}

bool cfile_open(CFile *cfile, const char *filepath, const char *mode)
{
    cfile_close(cfile);

    cfile->fp = fopen(filepath, mode);

    return (cfile->fp != NULL);
}

void cfile_flush(CFile *cfile)
{
    if (cfile->fp)
        fflush(cfile->fp);
}

void cfile_close(CFile *cfile)
{
    if (cfile->fp)
        fclose(cfile->fp);
    cfile->fp = NULL;
}

void cfile_free(CFile *cfile)
{
    cfile_close(cfile);
    cstr_free(cfile->buffer);
    free(cfile);
}

bool cfile_read(CFile *cfile, const char *filepath)
{
    if (!cfile_open(cfile, filepath, "rb"))
        return false;

    fseek(cfile->fp, 0, SEEK_END);
    unsigned long size = ftell(cfile->fp);
    fseek(cfile->fp, 0, SEEK_SET);

    cstr_clear(cfile->buffer);
    cstr_resize(cfile->buffer, size + 1);

    if (fread(c_str(cfile->buffer), 1, size, cfile->fp) != size)
    {
        cfile_close(cfile);
        cstr_clear(cfile->buffer);

        cfile->curr = NULL;

        return false;
    }

    cfile_close(cfile);

    cstr_terminate(cfile->buffer, size);
    cfile->curr = c_str(cfile->buffer);

    return true;
}

void cfile_write(CFile *cfile, const char *str)
{
    if (!cfile->fp || !str)
        return;

    fwrite(str, 1, strlen(str), cfile->fp);
}

bool cfile_getline(CFile *cfile, CString *result)
{
    if (!cfile->curr)
        return false;

    return str_getline(&cfile->curr, result);
}

CString* cfile_buffer(CFile *cfile)
{
    return cfile->buffer;
}

char* cfile_data(CFile *cfile)
{
    return cstr_data(cfile->buffer);
}

int cfile_size(CFile *cfile)
{
    return cstr_size(cfile->buffer);
}

// static write.
bool write_len(const char *filepath, const char *str, int len)
{
    FILE *fp = fopen(filepath, "wb");

    if (!fp)
        return false;

    if (len < 1)
        len = strlen(str);

    size_t ret = fwrite(str, 1, len, fp);

    if (ret != (size_t) len)
    {
        fclose(fp);
        return false;
    }

    fflush(fp);
    fclose(fp);

    return true;
}


