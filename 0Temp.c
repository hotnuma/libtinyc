
#if 0

//int cstr_toint(CString *cstr);
//int cstr_toint(CString *cstr)
//{
//    return atoi(cstr->buffer);
//}

//cstr_copy(strA, "10");
//ASSERT(cstr_toint(strA) == 10);

#ifdef 0
//GLIB_CHECK_VERSION
#include <glib-object.h>
#define c_autounref GC_CLEANUP(_c_autounref_func)

GC_UNUSED static inline void _c_autounref_func(void *p)
{
    void **pp = (void**) p;

    if (*pp)
        g_object_unref(*pp);
}
#endif

#endif


