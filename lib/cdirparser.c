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

    CDirParserMatch match;
    void *data;
};

CDirParser* cdirparser_new()
{
    CDirParser *parser = (CDirParser*) malloc(sizeof(CDirParser));

    parser->list = clist_new(32, (CDeleteFunc) cdirent_free);
    parser->flags = 0;
    parser->dirlen = 0;

    parser->match = NULL;
    parser->data = NULL;

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

void cdirparser_setmatch(CDirParser *parser, CDirParserMatch func, void *data)
{
    parser->match = func;
    parser->data = data;
}

bool cdirparser_read(CDirParser *parser, CString *filepath, int *type)
{
    CString *subdir = cstr_new_size(64);
    CString *item = cstr_new_size(32);

    bool ret = false;

    if (type)
        *type = (int) DT_UNKNOWN;

readnext: ;

    CDirent *currentry = (CDirent*) clist_last(parser->list);

    if (!currentry)
    {
        goto out; // nothing more to parse
    }

    int rtype;

    // get next item in the current dir
    if (!cdirent_read(currentry, item, &rtype))
    {
        // nothing more in current dir
        // close dir and continue one level up
        clist_remove_last(parser->list);
        goto readnext;
    }

    CString *currdir = cdirent_directory(currentry);

    // current item is a directory
    if (rtype == DT_DIR)
    {
        if (parser->match
            && parser->match(c_str(currdir),
                             c_str(item),
                             rtype,
                             parser->data) == false)
        {
            goto readnext;
        }

        path_join(subdir, c_str(currdir), c_str(item));

        if ((parser->flags & CDP_SUBDIRS) == CDP_SUBDIRS)
        {
            // open subdir and append to the list

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

    else
    {
        // other type, regular file etc....

        if ((parser->flags & CDP_FILES) == CDP_FILES)
        {
            if (parser->match
                && parser->match(c_str(currdir),
                                 c_str(item),
                                 rtype,
                                 parser->data) == false)
            {
                goto readnext;
            }

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


