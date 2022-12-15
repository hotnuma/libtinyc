#ifndef STRPATH_H
#define STRPATH_H

#include "CString.h"

#define PATHSEP '/'

const char* pathSep(const char *path);
const char* pathExt(const char *path, bool first = true);

int pathCmp(const char *s1, const char *s2);

bool pathStripExt(CString &path);
CString pathDirName(const char *path);
CString pathBaseName(const char *path);
CString pathJoin(const char *dir, const char *file);

bool pathCanonicalize(char *path, int *len);

#endif // STRPATH_H


