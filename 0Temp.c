
#if 0

#define cstr_auto(_n, _s) cstr_auto_len(_n, _s, strlen(_s))
#define cstr_auto_len(_n, _s, _l) _CCLEANUP(_freeCString) \
    CString *(_n) = cstr_new_len(_s, _l)

#define c_auto(t) _CCLEANUP(_free##t) t

#endif


