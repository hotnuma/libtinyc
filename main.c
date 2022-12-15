#include <print.h>
#include <cstring.h>

int main()
{
    CString *str = cstr_new_len("bla", -1);
    CString *other = cstr_new(c_str(str));

    cstr_append_len(str, " ble ", -1);
    cstr_append(str, c_str(other));

    print(c_str(str));
    print(c_str(other));

    cstr_free(str);
    cstr_free(other);

    return 0;
}


