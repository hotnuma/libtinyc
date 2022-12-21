#ifndef LIBAPP_H
#define LIBAPP_H

#include "cstring.h"

void get_apppath(CString *result);
void get_appdir(CString *result);

void get_homedir(CString *result);
void get_configdir(CString *result);
void get_username(CString *result);
void get_localtime(CString *result, const char *fmt);

bool dir_exists(const char *fileName);
bool file_exists(const char *fileName);
bool file_remove(const char *fileName);

int pexec(const char *cmd);

#endif // LIBAPP_H


