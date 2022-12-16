#include "cdirparser.h"

#include "cdirent.h"
#include "clist.h"
#include "libpath.h"
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>

#include "print.h"

//DELETEFUNC(CDirent)

struct _CDirParser
{
    CList *list;
    int flags;
    int dirlen;
};

CDirParser* cdirparser_new()
{
    CDirParser *parser = (CDirParser*) malloc(sizeof(CDirParser));

    //SETDELETEFUNC(parser->list, CDirent);

    clist_set_deletefunc(parser->list, (CDeleteFunc) cdirent_free);

    return parser;
}

//cdirparser_CDirParser(CDirParser *parser, const char *directory, int flags)
//{
//    //SETDELETEFUNC(parser->list, CDirent);

//    clist_set_deletefunc(parser->list, (CDeleteFunc) cdirent_free);

//    open(directory, flags);
//}

void cdirparser_close(CDirParser *parser)
{
    clist_clear(parser->list);
}

bool cdirparser_open(CDirParser *parser, const char *directory, int flags)
{
    cdirparser_close(parser);

    if (!flags)
        return false;

    parser->flags = flags;

    CDirent *entry = cdirent_new();

    if (!cdirent_open(entry, directory))
    {
        cdirent_free(entry);
        return false;
    }

    parser->dirlen = cstr_size(cdirent_directory(entry));
    clist_append(parser->list, entry);

    return true;
}

bool cdirparser_read(CDirParser *parser, CString *filepath /*, int* type*/)
{
    readnext: ;

    CDirent *entry = (CDirent*) clist_last(parser->list);

    if (!entry)
        return false; // nothing more to browse

    CString *item = NULL;
    int rtype;

    // get next item in the current dir
    if (!cdirent_read(entry, item, &rtype))
    {
        // nothing more in current dir
        // close dir and continue one level up
        clist_removeLast(parser->list);
        goto readnext;
    }

    CString *currdir = cdirent_directory(entry);

    // current item is a directory
    if (rtype == DT_DIR)
    {
        CString *subdir = pathJoin(c_str(currdir), c_str(item));

        // open sub dir and append to the list
        if ((parser->flags & CDP_SUBDIRS) == CDP_SUBDIRS)
        {
            CDirent *subentry = cdirent_new();

            if (!cdirent_open(subentry, c_str(subdir)))
            {
                cdirent_free(subentry);

                // what to do if we fail ?

                goto readnext;
            }

            clist_append(parser->list, subentry);
        }

        if ((parser->flags & CDP_DIRS) == CDP_DIRS)
        {
            // we want dirs so output subdir
            cstr_clear(filepath);

            if (parser->flags & CDP_RELATIVE)
            {
                if (cstr_size(subdir) <= parser->dirlen)
                    goto readnext;

                cstr_append(filepath, c_str(subdir) + parser->dirlen + 1);
            }
            else
            {
                cstr_append(filepath, c_str(subdir));
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
        if ((parser->flags & CDP_FILES) == CDP_FILES)
        {
            // we want files so output file item
            cstr_clear(filepath);

            if (parser->flags & CDP_RELATIVE)
            {
                if (cstr_size(currdir) > parser->dirlen)
                {
                    cstr_append(filepath, c_str(currdir) + parser->dirlen + 1);
                    cstr_append_c(filepath, PATHSEP);
                }

                cstr_append(filepath, c_str(item));
            }
            else
            {
                cstr_append(filepath, c_str(currdir));
                cstr_append_c(filepath, PATHSEP);
                cstr_append(filepath, c_str(item));
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



