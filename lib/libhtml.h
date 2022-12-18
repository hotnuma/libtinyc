#ifndef LIBHTML_H
#define LIBHTML_H

#include "cstring.h"

bool html_get_tag(const char *buffer, const char **tag, int *length);
bool html_get_element(const char *buffer, const char **result, int *length, bool outer);

void write_indent(CString *outbuff, int indent, const char *str);

#endif // LIBHTML_H


