#ifndef STRPATH_H
#define STRPATH_H

#include "cstring.h"

#define PATHSEP '/'

// sep
bool path_dirname(CString *result, const char *path);
bool path_basename(CString *result, const char *path);
const char* path_sep(const char *path);

// ext
const char* path_ext(const char *path, bool first);
bool path_strip_ext(CString *path);

// path
void path_join(CString *result, const char *dir, const char *file);
bool path_canonicalize(char *path, int *len);

int path_cmp(const char *s1, const char *s2);
void path_basename_sp(CString *cstr, const char *path);

#endif // STRPATH_H


