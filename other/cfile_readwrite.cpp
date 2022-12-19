#include <CFile.h>

#include <print.h>

int main()
{
    const char *inpath = "infile.txt";
    const char *outpath = "outfile.txt";

    CFile file;
    if (!file.read(inpath))
        return 1;

    CFile outfile;
    if (!outfile.open(outpath, "wb"))
        return 1;

    CString line;
    while (file.getLine(line))
    {
        outfile << line;
        outfile << "\n";

        print(line);
    }

    return 0;
}


