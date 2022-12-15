#include "CDirParser.h"
#include "CDirent.h"
#include "libpath.h"
#include <sys/stat.h>

#include "print.h"

DELETEFUNC(CDirent)

CDirParser::CDirParser()
{
    SETDELETEFUNC(&_list, CDirent);
}

CDirParser::CDirParser(const char *directory, int flags)
{
    SETDELETEFUNC(&_list, CDirent);

    open(directory, flags);
}

void CDirParser::close()
{
    _list.clear();
}

bool CDirParser::open(const char *directory, int flags)
{
    close();

    if (!flags)
        return false;

    _flags = flags;

    CDirent *entry = new CDirent();

    if (!entry->open(directory))
    {
        delete entry;
        return false;
    }

    _dirlen = entry->directory().size();
    _list.append(entry);

    return true;
}

bool CDirParser::read(CString &filepath, int* /*type*/)
{
    readnext:
    CDirent *entry = (CDirent*) _list.last();

    if (!entry)
        return false; // nothing more to browse

    CString item;
    int rtype;

    // get next item in the current dir
    if (!entry->read(item, &rtype))
    {
        // nothing more in current dir
        // close dir and continue one level up
        _list.removeLast();
        goto readnext;
    }

    const CString &currdir = entry->directory();

    // current item is a directory
    if (rtype == DT_DIR)
    {
        CString subdir = pathJoin(currdir, item);

        // open sub dir and append to the list
        if ((_flags & CDP_SUBDIRS) == CDP_SUBDIRS)
        {
            CDirent *subentry = new CDirent();
            if (!subentry->open(subdir))
            {
                delete subentry;

                // what to do if we fail ?

                goto readnext;
            }

            _list.append(subentry);
        }

        if ((_flags & CDP_DIRS) == CDP_DIRS)
        {
            // we want dirs so output subdir
            filepath.clear();

            if (_flags & CDP_RELATIVE)
            {
                if (subdir.size() <= _dirlen)
                    goto readnext;

                filepath.append(subdir.c_str() + _dirlen + 1);
            }
            else
            {
                filepath.append(subdir);
            }

            return true;
        }
        else
        {
            // we don't want dirs, continue
            goto readnext;
        }
    }

    // other type, regular file etc....
    else
    {
        if ((_flags & CDP_FILES) == CDP_FILES)
        {
            // we want files so output file item
            filepath.clear();

            if (_flags & CDP_RELATIVE)
            {
                if (currdir.size() > _dirlen)
                {
                    filepath.append(currdir.c_str() + _dirlen + 1);
                    filepath.append(PATHSEP);
                }

                filepath.append(item);
            }
            else
            {
                filepath.append(currdir);
                filepath.append(PATHSEP);
                filepath.append(item);
            }

            return true;
        }
        else
        {
            // we don't want files, continue
            goto readnext;
        }
    }
}


