

#if 0

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



// CiniFile

void clear();
int _addSectionTxt(CStringList &allLines, int fromline, int toline);

bool cinifile_save(CIniFile *inifile)
{
    // Output file.
    CFile outFile;
    if (!outFile.open(_filepath, "wb"))
        return false;

    int size = _sectionList.size();
    for (int i = 0; i < size; ++i)
    {
        CIniSection *section = (CIniSection*) _sectionList[i];
        section->writeSectionTxt(outFile);
    }

    return true;
}

bool cinifile_saveAs(CIniFile *inifile, const char *filepath)
{
    _filepath = filepath;

    return save();
}

CStringList cinifile_allSections(CIniFile *inifile)
{
    CStringList result;

    int size = _sectionList.size();
    for (int i = 0; i < size; ++i)
    {
        CIniSection *section = (CIniSection*) _sectionList[i];
        result.append(section->name());
    }

    return result;
}

void cinisection_setValue(CIniSection *section, const char *key, const char *value)
{
    CIniLine *iniLine = find(key);

    if (!iniLine)
        return;

    iniLine->setValue(value);
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


