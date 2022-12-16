#ifndef CSTRING_H
#define CSTRING_H

#include <stdbool.h>
#include <string.h>
#include <inttypes.h>

#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof(0[arr]))

#define MIN(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

#define MAX(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

typedef struct _CString CString;

#define CSTR_INITSIZE 16

// allocate -------------------------------------------------------------------

CString* cstr_alloc(int capacity, const char *str, int length);
#define cstr_new_copy(_a) cstr_alloc(_a->capacity, _a->buffer, _a->length)
#define cstr_new_len(_s, _l) cstr_alloc(-1, _s, _l)
#define cstr_new(_s) cstr_alloc(-1, _s, strlen(_s))
#define cstr_new_size(_n) cstr_alloc((_n > 0 ? _n : CSTR_INITSIZE), "", 0)
void cstr_resize(CString *cstr, int capacity);
void cstr_free(CString *cstr);
#define cstr_isempty(_a) (cstr_size(_a) == 0)

char* cstr_data(CString *cstr);
int cstr_capacity(CString *cstr);
int cstr_size(CString *cstr);
#define c_str(_a) cstr_data(_a)

// content --------------------------------------------------------------------

void cstr_swap(CString *cstr, CString *other);
void cstr_clear(CString *cstr);
void cstr_terminate(CString *cstr, int length);

// append ---------------------------------------------------------------------

#define cstr_append(_a, _s) cstr_append_len(_a, _s, strlen(_s))
void cstr_append_len(CString *cstr, const char *str, int length);
void cstr_append_c(CString *cstr, char c);

// edit -----------------------------------------------------------------------

void cstr_replace(CString *cstr, const char *find, const char *repl, bool sensitive);
void cstr_chop(CString *cstr, int n);
bool cstr_trim(CString *cstr, CString *result);
void cstr_remove(CString *cstr, int position, int n);

// extract --------------------------------------------------------------------

char cstr_first(CString *cstr);
char cstr_last(CString *cstr);
bool cstr_left(CString *cstr, CString *result, int length);
bool cstr_mid(CString *cstr, CString *result, int index, int length);

// test -----------------------------------------------------------------------
int cstr_compare(CString *cstr, const char *str, bool sensitive);
bool cstr_contains(CString *cstr, const char *str, bool sensitive);
bool cstr_startswith(CString *cstr, const char *str, bool sensitive);
bool cstr_endswith(CString *cstr, const char *str, bool sensitive);

// convert --------------------------------------------------------------------

int cstr_toint(CString *cstr);
void cstr_tolower(CString *cstr);
void cstr_toupper(CString *cstr);

// file read
bool cstr_fileread(CString *cstr, const char *filepath);
bool str_getline(char **start, CString *result);

CString* str_fmt(const char *fmt, ...);

// edit
CString* str_repeat(const char *str, int count);
CString* str_enquote(const char *str);
CString* str_unquote(const char *str);

// convert
CString* int_tostr(int num);
CString* uint64_tostr(uint64_t num);

// format
bool str_ellipsize(CString *str, int length, const char *part);
bool str_padleft(CString *str, int length, char c);
bool str_padright(CString *str, int length, char c);
CString* utf8_wrap(const char *str, int num);

#endif // CSTRING_H


