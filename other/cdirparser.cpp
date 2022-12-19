#include <CDirParser.h>

#include <print.h>

int main()
{
    const char *indir = "/proc/asound";

    CDirParser dir;
    if (!dir.open(indir, CDP_FILES | CDP_SUBDIRS))
        return 1;

    CString filepath;

    while (dir.read(filepath))
    {
        print(filepath);
    }

    return 0;
}


