#include <dbus/dbus.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void query()
{
    // initialiset the errors
    DBusError err;
    dbus_error_init(&err);

    // connect to the system bus and check for errors
    DBusConnection *conn = dbus_bus_get(DBUS_BUS_SESSION, &err);

    if (dbus_error_is_set(&err))
    {
        fprintf(stderr, "Connection Error (%s)\n", err.message);
        dbus_error_free(&err);
    }

    if (NULL == conn)
    {
        exit(1);
    }

    // request our name on the bus
    int ret = dbus_bus_request_name(conn,
                                    "test.method.caller",
                                    DBUS_NAME_FLAG_REPLACE_EXISTING,
                                    &err);

    if (dbus_error_is_set(&err))
    {
        fprintf(stderr, "Name Error (%s)\n", err.message);
        dbus_error_free(&err);
    }

    if (DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER != ret)
    {
        exit(1);
    }

    // create a new method call and check for errors
    DBusMessage *msg = dbus_message_new_method_call("org.freedesktop.Notifications",
                       "/org/freedesktop/Notifications",
                       "org.freedesktop.Notifications",
                       "GetServerInformation");
    if (NULL == msg)
    {
        fprintf(stderr, "Message Null\n");
        exit(1);
    }

    // send message and get a handle for a reply
    DBusPendingCall *pending;
    if (!dbus_connection_send_with_reply (conn, msg, &pending, -1))
    {
        fprintf(stderr, "Out Of Memory!\n");
        exit(1);
    }

    if (NULL == pending)
    {
        fprintf(stderr, "Pending Call Null\n");
        exit(1);
    }

    dbus_connection_flush(conn);

    printf("Request Sent\n");

    // free message
    dbus_message_unref(msg);

    // block until we recieve a reply
    dbus_pending_call_block(pending);

    // get the reply message
    msg = dbus_pending_call_steal_reply(pending);
    if (NULL == msg)
    {
        fprintf(stderr, "Reply Null\n");
        exit(1);
    }

    // free the pending message handle
    dbus_pending_call_unref(pending);

    const char *name = nullptr;
    const char *vendor = nullptr;
    const char *version = nullptr;
    const char *spec_version = nullptr;

    // read the parameters

    DBusMessageIter args;

    if (!dbus_message_iter_init(msg, &args))
        fprintf(stderr, "Message has no arguments!\n");
    else if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args))
        fprintf(stderr, "Argument is not int!\n");
    else
        dbus_message_iter_get_basic(&args, &name);

    if (!dbus_message_iter_next(&args))
        fprintf(stderr, "Message has too few arguments!\n");
    else if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args))
        fprintf(stderr, "Argument is not int!\n");
    else
        dbus_message_iter_get_basic(&args, &vendor);

    if (!dbus_message_iter_next(&args))
        fprintf(stderr, "Message has too few arguments!\n");
    else if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args))
        fprintf(stderr, "Argument is not int!\n");
    else
        dbus_message_iter_get_basic(&args, &version);

    if (!dbus_message_iter_next(&args))
        fprintf(stderr, "Message has too few arguments!\n");
    else if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args))
        fprintf(stderr, "Argument is not int!\n");
    else
        dbus_message_iter_get_basic(&args, &spec_version);

    printf("%s\n%s\n%s\n%s\n", name, vendor, version, spec_version);

    // free reply
    dbus_message_unref(msg);
}

int main()
{
    query();

    return 0;
}


