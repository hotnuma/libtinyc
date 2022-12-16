#if 0

CStringList cstr_split(CString *cstr, const char *sep, bool keepEmptyParts, bool sensitive)
{
    CStringList list;

    int slen = strlen(sep);

    const char *start = cstr->buffer;
    const char *c = start;
    int len = 0;

    while (1)
    {
        if (*c == '\0')
        {
            len = c - start;
            if (len || keepEmptyParts)
                list.append(start, len);

            break;
        }

        if (sensitive)
        {
            if (strncmp(c, sep, slen) == 0)
            {
                len = c - start;
                if (len > 0 || keepEmptyParts)
                    list.append(start, len);
                c += slen;
                start = c;

                continue;
            }
        }
        else
        {
            if (strncasecmp(c, sep, slen) == 0)
            {
                len = c - start;
                if (len > 0 || keepEmptyParts)
                    list.append(start, len);
                c += slen;
                start = c;

                continue;
            }
        }

        ++c;
    }

    return list;
}

void _utf8inc(const char **str, int *count)
{
    if ((**str & 0xC0) != 0x80)
        ++*count;
    ++*str;
}

CString* utf8wrap(const char *str, int num)
{
    CString *buffer = cstr_new_size(100);

    int len = strlen(str);
    if (len < 1)
        return buffer;

    buffer = str;
    //if (buffer[len-1] != '\n')
    buffer.append('\n');

    CString *result = cstr_new_size(len + 2);

    char *pp = buffer->buffer;

    while (*pp)
    {
        if (*pp == ' ')
        {
            char n = *(pp + 1);
            if (n == '?' || n == '!')
                *pp = 0x01;
        }
        ++pp;
    }

    const char *p = buffer->buffer;
    const char *start = p;
    const char *end = 0;
    int ucount = 0;

    while (*p)
    {
        if (isspace((unsigned char) *p))
        {
            if (ucount > num)
            {
                if (!end)
                    end = p;

                result.append(start, end - start);

                while (isspace((unsigned char) *end))
                    ++end;

                start = end;
                end = 0;
                ucount = 0;

                p = start;

                result.append("\n");

                continue;
            }

            end = p;

            while (isspace((unsigned char) *p))
                _utf8inc(&p, &ucount);

            continue;
        }

        _utf8inc(&p, &ucount);
    }

    result.append(start, p - start);

    pp = result->buffer;

    while (*pp)
    {
        if (*pp == 0x01)
            *pp = ' ';

        ++pp;
    }

    if (result.last() == '\n')
        result.chop(1);

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




CString* cstr_trimmed(CString *cstr)
{
    char *src = cstr->buffer;
    bool start = true;
    int skip = 0;
    int total = 0;

    CString *result = cstr_new_size(cstr->length + 1);

    while (*src)
    {
        unsigned char c = *src;

        if (isspace(c))
        {
            if (start)
            {
                ++skip;
                ++src;
                continue;
            }
        }
        else
        {
            start = false;
            total = src + 1 - cstr->buffer;
        }

        ++src;
    }

    cstr_append_len(result, cstr->buffer + skip, total - skip);

    return result;
}

CString* cstr_leftout(CString *cstr, int length)
{
    if (length < 0)
        length = 0;
    else if (length > cstr->length)
        length = cstr->length;

    return cstr_new_len(cstr->buffer, length);
}

CString* cstr_midout(CString *cstr, int index, int length)
{
    if (index < 0 || index > cstr->length)
        index = cstr->length;

    if (length < 0)
        length = cstr->length - index;

    CString *result = cstr_new_size(length + 1);

    if ((index + length) > cstr->length)
        length = -1;

    cstr_append_len(result, cstr->buffer + index, length);

    return result;
}

CString* path_basename(const char *path)
{
    if (!path)
        return NULL;

    CString *result = cstr_new(path);

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

            cstr_append_len(result, path, length);

            return result;
        }

        ++p;
    }
}

#endif


