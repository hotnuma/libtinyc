
#if 0

// libstr ---------------------------------------------------------------------

bool str_getpart(char **start, char **result, int *length);

bool str_getpart(char **start, char **result, int *length)
{
    // start of line.
    char *first = *start;

    while (isspace(*first)) ++first;

    // end of buffer ?
    if (*first == '\0')
        return false;

    // search end of line.
    char *p = first;

    while (1)
    {
        if (isspace(*p))
        {
            *result = first;
            *length = p - first;

            // move to the end.
            *start = ++p;

            return true;
        }
        else if (*p == '\0')
        {
            *result = first;
            *length = p - first;

            // move to the end.
            *start = p;

            return true;
        }

        ++p;
    }
}

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


