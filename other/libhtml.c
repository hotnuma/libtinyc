#include "libhtml.h"

#include <ctype.h>
#include <string.h>

#include "print.h"

bool htmlGetTag(const char *buffer, const char **tag, int *length)
{
    if (!buffer || *buffer != '<' || !isalpha(buffer[1]))
        return false;

    const char *p = ++buffer;

    while (isalpha(*p)) ++p;

    int len = p - buffer;
    if (len < 1 || len > 12 || (*p != ' ' && *p != '>'))
        return false;

    *tag = buffer;
    *length = len;

    return true;
}

bool htmlGetElement(const char *buffer, const char **result,
                    int *length, bool outer)
{
    bool ret = false;

    const char *tstart;
    int tlength;

    if (!htmlGetTag(buffer, &tstart, &tlength))
        return ret;

    const char *p = buffer;
    const char *start = p;

    while (*p != '>' && *p != '\0') ++p;

    if (*p == '\0')
        return ret;

    //print(tag.c_str());

    ++p;

    if (!outer)
        start = p;

    CString *tag = cstr_new_len(tstart, tlength);

    // "<div>"
    CString *elem1 = str_fmt("<%s>", c_str(tag));

    // "<div "
    CString *elem2 = str_fmt("<%s ", c_str(tag));

    // "</div>"
    CString *elem3 = str_fmt("</%s>", c_str(tag));

    int count = 1;

    while (*p)
    {
        if (strncmp(p, c_str(elem1), cstr_size(elem1)) == 0)
        {
            ++count;

            p += cstr_size(elem1);

            continue;
        }
        else if (strncmp(p, c_str(elem2), cstr_size(elem2)) == 0)
        {
            ++count;

            p += cstr_size(elem2);

            continue;
        }
        else if (strncmp(p, c_str(elem3), cstr_size(elem3)) == 0)
        {
            --count;

            if (count == 0)
                break;

            p += cstr_size(elem3);

            continue;
        }

        ++p;
    }

    if (count != 0)
        goto out;

    if (outer)
    {
        *result = start;
        *length = (p + cstr_size(elem3)) - start;
    }
    else
    {
        *result = start;
        *length = p - start;
    }

    ret = true;

 out:;

    cstr_free(tag);
    cstr_free(elem1);
    cstr_free(elem2);
    cstr_free(elem3);

    return ret;
}

//void writeIndent(CString *outbuff, int indent, const CString *str)
//{
//    outbuff += strFmt("%s%s\n", strRepeat("  ", indent).c_str(), str.c_str());
//}


