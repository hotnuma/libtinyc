#include "csingleinstance.h"

#include <sys/stat.h>
#include <unistd.h>

static bool _socket_check_permissions(const char *filepath)
{
    struct stat socket_stat;

    if (lstat(filepath, &socket_stat) == 0
        && socket_stat.st_uid != getuid())
    {
        return false;
    }

    return true;
}

static void _socket_remove_link_full(const char *filepath)
{
    char real_path[512];
    real_path[0] = '\0';

    // readlink should return something like "/tmp/app_socket.499602d2".

    int len = readlink(filepath, real_path, sizeof(real_path) - 1);

    if (len > 0)
    {
        real_path[len] = '\0';
        unlink(real_path);
    }

    unlink(filepath);
}

struct _CSingleInstance
{
    char *file_name;
    bool isfirst;
    int sock;

    GIOChannel *iochannel;
    uint watch;
};

CSingleInstance* csingleinst_new()
{
    CSingleInstance *inst = (CSingleInstance*) malloc(sizeof(CSingleInstance));

    inst->file_name = NULL;
    inst->isfirst = false;
    inst->sock = -1;

    inst->iochannel = NULL;
    inst->watch = 0;

    return inst;
}

void csingleinst_free(CSingleInstance *inst)
{
    if (!inst)
        return;

    csingleinst_close(inst);
    free(inst);
}

/* Returns the created socket, -1 if an error occurred or -2 if another socket exists and files
 * were sent to it. */
bool csingleinst_open(CSingleInstance *inst, const char *socket_path,
                      GIOFunc func, gpointer user_data)
{
    if (inst->sock > -1)
        csingleinst_close(inst);

    // check whether the real user id is the same as this of the socket file.
    if (!_socket_check_permissions(socket_path))
        return false;

    inst->isfirst = false;

    if (inst->file_name == NULL)
        inst->file_name = strdup(socket_path);

    inst->sock = socket_fd_connect_unix(inst->file_name);

    if (inst->sock < 0)
    {
        // first instance.

        _socket_remove_link_full(inst->file_name);

        inst->sock = socket_fd_open_unix(inst->file_name);

        if (inst->sock < 0)
            return false;

        inst->isfirst = true;

        inst->iochannel = g_io_channel_unix_new(inst->sock);

        inst->watch = g_io_add_watch(
            inst->iochannel,
            (GIOCondition) (G_IO_IN | G_IO_PRI | G_IO_ERR),
            func,
            user_data);
    }

    return true;
}

int csingleinst_close(CSingleInstance *inst)
{
    if (inst->watch > 0)
    {
        g_source_remove(inst->watch);

        inst->watch = 0;
    }

    if (inst->iochannel)
    {
        g_io_channel_shutdown(inst->iochannel, false, NULL);
        g_io_channel_unref(inst->iochannel);

        inst->iochannel = NULL;
    }

    if (inst->sock > -1)
    {
        socket_fd_close(inst->sock);

        inst->sock = -1;
    }

    if (inst->file_name != NULL)
    {
        if (inst->isfirst)
            _socket_remove_link_full(inst->file_name);

        free(inst->file_name);

        inst->file_name = NULL;
    }

    return 0;
}

bool csingleinst_isfirst(CSingleInstance *inst)
{
    return inst->isfirst;
}

int csingleinst_sock(CSingleInstance *inst)
{
    return inst->sock;
}


