#include "etkaction.h"


// Window init / destroy ------------------------------------------------------

GtkAccelGroup* etk_actions_init(GtkWindow *window, EtkActionEntry *actions)
{
    GtkAccelGroup *accel_group = gtk_accel_group_new();
    etk_actions_map_accels(actions);
    etk_actions_connect_accels(actions, accel_group, window);
    gtk_window_add_accel_group(window, accel_group);

    return accel_group;
}

void etk_actions_dispose(GtkWindow *window, GtkAccelGroup *accel_group)
{
    if (!accel_group)
        return;

    gtk_accel_group_disconnect(accel_group, NULL);
    gtk_window_remove_accel_group(window, accel_group);
    g_object_unref(accel_group);
}

void etk_actions_translate(EtkActionEntry *action_entries)
{
    int i = 0;
    while (action_entries[i].id != 0)
    {
        action_entries[i].label_text =
            g_strdup(g_dgettext(NULL, action_entries[i].label_text));

        action_entries[i].tooltip_text =
            g_strdup(g_dgettext(NULL, action_entries[i].tooltip_text));

        ++i;
    }
}

void etk_actions_map_accels(const EtkActionEntry *action_entries)
{
    int i = 0;
    while (action_entries[i].id != 0)
    {
        if (action_entries[i].accel_path == NULL
            || g_strcmp0(action_entries[i].accel_path, "") == 0)
        {
            ++i;
            continue;
        }

        GtkAccelKey key;

        /* If the accel path was not loaded to the acel_map via file, we add the default key for it to the accel_map */
        if (gtk_accel_map_lookup_entry(action_entries[i].accel_path, &key) == FALSE)
        {
            gtk_accelerator_parse(action_entries[i].default_accelerator,
                                  &key.accel_key, &key.accel_mods);

            gtk_accel_map_add_entry(action_entries[i].accel_path,
                                    key.accel_key, key.accel_mods);
        }

        ++i;
    }
}

void etk_actions_connect_accels(const EtkActionEntry *action_entries,
                                GtkAccelGroup        *accel_group,
                                gpointer             callback_data)
{
    g_return_if_fail(GTK_IS_ACCEL_GROUP(accel_group));

    GClosure *closure = NULL;

    int i = 0;
    while (action_entries[i].id != 0)
    {
        if (action_entries[i].accel_path == NULL
            || g_strcmp0(action_entries[i].accel_path, "") == 0)
        {
            ++i;
            continue;
        }

        if (action_entries[i].callback != NULL)
        {
            closure = g_cclosure_new_swap(action_entries[i].callback, callback_data, NULL);
            gtk_accel_group_connect_by_path(accel_group, action_entries[i].accel_path, closure);
        }

        ++i;
    }
}

void etk_actions_disconnect_accels(const EtkActionEntry *action_entries,
                                   GtkAccelGroup        *accel_group)
{
    g_return_if_fail(GTK_IS_ACCEL_GROUP(accel_group));

    int i = 0;
    while (action_entries[i].id != 0)
    {
        if (action_entries[i].accel_path == NULL
            || g_strcmp0(action_entries[i].accel_path, "") == 0)
        {
            ++i;
            continue;
        }

        if (action_entries[i].callback != NULL)
        {
            GtkAccelKey key;

            if (gtk_accel_map_lookup_entry(action_entries[i].accel_path, &key) == TRUE)
                gtk_accel_group_disconnect_key(accel_group, key.accel_key, key.accel_mods);
        }

        ++i;
    }
}

const EtkActionEntry* etk_actions_get_entry(const EtkActionEntry *action_entries,
                                                 guint                id)
{
    int i = 0;
    while (action_entries[i].id != 0)
    {
        if (action_entries[i].id == id)
            return &(action_entries[i]);

        ++i;
    }

    g_warning("There is no action with the id '%i'.", id);

    return NULL;
}

void etk_menu_item_set_accel_label(GtkMenuItem *menu_item, const gchar *accel_path)
{
    GtkAccelKey  key;
    GList       *list, *lp;
    gboolean     found = FALSE;

    g_return_if_fail(GTK_IS_MENU_ITEM(menu_item));

    list = gtk_container_get_children(GTK_CONTAINER(menu_item));

    if (accel_path != NULL)
        found = gtk_accel_map_lookup_entry(accel_path, &key);

    /* Only show the relevant accelerator, do not automatically connect to the callback */
    for (lp = list; lp != NULL; lp = lp->next)
    {
        if (GTK_IS_ACCEL_LABEL(lp->data))
        {
            if (found)
                gtk_accel_label_set_accel(lp->data, key.accel_key, key.accel_mods);
            else
                gtk_accel_label_set_accel(lp->data, 0, 0);
        }
    }

    g_list_free(list);
}

