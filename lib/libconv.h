#ifndef LIBCONV_H
#define LIBCONV_H

#include "libmacros.h"
#include <stdio.h>

bool iconvert(const char *str, size_t len,
              char **outbuff, size_t *outsize,
              const char *fromcharset, const char *tocharset);

#endif // LIBCONV_H


