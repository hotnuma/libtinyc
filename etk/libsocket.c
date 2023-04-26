/*
 *      Copyright 2006 The Geany contributors
 *      Copyright 2006 Hiroyuki Yamamoto (author of Sylpheed)
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License along
 *      with this program; if not, write to the Free Software Foundation, Inc.,
 *      51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "libsocket.h"

#include <sys/socket.h>
#include <sys/un.h>
#include <glib/gstdio.h>
#include <unistd.h>
#include <fcntl.h>

//#include <print.h>

static void _log_error(const char *message_prefix, int error_code);

static int _socket_fd_check_io(int fd, GIOCondition cond);
static int _socket_fd_read(int sock, char *buf, int len);
static int _socket_fd_recv(int fd, char *buf, int len, int flags);
static int _socket_fd_write(int sock, const char *buf, int len);

int socket_fd_open_unix(const char *filepath)
{
    int sock = socket(PF_UNIX, SOCK_STREAM, 0);
    if (sock < 0)
    {
        _log_error("Failed to create IPC socket", errno);
        return -1;
    }

    int val = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)) < 0)
    {
        _log_error("Failed to set IPC socket option", errno);
        socket_fd_close(sock);
        return -1;
    }

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));

    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, filepath, sizeof(addr.sun_path) - 1);

    if (bind(sock, (struct sockaddr*) &addr, sizeof(addr)) < 0)
    {
        int saved_errno = errno;

        char *message = g_strdup_printf("Failed to bind IPC socket (%s)", filepath);
        _log_error(message, saved_errno);
        g_free(message);

        socket_fd_close(sock);

        return -1;
    }

    if (listen(sock, 1) < 0)
    {
        int saved_errno = errno;

        char *message = g_strdup_printf("Failed to listen on IPC socket (%s)", filepath);
        _log_error(message, saved_errno);
        g_free(message);

        socket_fd_close(sock);

        return -1;
    }

    g_chmod(filepath, 0600);

    return sock;
}

int socket_fd_connect_unix(const char *filepath)
{
    int sock = socket(PF_UNIX, SOCK_STREAM, 0);

    if (sock < 0)
    {
        _log_error("Failed to create IPC socket", errno);
        return -1;
    }

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));

    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, filepath, sizeof(addr.sun_path) - 1);

    if (connect(sock, (struct sockaddr*) &addr, sizeof(addr)) < 0)
    {
        socket_fd_close(sock);
        return -1;
    }

    return sock;
}

int socket_fd_close(int sock)
{
    return close(sock);
}

static void _log_error(const char *message_prefix, int error_code)
{
    error_code = (error_code == -1 ? errno : error_code);
    char *error_message = g_strdup(g_strerror(error_code));

    g_warning("%s: %d: %s", message_prefix, error_code, error_message);

    g_free(error_message);
}




int socket_fd_gets(int sock, char *buf, int len)
{
    char *bp = buf;

    char *newline;
    int n;

    if (--len < 1)
        return -1;

    do
    {
        if ((n = _socket_fd_recv(sock, bp, len, MSG_PEEK)) <= 0)
            return -1;

        if ((newline = (char*) memchr(bp, '\n', n)) != NULL)
            n = newline - bp + 1;

        if ((n = _socket_fd_read(sock, bp, n)) < 0)
            return -1;

        bp += n;
        len -= n;

    } while (! newline && len);

    *bp = '\0';

    return bp - buf;
}

static int _socket_fd_recv(int sock, char *buf, int len, int flags)
{
    if (_socket_fd_check_io(sock, G_IO_IN) < 0)
        return -1;

    return recv(sock, buf, len, flags);
}

static int _socket_fd_read(int sock, char *buf, int len)
{
    if (_socket_fd_check_io(sock, G_IO_IN) < 0)
        return -1;

    return read(sock, buf, len);
}

static int _socket_fd_check_io(int sock, GIOCondition cond)
{
    struct timeval timeout;
    fd_set fds;
    int flags;

    timeout.tv_sec  = 60;
    timeout.tv_usec = 0;

    /* checking for non-blocking mode */
    flags = fcntl(sock, F_GETFL, 0);
    if (flags < 0)
    {
        _log_error("fcntl() failed", errno);
        return 0;
    }
    if ((flags & O_NONBLOCK) != 0)
        return 0;

    FD_ZERO(&fds);
    FD_SET(sock, &fds);

    if (cond == G_IO_IN)
    {
        select(sock + 1, &fds, NULL, NULL, &timeout);
    }
    else
    {
        select(sock + 1, NULL, &fds, NULL, &timeout);
    }

    if (FD_ISSET(sock, &fds))
    {
        return 0;
    }
    else
    {
        printf("Socket IO timeout\n");
        return -1;
    }
}

int socket_fd_write_all(int sock, const char *buf, int len)
{
    int n, wrlen = 0;

    if (len == -1)
        len = strlen(buf);

    while (len)
    {
        n = _socket_fd_write(sock, buf, len);
        if (n <= 0)
            return -1;
        len -= n;
        wrlen += n;
        buf += n;
    }

    return wrlen;
}

int _socket_fd_write(int sock, const char *buf, int len)
{
    if (_socket_fd_check_io(sock, G_IO_OUT) < 0)
        return -1;

    return write(sock, buf, len);
}


