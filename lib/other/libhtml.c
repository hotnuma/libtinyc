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
    const char *tstart;
    int tlength;

    if (!htmlGetTag(buffer, &tstart, &tlength))
        return false;

    const char *p = buffer;
    const char *start = p;

    while (*p != '>' && *p != '\0') ++p;

    if (*p == '\0')
        return false;

    //print(tag.c_str());

    ++p;

    if (!outer)
        start = p;

    CString tag;
    tag.append(tstart, tlength);

    // "<div>"
    CString elem1 = strFmt("<%s>", tag.c_str());

    // "<div "
    CString elem2 = strFmt("<%s ", tag.c_str());

    // "</div>"
    CString elem3 = strFmt("</%s>", tag.c_str());

    int count = 1;

    while (*p)
    {
        if (strncmp(p, elem1.c_str(), elem1.size()) == 0)
        {
            ++count;

            p += elem1.size();

            continue;
        }
        else if (strncmp(p, elem2.c_str(), elem2.size()) == 0)
        {
            ++count;

            p += elem2.size();

            continue;
        }
        else if (strncmp(p, elem3.c_str(), elem3.size()) == 0)
        {
            --count;

            if (count == 0)
                break;

            p += elem3.size();

            continue;
        }

        ++p;
    }

    if (count != 0)
        return false;

    if (outer)
    {
        *result = start;
        *length = (p + elem3.size()) - start;
    }
    else
    {
        *result = start;
        *length = p - start;
    }

    return true;
}

void writeIndent(CString &outbuff, int indent, const CString &str)
{
    outbuff += strFmt("%s%s\n", strRepeat("  ", indent).c_str(), str.c_str());
}


