#include <cstring.h>

#include <stdlib.h>
#include <string.h>
#include <print.h>


int main()
{
    c_autofree char *buffer = (char*) malloc(sizeof(char) * 32);
    strcpy(buffer, "blablie");

    cstr_auto CString *other = cstr_new("test");

    print(c_str(other));
    print(buffer);

    return 0;
}


