#ifndef STRPATH_H
#define STRPATH_H

#include "cstring.h"

#define PATHSEP '/'

const char* path_sep(const char *path);
const char* path_ext(const char *path, bool first);
bool path_strip_ext(CString *path);
bool path_dirname(CString *result, const char *path);
bool path_basename(CString *result, const char *path);

//int pathCmp(const char *s1, const char *s2);

//CString* path_basename(const char *path);

void path_join(CString *result, const char *dir, const char *file);

//bool pathCanonicalize(char *path, int *len);

#endif // STRPATH_H


