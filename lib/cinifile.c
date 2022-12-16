#include "cinifile.h"

#include <assert.h>
#include "cfile.h"
#include "libstr.h"
#include "print.h"

//void _deleteSection(CIniSection *section)
//{
//    if (section)
//        delete section;
//}

#if 0
DELETEFUNC(CIniSection)

CIniFile::CIniFile()
{
    SETDELETEFUNC(&_sectionList, CIniSection);

    //_sectionList.setDeleteFunc((CDeleteFunc) _deleteSection);
}

bool CIniFile::open(const char *filepath)
{
    CFile file;
    if (!file.read(filepath))
        return false;

    _filepath = filepath;

    char *ptr = file.data();
    char *result = NULL;
    int length = 0;

    CIniSection *section = NULL;
    int count = 0;

    while (strGetLinePtr(&ptr, &result, &length))
    {
        // skip empty lines.
        //if (length == 0)
        //    continue;

        ++count;

        result[length] = '\0';

        char *sec = getSection(result, length);
        if (count == 1 && !sec)
        {
            // default section.
            section = new CIniSection();
            _sectionList.append(section);
            continue;
        }
        else if (sec)
        {
            section = new CIniSection(sec);
            _sectionList.append(section);
            continue;
        }

        assert(section);

        // append line in current section.
        section->append(result);
    }

    return true;
}

CIniSection* CIniFile::section(const char *section)
{
    int size = _sectionList.size();
    for (int i = 0; i < size; ++i)
    {
        CIniSection *iniSection = (CIniSection*) _sectionList[i];
        if (iniSection->name() == section)
            return iniSection;
    }

    return NULL;
}
#endif


#if 0

bool CIniFile::save()
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

bool CIniFile::saveAs(const char *filepath)
{
    _filepath = filepath;

    return save();
}

CStringList CIniFile::allSections()
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

#endif


