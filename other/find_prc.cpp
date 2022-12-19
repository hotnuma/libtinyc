#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>

// https://stackoverflow.com/questions/6898337/

pid_t find_prc(const char *name)
{
    DIR *dir;

    if (!(dir = opendir("/proc")))
    {
        perror("can't open /proc");
        return -1;
    }

    struct dirent *ent;
    char *endptr;
    char buf[512];

    while ((ent = readdir(dir)) != NULL)
    {
        // if endptr is not a null character, the directory is not
        // entirely numeric, so ignore it

        long lpid = strtol(ent->d_name, &endptr, 10);
        if (*endptr != '\0')
        {
            continue;
        }

        // try to open the cmdline file

        snprintf(buf, sizeof(buf), "/proc/%ld/cmdline", lpid);
        FILE *fp = fopen(buf, "r");

        if (fp)
        {
            if (fgets(buf, sizeof(buf), fp) != NULL)
            {
                // check the first token in the file, the program name

                char *first = strtok(buf, " ");

                char *pname = basename(first);
                if (pname)
                    first = pname;

                if (!strcmp(first, name))
                {
                    fclose(fp);
                    closedir(dir);

                    return (pid_t) lpid;
                }
            }

            fclose(fp);
        }

    }

    closedir(dir);

    return -1;
}

int main()
{
    const char *name = "xfconfd";

    pid_t pid = find_prc(name);

    if (pid == -1)
    {
        printf("%s: not found\n", name);
    }
    else
    {
        printf("%s: %d\n", name, pid);
    }

    return 0;
}


