#include <libstr.h>
#include <ctype.h>

#include <print.h>

int main()
{
    CString buff = "   123   456   789    1 2 3 4 5 6    ";
    int nparts = 3;

    char *ptr = buff.data();
    char *result;
    int length;

    int count = 0;
    while (count < nparts && strGetPart(&ptr, &result, &length))
    {
        result[length] = '\0';

        print(result);

        ++count;
    }

    while(isspace(*ptr)) ++ptr;

    // end of buffer ?
    if (*ptr == '\0')
        return -1;

    print(ptr);

    return 0;
}


