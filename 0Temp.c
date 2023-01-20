
#if 0

// auto free ------------------------------------------------------------------

#define CStringAuto GC_CLEANUP(_freeCString) CString
#define cstr_auto GC_CLEANUP(_freeCString)
GC_UNUSED static inline void _freeCString(CString **cstr)
{
    //print("_freeCString");

    cstr_free(*cstr);
}

#define cstr_auto(_n, _s) cstr_auto_len(_n, _s, strlen(_s))
#define cstr_auto_len(_n, _s, _l) GC_CLEANUP(_freeCString) \
    CString *(_n) = cstr_new_len(_s, _l)

#define c_auto(t) GC_CLEANUP(_free##t) t

#define cstr_auto GC_CLEANUP(_freeCString)

#endif


