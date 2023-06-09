#include "cdirparser.h"

#include "cdirent.h"
#include "clist.h"
#include "libpath.h"
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>

#include "print.h"

struct _CDirParser
{
    CList *list;
    int flags;
    int dirlen;
};

CDirParser* cdirparser_new()
{
    CDirParser *parser = (CDirParser*) malloc(sizeof(CDirParser));

    parser->list = clist_new(32, (CDeleteFunc) cdirent_free);
    parser->flags = 0;
    parser->dirlen = 0;

    return parser;
}

void cdirparser_free(CDirParser *parser)
{
    if (parser == NULL)
        return;

    clist_free(parser->list);
    free(parser);
}

bool cdirparser_open(CDirParser *parser, const char *directory, int flags)
{
    cdirparser_close(parser);

    if (!flags)
        flags = (CDP_DIRS | CDP_FILES);

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

void cdirparser_close(CDirParser *parser)
{
    clist_clear(parser->list);
    parser->flags = 0;
    parser->dirlen = 0;
}

bool cdirparser_read(CDirParser *parser, CString *filepath, int *type)
{
    CString *item = cstr_new_size(32);
    CString *subdir = cstr_new_size(32);
    bool ret = false;

    if (type)
        *type = (int) DT_UNKNOWN;

readnext: ;

    CDirent *entry = (CDirent*) clist_last(parser->list);

    if (!entry)
    {
        goto out;
    }

    int rtype;

    // get next item in the current dir
    if (!cdirent_read(entry, item, &rtype))
    {
        // nothing more in current dir
        // close dir and continue one level up
        clist_remove_last(parser->list);
        goto readnext;
    }

    CString *currdir = cdirent_directory(entry);

    // current item is a directory
    if (rtype == DT_DIR)
    {
        path_join(subdir, c_str(currdir), c_str(item));

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

            ret = true;
            goto out;
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

            ret = true;
            goto out;
        }
        else
        {
            // we don't want files, continue
            goto readnext;
        }
    }

out:;

    cstr_free(item);
    cstr_free(subdir);

    if (type)
        *type = (int) rtype;

    return ret;
}


