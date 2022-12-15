#include "libpath.h"

#include "libstr.h"
#include <string.h>

const char* pathSep(const char *path)
{
    if (!path)
        return nullptr;

    const char *sep = nullptr;

    while (*path)
    {
        if (*path == '/')
            sep = path;

        ++path;
    }

    return sep;
}

const char* pathExt(const char *path, bool first)
{
    const char *sep = pathSep(path);

    if (sep)
    {
        path = ++sep;

        if (*path == '\0')
            return nullptr;
    }

    // hidden file.
    if (*path == '.')
        ++path;

    const char *found = nullptr;

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

bool pathStripExt(CString &path)
{
    const char *p = path.c_str();
    const char *dot = pathExt(p);

    if (!dot)
        return false;

    path.terminate(dot - path);
    return true;
}

CString pathDirName(const char *path)
{
    CString result(128);

    if (!path || !*path)
        return result;

    const char *p = path;
    const char *sep = nullptr;

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
                result.append(path, len);
            }

            return result;
        }

        ++p;
    }
}

CString pathBaseName(const char *path)
{
    CString result(64);

    if (!path || !*path)
        return result;

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
            result.append(path, p - path);

            return result;
        }

        ++p;
    }
}

CString pathJoin(const char *dir, const char *file)
{
    CString result(128);

    if (!dir || !file)
        return result;

    int dirlen = strlen(dir);

    if (!dirlen)
    {
        result.append(file);
        return result;
    }

    // strip last seps
    while (dirlen > 0 && dir[dirlen - 1] == '/')
        --dirlen;

    // strip first seps
    while (*file == '/')
        ++file;

    int filelen = strlen(file);

    result.resize(dirlen + filelen + 2);

    result.append(dir, dirlen);

    if (filelen > 0)
    {
        result.append('/');
        result.append(file);
    }

    return result;
}

int _count(const char *str)
{
    int ret = 0;

    while (*str)
    {
        if (*str == '/')
            ++ret;

        ++str;
    }

    return ret;
}

int pathCmp(const char *s1, const char *s2)
{
    int n1 = _count(s1);
    int n2 = _count(s2);

    if (n1 != n2)
        return n2 - n1;

    return strcmp(s1, s2);
}

bool pathCanonicalize(char *path, int *len)
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


