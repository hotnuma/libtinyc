#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

// The C11 way : timespec_get(&tms, TIME_UTC)

void print_unixtime()
{
    printf("timestamp: %" PRId64 "\n", time(nullptr));
}

void print_microseconds()
{
    struct timespec tms;

    // POSIX.1-2008 way
    if (clock_gettime(CLOCK_REALTIME, &tms))
    {
        return;
    }

    // seconds, multiplied with 1 million
    int64_t micros = tms.tv_sec * 1000000;

    // Add full microseconds
    micros += tms.tv_nsec/1000;

    // round up if necessary
    if (tms.tv_nsec % 1000 >= 500)
    {
        ++micros;
    }

    printf("µ seconds: %" PRId64 "\n", micros);
}

int main(void)
{
    setbuf(stdout, nullptr);

    print_unixtime();
    print_microseconds();

    return 0;
}


