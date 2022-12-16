#include "libapp.h"
#include "libpath.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <pwd.h>
#include <wordexp.h>

#include "print.h"

void get_apppath(CString *result)
{
    cstr_clear(result);

    int dest_len = 256;
    cstr_resize(result, dest_len);

    int ret = readlink("/proc/self/exe", cstr_data(result), dest_len);

    if (ret == -1)
    {
        cstr_clear(result);
        return;
    }

    cstr_terminate(result, ret);

    return;
}

void get_appdir(CString *result)
{
    get_apppath(result);

    char *start = cstr_data(result);
    char *p = start;
    char *sep = NULL;

    while (1)
    {
        if (*p == '/')
        {
            sep = p;
        }
        else if (*p == '\0')
        {
            if (sep)
                cstr_terminate(result, sep - start);
            else
                cstr_clear(result);

            return;
        }

        ++p;
    }
}

void get_homedir(CString *result)
{
    cstr_clear(result);
    cstr_resize(result, 100);

    const char *homedir = getenv("HOME");

    if (homedir != NULL)
        cstr_copy(result, homedir);
}

void get_username(CString *result)
{
    cstr_clear(result);

    uid_t uid = geteuid();

    struct passwd *pw = getpwuid(uid);

    if (pw)
        cstr_copy(result, pw->pw_name);

    return;
}

void get_localtime(CString *result, const char *fmt)
{
    cstr_clear(result);
    cstr_resize(result, 64);

    time_t rawtime;

    time(&rawtime);
    struct tm *info = localtime(&rawtime);

    size_t len = strftime(cstr_data(result), cstr_capacity(result), fmt, info);
    cstr_terminate(result, len);
}

bool dir_exists(const char *fileName)
{
    struct stat st;
    int result = stat(fileName, &st);

    return (result == 0 && (st.st_mode & S_IFDIR));
}

bool file_exists(const char *fileName)
{
    struct stat st;
    int result = stat(fileName, &st);

    return (result == 0 && (st.st_mode & S_IFREG));
}

bool file_remove(const char *fileName)
{
    return (remove(fileName) == 0);
}

int pexec(const char *cmd)
{
    if (!cmd || !*cmd)
        return EXIT_FAILURE;

    wordexp_t p;
    wordexp(cmd, &p, 0);
    char **w = p.we_wordv;

    pid_t childpid = fork();

    if (childpid < 0)
    {
        //perror("failed to fork.");

        wordfree(&p);

        return EXIT_FAILURE;
    }
    else if (childpid == 0)
    {
        umask(0);
        setsid();

        if (chdir("/") < 0)
            exit(EXIT_FAILURE);

        // close out the standard file descriptors.
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);

        execve(w[0], (char**) w, __environ);

        exit(EXIT_FAILURE);
    }

    wordfree(&p);

    return EXIT_SUCCESS;
}

