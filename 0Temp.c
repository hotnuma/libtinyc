
#if 0

bool cstr_ellipsize(CString *cstr, int length, const char *part);

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


