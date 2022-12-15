#include "libconv.h"

#include <iconv.h>
#include <errno.h>
#include <stdlib.h>

bool iconvert(const char *str, size_t len,
              char **outbuff, size_t *outsize,
              const char *fromcharset, const char *tocharset)
{
    iconv_t conv = iconv_open(tocharset, fromcharset);
    if (conv == (iconv_t) -1)
        return false;

    const char *p = str;
    size_t inbytes_remaining = len;

    char *outp = *outbuff;
    size_t outbytes_remaining = *outsize - 1;

    bool done = false;
    bool have_error = false;
    size_t err = 0;

    while (!done && !have_error)
    {
        err = iconv(conv, (char **)&p, &inbytes_remaining,
                    &outp, &outbytes_remaining);

        if (err == (size_t) -1)
        {
            switch (errno)
            {
                case EINVAL:
                {
                    // Incomplete text.
                    done = true;
                    break;
                }
                case E2BIG:
                {
                    size_t used = outp - *outbuff;
                    *outsize *= 2;
                    *outbuff = (char *) realloc(*outbuff, *outsize);
                    if (*outbuff == nullptr)
                    {
                        iconv_close(conv);
                        return false;
                    }
                    outp = *outbuff + used;
                    outbytes_remaining = *outsize - used - 1;
                    break;
                }
                case EILSEQ:
                {
                    have_error = true;
                    break;
                }
                default:
                {
                    have_error = true;
                    break;
                }
            }
        }
        else if (err > 0)
        {
            // err gives the number of replacement characters used.
            have_error = true;
        }
        else
        {
            done = true;
        }
    }

    iconv_close(conv);
    *outp = '\0';

    if (!have_error)
        return true;

    return false;
}


