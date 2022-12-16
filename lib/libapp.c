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

#if 0
CString* getApplicationPath()
{
    int dest_len = 256;

    CString result(dest_len);

    int ret = readlink("/proc/self/exe", result.data(), dest_len);

    if (ret == -1)
    {
        result.clear();
        return result;
    }

    result.terminate(ret);

    return result;
}

CString* getApplicationDir()
{
    CString path = getApplicationPath();

    char *p = path.data();
    char *sep = nullptr;

    while (1)
    {
        if (*p == '/')
        {
            sep = p;
        }
        else if (*p == '\0')
        {
            if (sep)
                path.terminate(sep - path.data());
            else
                path.clear();

            return path;
        }

        ++p;
    }
}

CString* getHomeDirectory()
{
    CString result(100);

    const char *homedir = getenv("HOME");

    if (homedir != nullptr)
        result = homedir;

    return result;
}

CString* getUserName()
{
    CString result;

    uid_t uid = geteuid();

    struct passwd *pw = getpwuid(uid);

    if (pw)
    {
        result = pw->pw_name;
    }

    return result;
}

CString* getCurrentTime(const char *fmt)
{
    time_t rawtime;
    struct tm *info;
    CString result(80);

    time(&rawtime);

    info = localtime(&rawtime);

    strftime(result.data(), result.capacity(), fmt, info);
    result.terminate();

    return result;
}
#endif

bool dirExists(const char *fileName)
{
    struct stat st;
    int result = stat(fileName, &st);

    return (result == 0 && (st.st_mode & S_IFDIR));
}

bool fileExists(const char *fileName)
{
    struct stat st;
    int result = stat(fileName, &st);

    return (result == 0 && (st.st_mode & S_IFREG));
}

bool fileRemove(const char *fileName)
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