static void _etk_menu_fill_item(GtkMenuShell *menu,
                                GtkWidget    *item,
                                const gchar  *tooltip_text,
                                const gchar  *accel_path,
                                GCallback    callback,
                                GObject      *callback_param)
{
    g_return_if_fail(GTK_IS_MENU_ITEM(item));

    if (tooltip_text != NULL)
        gtk_widget_set_tooltip_text(item, tooltip_text);

    /* Explicitly dont use 'gtk_menu_item_set_accel_path'
     * in order to give more control over accelerator management for non-permanent menu items */
    etk_menu_item_set_accel_label(GTK_MENU_ITEM(item), accel_path);

    if (callback != NULL)
        g_signal_connect_swapped(G_OBJECT(item), "activate", callback, callback_param);

    if (menu != NULL)
        gtk_menu_shell_append(menu, item);
}

GtkWidget* etk_menu_item_new(GtkMenuShell *menu,
                             const gchar  *label_text,
                             const gchar  *tooltip_text,
                             const gchar  *accel_path,
                             GCallback    callback,
                             GObject      *callback_param)
{
    GtkWidget *item = gtk_menu_item_new_with_mnemonic(label_text);

    _etk_menu_fill_item(menu,
                        item,
                        tooltip_text,
                        accel_path,
                        callback,
                        callback_param);

    return item;
}

GtkWidget* etk_image_menu_item_new_from_icon_name(GtkMenuShell *menu,
                                                  const gchar  *label_text,
                                                  const gchar  *tooltip_text,
                                                  const gchar  *icon_name,
                                                  const gchar  *accel_path,
                                                  GCallback    callback,
                                                  GObject      *callback_param)
{
    GtkWidget *image = gtk_image_new_from_icon_name(icon_name, GTK_ICON_SIZE_MENU);

    return etk_image_menu_item_new(menu,
                                   label_text,
                                   tooltip_text,
                                   image,
                                   accel_path,
                                   callback,
                                   callback_param);
}

GtkWidget* etk_image_menu_item_new(GtkMenuShell *menu,
                                   const gchar  *label_text,
                                   const gchar  *tooltip_text,
                                   GtkWidget    *image,
                                   const gchar  *accel_path,
                                   GCallback    callback,
                                   GObject      *callback_param)
{
    G_GNUC_BEGIN_IGNORE_DEPRECATIONS
    GtkWidget *item = gtk_image_menu_item_new_with_mnemonic(label_text);
    G_GNUC_END_IGNORE_DEPRECATIONS

    _etk_menu_fill_item(menu,
                        item,
                        tooltip_text,
                        accel_path,
                        callback,
                        callback_param);

    if (image != NULL)
    {
        G_GNUC_BEGIN_IGNORE_DEPRECATIONS
        gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(item), image);
        G_GNUC_END_IGNORE_DEPRECATIONS
    }

    return item;
}

GtkWidget* etk_check_menu_item_new(GtkMenuShell *menu,
                                   const gchar  *label_text,
                                   const gchar  *tooltip_text,
                                   gboolean     active,
                                   const gchar  *accel_path,
                                   GCallback    callback,
                                   GObject      *callback_param)
{
    GtkWidget *item = gtk_check_menu_item_new_with_mnemonic(label_text);

    _etk_menu_fill_item(menu,
                        item,
                        tooltip_text,
                        accel_path,
                        NULL,
                        NULL);

    /* 'gtk_check_menu_item_set_active' has to be done before 'g_signal_connect_swapped', to don't trigger the callback */
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(item), active);

    if (callback != NULL)
        g_signal_connect_swapped(G_OBJECT(item), "toggled", callback, callback_param);

    return item;
}

GtkWidget* etk_radio_menu_item_new(GtkMenuShell *menu,
                                   const gchar  *label_text,
                                   const gchar  *tooltip_text,
                                   gboolean     active,
                                   const gchar  *accel_path,
                                   GCallback    callback,
                                   GObject      *callback_param)
{
    /* It's simpler to just use a gtk_check_menu_item and display it with a radio button */
    GtkWidget *item = etk_check_menu_item_new(menu,
                                              label_text,
                                              tooltip_text,
                                              active,
                                              accel_path,
                                              callback,
                                              callback_param);

    gtk_check_menu_item_set_draw_as_radio(GTK_CHECK_MENU_ITEM(item), TRUE);

    return item;
}

