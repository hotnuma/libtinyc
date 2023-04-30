
#if 0

// https://stackoverflow.com/questions/7090998/
#ifndef UNUSED
#define UNUSED(x) (void)(x)
#endif

void cstr_repeat(CString *cstr, const char *str, int count);
bool cstr_padleft(CString *cstr, char c, int length);
bool cstr_padright(CString *cstr, char c, int length);
bool cstr_ellipsize(CString *cstr, int length, const char *part);

void cstr_repeat(CString *cstr, const char *str, int count)
{
    if (!str || count < 1)
        return;

    int len = strlen(str);

    cstr_clear(cstr);
    cstr_resize(cstr, (len * count) + 1);

    while (count > 0)
    {
        cstr_append_len(cstr, str, len);

        --count;
    }
}

bool cstr_padleft(CString *cstr, char c, int length)
{
    int size = cstr->length;

    if (size < 1 || length < 1 || length <= size)
        return false;

    CString *buffer = cstr_new_size(length + 1);

    int delta = length - size;

    for (int i = 0; i < delta; ++i)
    {
        cstr_append_c(buffer, c);
    }

    cstr_append(buffer, c_str(cstr));
    cstr_swap(buffer, cstr);
    cstr_free(buffer);

    return true;
}

bool cstr_padright(CString *cstr, char c, int length)
{
    int delta = length - cstr->length;

    if (length < 1 || delta < 1)
        return false;

    cstr_resize(cstr, length + 1);

    for (int i = 0; i < delta; ++i)
    {
        cstr_append_c(cstr, c);
    }

    return true;
}

bool cstr_ellipsize(CString *cstr, int length, const char *part)
{
    int partlen = strlen(part);

    if (length < 2 || partlen < 1 || length <= partlen)
        return false;

    int size = cstr->length;

    if (size <= length)
        return true;

    length -= partlen;

    cstr_terminate(cstr, length);
    cstr_append(cstr, part);

    return true;
}

#endif


