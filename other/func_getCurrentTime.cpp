#include <libapp.h>
#include <time.h>

#include <print.h>

int main()
{
    CString result = getCurrentTime("%b %d %H:%M:%S");

    print(result);

    struct tm tm;
    time_t epoch;

    if (strptime(result, "%b %d %H:%M:%S", &tm) != nullptr)
    {
        epoch = mktime(&tm);

        print("%" PRIu64, epoch);
    }


    return 0;
}



