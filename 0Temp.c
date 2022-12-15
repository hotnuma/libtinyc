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

// test main

void _createFile(const char *filepath)
{
    int fd2 = creat(filepath, 0777);

    if (fd2 != -1)
        close(fd2);
}

// create dirs
void _createDirs()
{
    struct stat st;

    if (stat(_testroot, &st) != -1)
        return;

    mkdir(_testroot, 0700);
    _createFile(_testroot"/file.txt");

    mkdir(_testroot"/dirAA", 0700);
    mkdir(_testroot"/dirAA/dirA", 0700);
    _createFile(_testroot"/dirAA/dirA/fileA.txt");

    mkdir(_testroot"/dirBB", 0700);
    mkdir(_testroot"/dirBB/dirB", 0700);
    _createFile(_testroot"/dirBB/dirB/fileB.txt");

    mkdir(_testroot"/dirCC", 0700);
    mkdir(_testroot"/dirCC/dirC", 0700);
    _createFile(_testroot"/dirCC/dirC/fileC.txt");
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

#include "CStringList.h"

cstr_CString()
{
    cstr->length = 0;
    cstr->capacity = CSTR_INITSIZE;
    cstr->buffer = (char*) malloc(cstr->capacity * sizeof(char));
    cstr->buffer[0] = '\0';
}

cstr_CString(const CString &str)
{
    cstr->length = str.size();
    cstr->capacity = cstr->length + 1;
    cstr->buffer = (char*) malloc(cstr->capacity * sizeof(char));
    memcpy(cstr->buffer, str.data(), cstr->capacity);
}

CString& cstr_operator=(const CString &s)
{
    assert(cstr->buffer);

    cstr->length = 0;
    cstr->buffer[0] = '\0';

    append(s.data(), s.size());

    return *this;
}

CString& cstr_operator=(const char *s)
{
    assert(cstr->buffer);

    cstr->length = 0;
    cstr->buffer[0] = '\0';

    append(s, strlen(s));

    return *this;
}

bool cstr_operator==(const char *str)
{
    assert(cstr->buffer);

    if (!str)
        return false;

    return (strcmp(cstr->buffer, str) == 0);
}

bool cstr_operator!=(const char *str)
{
    assert(cstr->buffer);

    if (!str)
        return false;

    return (strcmp(cstr->buffer, str) != 0);
}

void cstr_operator+=(const char *s)
{
    append(s, strlen(s));
}

char cstr_operator[](int index)
{
    if (index < 0 || index >= cstr->length)
        return 0;

    return cstr->buffer[index];
}

#endif


