
#if 0

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

#endif


