#include "cprocess.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <wordexp.h>

#define CHUNCK 1024

static char* _get_ptr(CString *buffer, int minchuck)
{
    int capacity = cstr_capacity(buffer);
    int size = cstr_size(buffer);

    if (minchuck < 1)
        minchuck = 1024;

    if (capacity < 1)
    {
        cstr_resize(buffer, minchuck * 2);
        return cstr_data(buffer) + cstr_size(buffer);
    }

    while (capacity - (size + 1) < minchuck)
    {
        //print("while");

        capacity *= 2;
    }

    cstr_resize(buffer, capacity);

    return cstr_data(buffer) + cstr_size(buffer);
}

static int _read_pipe(int fd, CString *buffer)
{
    if (fd < 0)
        return -1;

    char *ptr = _get_ptr(buffer, CHUNCK);
    int nb_read = read(fd, ptr, CHUNCK - 1);

    if (nb_read < 1)
    {
        //print("nothing to read");

        return 0;
    }

    //print("%d bytes read", nb_read);

    cstr_terminate(buffer, cstr_size(buffer) + nb_read);

    return nb_read;
}

struct _CProcess
{
    int inpipe[2];  // = {CPH_INVALID};
    int outpipe[2]; // = {CPH_INVALID};
    int errpipe[2]; // = {CPH_INVALID};

    int exitstatus; // = -1;

    CString *inbuff;
    CString *outbuff;
    CString *errbuff;
};

CProcess* cprocess_new()
{
    CProcess *cpr = (CProcess*) malloc(sizeof(CProcess));

    cpr->inpipe[0] = CPH_INVALID;
    cpr->inpipe[1] = CPH_INVALID;
    cpr->outpipe[0] = CPH_INVALID;
    cpr->outpipe[1] = CPH_INVALID;
    cpr->errpipe[0] = CPH_INVALID;
    cpr->errpipe[1] = CPH_INVALID;

    cpr->exitstatus = -1;

    cpr->inbuff = cstr_new_size(128);
    cpr->outbuff = cstr_new_size(128);
    cpr->errbuff = cstr_new_size(128);

    return cpr;
}

void cprocess_free(CProcess *cpr)
{
    if (cpr == NULL)
        return;

    cstr_free(cpr->inbuff);
    cstr_free(cpr->outbuff);
    cstr_free(cpr->errbuff);

    free(cpr);
}

bool cprocess_start(CProcess *cpr, const char *cmd, int flags)
{
    if (!cmd || !*cmd)
        return false;

    bool outpipe = flags & CP_PIPEOUT;

    cpr->exitstatus = -1;

    // Create pipes.
    if (outpipe)
    {
        if (pipe(cpr->outpipe) == -1)
            return false;
    }

    wordexp_t we;
    wordexp(cmd, &we, 0);

    int pid = fork();

    if (pid < 0)
    {
        wordfree(&we);
        return false;
    }
    else if (pid == 0)
    {
        if (outpipe)
        {
            dup2(cpr->outpipe[CPH_IN], STDOUT_FILENO);
            close(cpr->outpipe[CPH_OUT]);
        }

        char **w = we.we_wordv;

        execvp(w[0], (char**) w);

        exit(EXIT_FAILURE);
    }

    fd_set readfds;
    struct timeval timeout;

    if (outpipe)
    {
        close(cpr->outpipe[CPH_IN]);

        cstr_resize(cpr->outbuff, CHUNCK * 2);
    }

    int status = -1;

    while (waitpid(pid, &status, WNOHANG) != pid)
    {
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;

        FD_ZERO(&readfds);

        if (outpipe)
            FD_SET(cpr->outpipe[CPH_OUT], &readfds);

        select(FD_SETSIZE, &readfds, NULL, NULL, &timeout);

        if (outpipe && FD_ISSET(cpr->outpipe[CPH_OUT], &readfds))
        {
            _read_pipe(cpr->outpipe[CPH_OUT], cpr->outbuff);
        }
    }

    if (outpipe)
        close(cpr->outpipe[CPH_OUT]);

    wordfree(&we);

    if (WIFEXITED(status))
        cpr->exitstatus = WEXITSTATUS(status);

    //print("status = %d", _exitCode);

    return true;
}

CString *cprocess_outbuff(CProcess *cpr)
{
    return cpr->outbuff;
}

int cprocess_exitstatus(CProcess *cpr)
{
    return cpr->exitstatus;
}


