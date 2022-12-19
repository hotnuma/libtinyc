#include <CString.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include <print.h>

int main()
{
    CString buffer(2048);

    strFileRead(buffer, "/proc/meminfo");

    print(buffer);

    print("size = %d", buffer.size());
    print("capacity = %d", buffer.capacity());

    return 0;
}


