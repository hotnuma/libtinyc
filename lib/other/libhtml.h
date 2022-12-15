#ifndef LIBHTML_H
#define LIBHTML_H

#include "CString.h"

bool htmlGetTag(const char *buffer, const char **tag, int *length);
bool htmlGetElement(const char *buffer, const char **result,
                    int *length, bool outer = false);

void writeIndent(CString &outbuff, int indent, const CString &str);

#endif // LIBHTML_H


