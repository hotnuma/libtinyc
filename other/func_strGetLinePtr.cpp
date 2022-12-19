#include <CString.h>

#include <print.h>

int main()
{
    CString buffer = "bla\nble\nblie";

    char *ptr = buffer.data();
    char *result;
    int length;

    while (strGetLinePtr(&ptr, &result, &length))
    {
        result[length] = '\0';
        print(result);
    }

    return 0;
}


