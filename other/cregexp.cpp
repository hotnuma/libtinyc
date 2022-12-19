#include <CRegExp.h>
#include <string.h>

#include <print.h>

int main()
{
    const char *str = "01,bla,02,blé,03,blie";

    CRegExp rx("(\\w+)");

    int pos = 0;

    while ((pos = rx.indexIn(str, pos)) != -1)
    {
        CString part = rx.cap(0);

        print("count = %i", rx.captureCount());

        print(part);

        if (strcmp(part.c_str(), "blé") == 0)
        {
            print("> found : %s", part.c_str());
        }

        pos += rx.matchedLength();
    }

    return 0;
}


