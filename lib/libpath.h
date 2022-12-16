#ifndef STRPATH_H
#define STRPATH_H

#include "cstring.h"

#define PATHSEP '/'

const char* pathSep(const char *path);
const char* pathExt(const char *path, bool first);

int pathCmp(const char *s1, const char *s2);

bool pathStripExt(CString *path);
CString* pathDirName(const char *path);
CString* pathBaseName(const char *path);
CString* pathJoin(const char *dir, const char *file);

bool pathCanonicalize(char *path, int *len);

// path
CString* path_basename(const char *path);

#endif // STRPATH_H


