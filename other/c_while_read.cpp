#include <CString.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include <print.h>

#define CHUNCK 1024

int main()
{
    CString buffer(CHUNCK * 2);

    int fd = open("/home/hotnuma/sys_prcquery.txt", O_RDONLY);
    if (fd < 0)
        return 1;

    for (int i = 0; i < 10; ++i)
    {
        char *buff = getPtr(buffer, CHUNCK);
        int nb_read = read(fd, buff, CHUNCK - 1);

        if (nb_read < 1)
        {
            print("eof");
            break;
        }

        print("%d bytes read", nb_read);
        buffer.terminate(buffer.size() + nb_read);
    }

    close(fd);

    print(buffer);

    return 0;
}


