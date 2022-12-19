/*
 * Example low-level D-Bus code.
 * Written by Matthew Johnson <dbus@matthew.ath.cx>
 *
 * This code has been released into the Public Domain.
 * You may do whatever you like with it.
 *
 * Subsequent tweaks by Will Ware <wware@alum.mit.edu>
 * Still in the public domain.
 */
#include <dbus/dbus.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void reply_to_method_call(DBusMessage* msg, DBusConnection* conn);

// Server that exposes a method call and waits for it to be called
void listen()
{

   printf("Listening for method calls\n");

   // initialise the error
   DBusError err;
   dbus_error_init(&err);

   // connect to the bus and check for errors
   DBusConnection* conn;
   conn = dbus_bus_get(DBUS_BUS_SESSION, &err);
   if (dbus_error_is_set(&err))
   {
      fprintf(stderr, "Connection Error (%s)\n", err.message);
      dbus_error_free(&err);
   }
   if (NULL == conn)
   {
      fprintf(stderr, "Connection Null\n");
      exit(1);
   }

   // request our name on the bus and check for errors
   int ret;
   ret = dbus_bus_request_name(conn,
                               "test.method.server",
                               DBUS_NAME_FLAG_REPLACE_EXISTING,
                               &err);
   if (dbus_error_is_set(&err))
   {
      fprintf(stderr, "Name Error (%s)\n", err.message);
      dbus_error_free(&err);
   }

   if (DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER != ret)
   {
      fprintf(stderr, "Not Primary Owner (%d)\n", ret);
      exit(1);
   }

   // loop, testing for new messages
   DBusMessage* msg;

   while (true)
   {
      // non blocking read of the next available message
      dbus_connection_read_write(conn, 0);
      msg = dbus_connection_pop_message(conn);

      // loop again if we haven't got a message
      if (NULL == msg)
      {
         usleep(10000);
         continue;
      }

      // check this is a method call for the right interface & method
      if (dbus_message_is_method_call(msg, "test.method.Type", "Method"))
         reply_to_method_call(msg, conn);

      // free the message
      dbus_message_unref(msg);
   }

}

void reply_to_method_call(DBusMessage* msg, DBusConnection* conn)
{
   const char* param = "";

   // read the arguments
   DBusMessageIter args;
   if (!dbus_message_iter_init(msg, &args))
      fprintf(stderr, "Message has no arguments!\n");
   else if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args))
      fprintf(stderr, "Argument is not string!\n");
   else
      dbus_message_iter_get_basic(&args, &param);

   printf("Method called with %s\n", param);

   // create a reply from the message
   DBusMessage* reply;
   reply = dbus_message_new_method_return(msg);

   // add the arguments to the reply
   dbus_message_iter_init_append(reply, &args);

   dbus_uint32_t level = 21614;
   if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_UINT32, &level))
   {
      fprintf(stderr, "Out Of Memory!\n");
      exit(1);
   }

   // send the reply && flush the connection
   dbus_uint32_t serial = 0;
   if (!dbus_connection_send(conn, reply, &serial))
   {
      fprintf(stderr, "Out Of Memory!\n");
      exit(1);
   }

   dbus_connection_flush(conn);

   // free the reply
   dbus_message_unref(reply);
}

void query(const char* param)
{
   printf("Calling remote method with %s\n", param);

   // initialiset the errors
   DBusError err;
   dbus_error_init(&err);

   // connect to the system bus and check for errors
   DBusConnection* conn;
   conn = dbus_bus_get(DBUS_BUS_SESSION, &err);

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
   int ret;
   ret = dbus_bus_request_name(conn,
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
   DBusMessage* msg;
   msg = dbus_message_new_method_call("test.method.server", // target for the method call
                                      "/test/method/Object", // object to call on
                                      "test.method.Type", // interface to call on
                                      "Method"); // method name
   if (NULL == msg)
   {
      fprintf(stderr, "Message Null\n");
      exit(1);
   }

   // append arguments
   DBusMessageIter args;
   dbus_message_iter_init_append(msg, &args);
   if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &param))
   {
      fprintf(stderr, "Out Of Memory!\n");
      exit(1);
   }

   // send message and get a handle for a reply
   DBusPendingCall* pending;
   if (!dbus_connection_send_with_reply (conn, msg, &pending, -1))
   { // -1 is default timeout
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



   // read the parameters
   dbus_uint32_t level = 0;

   if (!dbus_message_iter_init(msg, &args))
      fprintf(stderr, "Message has no arguments!\n");
   else if (DBUS_TYPE_UINT32 != dbus_message_iter_get_arg_type(&args))
      fprintf(stderr, "Argument is not int!\n");
   else
      dbus_message_iter_get_basic(&args, &level);

    //if (!dbus_message_iter_next(&args))
    //  fprintf(stderr, "Message has too few arguments!\n");
    //else if (DBUS_TYPE_UINT32 != dbus_message_iter_get_arg_type(&args))
    //  fprintf(stderr, "Argument is not int!\n");
    //else
    //  dbus_message_iter_get_basic(&args, &level);

   printf("Got Reply: %d\n", level);

   // free reply
   dbus_message_unref(msg);
}

int main(int argc, char** argv)
{
   if (2 > argc)
   {
      printf ("Syntax: dbus-example [send|receive|listen|query] [<param>]\n");
      return 1;
   }

   const char* param = "no param";

   if (3 >= argc && NULL != argv[2])
       param = argv[2];

   if (0 == strcmp(argv[1], "listen"))
   {
      listen();
   }
   else if (0 == strcmp(argv[1], "query"))
   {
      query(param);
   }
   else
   {
      printf ("Syntax: dbus-example [send|receive|listen|query] [<param>]\n");
      return 1;
   }

   return 0;
}


