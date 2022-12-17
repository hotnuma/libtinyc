#include <cstring.h>
#include <print.h>

#define CStringAuto(_t, _s) cstr_auto_len(_t, _s, strlen(_s))

int main()
{
    //c_auto(CString) *str = cstr_new("bla");

    cstr_auto(other, "test");
    //CString_auto(test, "blablie");

    CStringAuto(test, "blablie");

    print(c_str(test));

    return 0;
}