GtkWidget* etk_menu_item_new_from_action(GtkMenuShell         *menu,
                                         int                  id,
                                         const EtkActionEntry *action_entries,
                                         GObject              *callback_param)
{
    const EtkActionEntry *action_entry = etk_actions_get_entry(action_entries, id);

    g_return_val_if_fail(action_entry != NULL, NULL);

    if (action_entry->item_type == ETK_IMAGE_MENU_ITEM)
    {
        return etk_image_menu_item_new_from_icon_name(menu,
                                                      action_entry->label_text,
                                                      action_entry->tooltip_text,
                                                      action_entry->icon_name,
                                                      action_entry->accel_path,
                                                      action_entry->callback,
                                                      callback_param);
    }

    if (action_entry->item_type == ETK_MENU_ITEM)
    {
        return etk_menu_item_new(menu,
                                 action_entry->label_text,
                                 action_entry->tooltip_text,
                                 action_entry->accel_path,
                                 action_entry->callback,
                                 callback_param);
    }

    g_warning("etk_menu_item_new_from_action_entry: Unknown item_type");

    return NULL;
}

GtkWidget* etk_toggle_menu_item_new_from_action(GtkMenuShell         *menu,
                                                int                  id,
                                                const EtkActionEntry *action_entries,
                                                gboolean             active,
                                                GObject              *callback_param)
{
    const EtkActionEntry *action_entry = etk_actions_get_entry(action_entries, id);

    g_return_val_if_fail(action_entry != NULL, NULL);

    if (action_entry->item_type == ETK_CHECK_MENU_ITEM)
    {
        return etk_check_menu_item_new(menu,
                                       action_entry->label_text,
                                       action_entry->tooltip_text,
                                       active,
                                       action_entry->accel_path,
                                       action_entry->callback,
                                       callback_param);
    }

    if (action_entry->item_type == ETK_RADIO_MENU_ITEM)
    {
        return etk_radio_menu_item_new(menu,
                                       action_entry->label_text,
                                       action_entry->tooltip_text,
                                       active,
                                       action_entry->accel_path,
                                       action_entry->callback,
                                       callback_param);
    }

    g_warning("etk_toggle_menu_item_new_from_action_entry: Unknown item_type");

    return NULL;
}

void etk_menu_append_separator(GtkMenuShell *menu)
{
    g_return_if_fail(GTK_IS_MENU_SHELL(menu));

    gtk_menu_shell_append(menu, gtk_separator_menu_item_new());
}



GtkWidget* etk_tool_button_new_from_action(GtkToolbar           *toolbar,
                                           int                  id,
                                           const EtkActionEntry *action_entries,
                                           GObject              *callback_param)
{
    const EtkActionEntry *action_entry = etk_actions_get_entry(action_entries, id);

    g_return_val_if_fail(action_entry != NULL, NULL);

    GtkWidget *image = gtk_image_new_from_icon_name(action_entry->icon_name,
                                                    GTK_ICON_SIZE_LARGE_TOOLBAR);

    GtkToolItem *tool_item = gtk_tool_button_new(image, action_entry->label_text);
    g_signal_connect_swapped(G_OBJECT(tool_item), "clicked", action_entry->callback, callback_param);
    gtk_widget_set_tooltip_text(GTK_WIDGET(tool_item), action_entry->tooltip_text);
    gtk_toolbar_insert(toolbar, tool_item, -1);

    return GTK_WIDGET(tool_item);
}

GtkWidget* etk_toggle_tool_button_new_from_action(GtkToolbar           *toolbar,
                                                  int                  id,
                                                  const EtkActionEntry *action_entries,
                                                  gboolean             active,
                                                  GObject              *callback_param)
{
    const EtkActionEntry *action_entry = etk_actions_get_entry(action_entries, id);

    g_return_val_if_fail(action_entry != NULL, NULL);

    GtkToolButton *tool_item = GTK_TOOL_BUTTON(gtk_toggle_tool_button_new());
    GtkWidget *image = gtk_image_new_from_icon_name(action_entry->icon_name,
                                                    GTK_ICON_SIZE_LARGE_TOOLBAR);

    gtk_tool_button_set_label(tool_item, action_entry->label_text);
    gtk_tool_button_set_icon_widget(tool_item, image);
    gtk_widget_set_tooltip_text(GTK_WIDGET(tool_item), action_entry->tooltip_text);
    gtk_toolbar_insert(toolbar, GTK_TOOL_ITEM(tool_item), -1);

    /* 'gtk_check_menu_item_set_active' has to be done before 'g_signal_connect_swapped', to don't trigger the callback */
    gtk_toggle_tool_button_set_active(GTK_TOGGLE_TOOL_BUTTON(tool_item), active);
    g_signal_connect_swapped(G_OBJECT(tool_item), "toggled", action_entry->callback, callback_param);

    return GTK_WIDGET(tool_item);
}


