#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <wordexp.h>

#include <print.h>

#define BUFF_SIZE 256

#define FD_OUT 0
#define FD_IN 1

int main()
{
    char buff[BUFF_SIZE];
    memset(buff, 0, sizeof(buff));

    const char *cmd = "ls -la";

    if (!cmd || !*cmd)
        exit(EXIT_FAILURE);

    int _outPipe[2];

    // create pipes.
    if (pipe(_outPipe) == -1)
    {
        perror("pipe failed\n");
        return 1;
    }

    wordexp_t we;
    wordexp(cmd, &we, 0);
    char **w = we.we_wordv;

    int pid;
    if ((pid = fork()) == -1)
    {
        perror("fork failed\n");

        wordfree(&we);
        return 2;
    }

    if (pid == 0)
    {
        dup2(_outPipe[FD_IN], STDOUT_FILENO);
        close(_outPipe[FD_OUT]);

        if (execvp(w[0], (char**) w) == -1)
            perror("could not execve...");

        exit(EXIT_FAILURE);
    }
    else
    {
        close(_outPipe[FD_IN]);

        fd_set readfds;
        struct timeval timeout;

        while (waitpid(pid, nullptr, WNOHANG) != pid)
        {
            timeout.tv_sec = 5;
            timeout.tv_usec = 0;

            FD_ZERO(&readfds);

            FD_SET(_outPipe[FD_OUT], &readfds);

            select(FD_SETSIZE, &readfds, nullptr, nullptr, &timeout);

            if (FD_ISSET(_outPipe[FD_OUT], &readfds))
            {
                int nb_read = read(_outPipe[FD_OUT], buff, BUFF_SIZE - 1);

                if (nb_read > 0)
                {
                    buff[nb_read] = '\0';
                    print("line : %s", buff);
                }
                else
                {
                    print("nothing to read");
                }
            }
        }
    }

    close(_outPipe[FD_OUT]);

    wordfree(&we);

    return 0;
}


