#include <CString.h>

#include <print.h>

int main()
{
    CString buffer = "bla\nble\nblie";

    char *ptr = buffer.data();
    CString line;

    while (strGetLine(&ptr, line))
    {
        print(line);
    }

    return 0;
}


