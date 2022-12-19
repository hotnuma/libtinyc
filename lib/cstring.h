#ifndef CSTRING_H
#define CSTRING_H

#include <inttypes.h>
#include <stdbool.h>
#include <string.h>

#include "macros.h"

typedef struct _CString CString;

#define CSTR_INITSIZE 16

// allocate -------------------------------------------------------------------

#define cstr_auto(_n, _s) cstr_auto_len(_n, _s, strlen(_s))
#define cstr_auto_len(_n, _s, _l) _CCLEANUP(_freeCString) \
    CString *(_n) = cstr_new_len(_s, _l)

CString* cstr_alloc(int capacity, const char *str, int length);
#define cstr_new_copy(_a) cstr_alloc(_a->capacity, _a->buffer, _a->length)
#define cstr_new(_s) cstr_alloc(-1, _s, strlen(_s))
#define cstr_new_len(_s, _l) cstr_alloc(-1, _s, _l)
#define cstr_new_size(_n) cstr_alloc((_n > 0 ? _n : CSTR_INITSIZE), "", 0)
void cstr_resize(CString *cstr, int capacity);
void cstr_free(CString *cstr);
void _freeCString(CString **cstr);

// content --------------------------------------------------------------------

#define cstr_isempty(_a) (cstr_size(_a) == 0)
void cstr_clear(CString *cstr);
void cstr_swap(CString *cstr, CString *other);
void cstr_terminate(CString *cstr, int length);
char* cstr_data(CString *cstr);
int cstr_capacity(CString *cstr);
int cstr_size(CString *cstr);
#define c_str(_a) cstr_data(_a)

// edit -----------------------------------------------------------------------

#define cstr_copy(_a, _s) cstr_copy_len(_a, _s, strlen(_s))
void cstr_copy_len(CString *cstr, const char *str, int length);
#define cstr_append(_a, _s) cstr_append_len(_a, _s, strlen(_s))
void cstr_append_len(CString *cstr, const char *str, int length);
void cstr_append_c(CString *cstr, char c);
void cstr_remove(CString *cstr, int position, int n);
void cstr_replace(CString *cstr, const char *find, const char *repl, bool sensitive);
void cstr_chop(CString *cstr, int n);
bool cstr_trim(CString *cstr, CString *result);
void cstr_fmt(CString *cstr, const char *fmt, ...);

// extract --------------------------------------------------------------------

char cstr_first(CString *cstr);
char cstr_last(CString *cstr);
bool cstr_left(CString *cstr, CString *result, int length);
bool cstr_mid(CString *cstr, CString *result, int index, int length);

// tests ----------------------------------------------------------------------

int cstr_compare(CString *cstr, const char *str, bool sensitive);
bool cstr_contains(CString *cstr, const char *str, bool sensitive);
bool cstr_startswith(CString *cstr, const char *str, bool sensitive);
bool cstr_endswith(CString *cstr, const char *str, bool sensitive);

// convert --------------------------------------------------------------------

int cstr_toint(CString *cstr);
void cstr_tolower(CString *cstr);
void cstr_toupper(CString *cstr);
void cstr_int(CString *cstr, int val);
void cstr_uint64(CString *cstr, uint64_t val);

// read file ------------------------------------------------------------------

bool cstr_fileread(CString *cstr, const char *filepath);
bool str_getline(char **start, CString *result);

// utils ----------------------------------------------------------------------

void cstr_repeat(CString *cstr, const char *str, int count);
void cstr_enquote(CString *cstr, const char *str);
void cstr_unquote(CString *cstr, const char *str);
bool cstr_ellipsize(CString *cstr, int length, const char *part);
bool cstr_padleft(CString *cstr, int length, char c);
bool cstr_padright(CString *cstr, int length, char c);

#endif // CSTRING_H


