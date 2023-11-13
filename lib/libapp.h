#ifndef LIBAPP_H
#define LIBAPP_H

#include "cstring.h"

bool app_isfirst(const char *lockfile);

void get_apppath(CString *result);
void get_appdir(CString *result);

void get_homedir(CString *result);
void get_configdir(CString *result);
void get_username(CString *result);
void get_localtime(CString *result, const char *fmt);

int pexec(const char *cmd);

#endif // LIBAPP_H


