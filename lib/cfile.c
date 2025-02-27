#include "cfile.h"

#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>


// file -----------------------------------------------------------------------

int file_type(const char *filepath)
{
    if (filepath)
    {
        struct stat st;
        return (stat(filepath, &st) == 0 ? st.st_mode : 0);
    }
    return 0;
}

bool file_exists(const char *filepath)
{
    if (filepath)
    {
        struct stat st;
        return (stat(filepath, &st) == 0);
    }
    return false;
}

bool file_isdir(const char *filepath)
{
    if (filepath)
    {
        struct stat st;
        return (stat(filepath, &st) == 0 && (st.st_mode & S_IFDIR));
    }
    return false;
}

bool file_remove(const char *filepath)
{
    return (remove(filepath) == 0);
}

bool file_read(CString *cstr, const char *filepath)
{
    if (cstr_capacity(cstr) < 1)
        return false;

    int fd = open(filepath, O_RDONLY);
    if (fd < 0)
        return false;

    cstr_clear(cstr);

    while (1)
    {
        int capacity = cstr_capacity(cstr);
        int length = cstr_size(cstr);
        int remain = capacity - length;

        while (remain < 2)
        {
            capacity *= 2;
            remain = capacity - length;
        }

        cstr_resize(cstr, capacity);

        char *buff = cstr_data(cstr) + cstr_size(cstr);

        int nb_read = read(fd, buff, remain - 1);

        if (nb_read < 1)
            break;

        cstr_terminate(cstr, cstr_size(cstr) + nb_read);
    }

    close(fd);

    return true;
}

bool file_getline(const char **start, CString *result)
{
    // start of line.
    const char *first = *start;

    // end of buffer ?
    if (*first == '\0')
        return false;

    // search end of line.
    const char *p = first;

    while (1)
    {
        if (*p == '\r')
        {
            cstr_copy_len(result, first, p - first);

            // skip.
            if (*(p + 1) == '\n')
                ++p;

            // move to next line.
            *start = ++p;

            return true;
        }
        else if (*p == '\n')
        {
            cstr_copy_len(result, first, p - first);

            // move to next line.
            *start = ++p;

            return true;
        }
        else if (*p == '\0')
        {
            cstr_copy_len(result, first, p - first);

            // move to the end.
            *start = p;

            return true;
        }

        ++p;
    }
}

// static write.
bool file_write_len(const char *filepath, const char *str, int len)
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


// CFile ----------------------------------------------------------------------

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
    if (cfile == NULL)
        return;

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

void cfile_writefmt(CFile *cfile, const char *fmt, ...)
{
    if (!cfile->fp || !fmt)
        return;

    va_list va;
    va_start(va, fmt);
    vfprintf(cfile->fp, fmt, va);
    va_end(va);
}

bool cfile_getline(CFile *cfile, CString *result)
{
    if (!cfile->curr)
        return false;

    return file_getline(&cfile->curr, result);
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

FILE* cfile_fp(CFile *cfile)
{
    return cfile->fp;
}

int cfile_fd(CFile *cfile)
{
    return fileno(cfile->fp);
}


