#include <glib.h>
#include <glib/gprintf.h>

int main()
{
    GKeyFile *key_file = g_key_file_new();

    if (!g_key_file_load_from_file(key_file,
                                   "/home/hotnuma/DevFiles/testcmd/samples/ini_parse.ini",
                                   (GKeyFileFlags) (G_KEY_FILE_KEEP_COMMENTS
                                   | G_KEY_FILE_KEEP_TRANSLATIONS),
                                   NULL))
    {
        exit(1);
    }

    gsize size;
    gchar **groups = g_key_file_get_groups(key_file, &size);
    for (guint group = 0; group < size; group++)
    {
        g_print("%s\n", groups[group]);

        gsize num_keys;
        gchar **keys = g_key_file_get_keys(key_file, groups[group], &num_keys, NULL);

        for (guint key = 0; key < num_keys; ++key)
        {
            gchar *value = g_key_file_get_value(key_file,
                                                groups[group],
                                                keys[key],
                                                NULL);
            g_print("%s => %s\n", keys[key], value);
        }
    }

    g_key_file_free(key_file);

    return 0;
}


