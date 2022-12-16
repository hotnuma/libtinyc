#include "cdirent.h"

#include <stdlib.h>
#include <string.h>

struct _CDirent
{
    CString *basedir;
    void    *dir;
    bool    skipdot;
};

CDirent* cdirent_new_path(const char *basedir)
{
    CDirent *cdir = (CDirent*) malloc(sizeof(CDirent));

    if (basedir)
        cdir->basedir = cstr_new_size(strlen(basedir + 1));
    else
        cdir->basedir = cstr_new_size(32);

    cdir->dir = NULL;
    cdir->skipdot = true;

    if (basedir)
        cdirent_open(cdir, basedir);

    return cdir;
}

void cdirent_free(CDirent *cdir)
{
    cdirent_close(cdir);

    cstr_free(cdir->basedir);

    free(cdir);
}

CString* cdirent_directory(CDirent *cdir)
{
    return cdir->basedir;
}

bool cdirent_open(CDirent *cdir, const char *basedir)
{
    cdirent_close(cdir);

    cstr_append(cdir->basedir, basedir);

    if (cstr_last(cdir->basedir) == '/')
        cstr_chop(cdir->basedir, 1);

    cdir->dir = opendir(basedir);

    return (cdir->dir != NULL);
}

void cdirent_close(CDirent *cdir)
{
    if (cdir->basedir)
        cstr_clear(cdir->basedir);

    if (cdir->dir)
    {
        closedir((DIR*) cdir->dir);
        cdir->dir = NULL;
    }
}

bool cdirent_read(CDirent *cdir, CString *result, int *type)
{
    cstr_clear(result);

    if (!cdir->dir)
        return false;

    readnext:;

    struct dirent *nextp = readdir((DIR*) cdir->dir);

    if (!nextp)
    {
        cdirent_close(cdir);
        return false;
    }

    if (cdir->skipdot && (strcmp(nextp->d_name, ".") == 0
                          || strcmp(nextp->d_name, "..") == 0))
    {
        goto readnext;
    }

    cstr_append(result, nextp->d_name);

    if (type)
        *type = (int) nextp->d_type;

    return true;
}


