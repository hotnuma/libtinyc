
#if 0

CString -----------------------------------------------------------------------

void _utf8inc(const char **str, int *count)
{
    if ((**str & 0xC0) != 0x80)
        ++*count;

    ++*str;
}

void cstr_utf8wrap(CString *cstr, CString *result, const char *str, int num)
{
    int len = strlen(str);
    if (len < 1)
        return;

    //CString *buffer = cstr_new_size(100);

    cstr_copy_len(cstr, str, len);
    cstr_append_c(cstr, '\n');

    //CString *result = cstr_new_size(len + 2);

    cstr_clear(result);

    char *pp = cstr->buffer;

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

    const char *p = cstr->buffer;
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

                cstr_append_len(result, start, end - start);

                while (isspace((unsigned char) *end))
                    ++end;

                start = end;
                end = 0;
                ucount = 0;

                p = start;

                cstr_append_c(result, '\n');

                continue;
            }

            end = p;

            while (isspace((unsigned char) *p))
                _utf8inc(&p, &ucount);

            continue;
        }

        _utf8inc(&p, &ucount);
    }

    cstr_append_len(result, start, p - start);

    pp = result->buffer;

    while (*pp)
    {
        if (*pp == 0x01)
            *pp = ' ';

        ++pp;
    }

    if (cstr_last(result) == '\n')
        cstr_chop(result, 1);
}

// CiniFile -------------------------------------------------------------------

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

#endif


