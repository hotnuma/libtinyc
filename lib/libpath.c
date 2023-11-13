#include "libpath.h"

#include "libstr.h"
#include <string.h>

bool path_dirname(CString *result, const char *path)
{
    cstr_clear(result);

    if (!result || !path || !*path)
        return false;

    const char *p = path;
    const char *sep = NULL;

    while (1)
    {
        if (*p == '/')
        {
            sep = p;
        }
        else if (*p == '\0')
        {
            if (sep)
            {
                int len = sep - path;
                cstr_append_len(result, path, len);
            }

            return true;
        }

        ++p;
    }
}

bool path_basename(CString *result, const char *path)
{
    cstr_clear(result);

    if (!result || !path || !*path)
        return false;

    const char *p = path;

    while (1)
    {
        if (*p == '/')
        {
            path = ++p;
            continue;
        }
        else if (*p == '\0')
        {
            cstr_append_len(result, path, p - path);

            return true;
        }

        ++p;
    }
}

const char* path_basename_ptr(const char *path)
{
    const char *sep = path_sep(path);
    if (!sep)
        return NULL;

    return sep + 1;
}

const char* path_sep(const char *path)
{
    if (!path)
        return NULL;

    const char *sep = NULL;

    while (*path)
    {
        if (*path == '/')
            sep = path;

        ++path;
    }

    return sep;
}

const char* path_ext(const char *path, bool first)
{
    const char *sep = path_sep(path);

    if (sep)
    {
        path = ++sep;

        if (*path == '\0')
            return NULL;
    }

    // hidden file.
    if (*path == '.')
        ++path;

    const char *found = NULL;

    while (*path)
    {
        if (*path == '.' && path[1] != '\0')
        {
            found = path;

            if (first)
                return found;
        }

        ++path;
    }

    return found;
}

bool path_strip_ext(CString *path, bool first)
{
    const char *start = c_str(path);
    const char *p = start;
    const char *dot = path_ext(p, first);

    if (!dot)
        return false;

    cstr_terminate(path, dot - start);

    return true;
}

void path_join(CString *result, const char *dir, const char *file)
{
    cstr_clear(result);

    if (!dir || !file)
        return; // result;

    int dirlen = strlen(dir);

    if (!dirlen)
    {
        cstr_append(result, file);

        return; // result;
    }

    // strip last seps
    while (dirlen > 0 && dir[dirlen - 1] == '/')
        --dirlen;

    // strip first seps
    while (*file == '/')
        ++file;

    int filelen = strlen(file);

    cstr_resize(result, dirlen + filelen + 2);

    cstr_append_len(result, dir, dirlen);

    if (filelen > 0)
    {
        cstr_append_c(result, '/');
        cstr_append_len(result, file, filelen);
    }

    return; // result;
}

bool path_canonicalize(char *path, int *len)
{
    if (*len == 0)
        return false;

    const int maxParts = 60;
    char *parts[maxParts];
    int count = 0;

    char *start = path;
    char *dst = start;
    const char *src = start;
    const char *end = start + *len;

    if (*src == '/')
    {
        ++src;
        ++dst;
    }

    while (src < end)
    {
        if (*src == '.')
        {
            if (src + 1 == end || src[1] == '/')
            {
                // '.' component; eliminate.
                src += 2;
                continue;
            }
            else if (src[1] == '.' && (src + 2 == end || src[2] == '/'))
            {
                // '..' component.  Back up if possible.
                if (count > 0)
                {
                    dst = parts[count - 1];
                    src += 3;
                    --count;
                }
                else
                {
                    *dst++ = *src++;
                    *dst++ = *src++;
                    *dst++ = *src++;
                }
                continue;
            }
        }

        if (*src == '/')
        {
            src++;
            continue;
        }

        if (count == maxParts)
            return false;

        parts[count] = dst;
        ++count;

        while (src != end && *src != '/')
            *dst++ = *src++;

        *dst++ = *src++;  // Copy '/' or final \0 character as well.
    }

    if (dst == start)
    {
        *dst++ = '.';
        *dst++ = '\0';
    }

    *len = dst - start - 1;

    return true;
}

void path_basename_sp(CString *cstr, const char *path)
{
    if (!path)
        return;

    //CString *result = cstr_new(path);

    cstr_clear(cstr);

    const char *p = path;

    while (1)
    {
        if (*p == '/')
        {
            path = ++p;
            continue;
        }
        else if (*p == ' ' || *p == '\0')
        {
            int length = p - path;

            cstr_append_len(cstr, path, length);

            return;
        }

        ++p;
    }
}


