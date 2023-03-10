#include "etkaction.h"

static void _etk_menu_item_fill_base(GtkWidget    *item,
                                     const gchar  *tooltip_text,
                                     const gchar  *accel_path,
                                     GCallback    callback,
                                     GObject      *callback_param,
                                     GtkMenuShell *menu_to_append_item)
{
    g_return_if_fail(GTK_IS_MENU_ITEM(item));

    if (tooltip_text != NULL)
        gtk_widget_set_tooltip_text(item, tooltip_text);

    /* Explicitly dont use 'gtk_menu_item_set_accel_path'
     * in order to give more control over accelerator management for non-permanent menu items */
    etk_menu_item_set_accel_label(GTK_MENU_ITEM(item), accel_path);

    if (callback != NULL)
        g_signal_connect_swapped(G_OBJECT(item), "activate", callback, callback_param);

    if (menu_to_append_item != NULL)
        gtk_menu_shell_append(menu_to_append_item, item);
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

GtkWidget* etk_menu_item_new(const gchar  *label_text,
                             const gchar  *tooltip_text,
                             const gchar  *accel_path,
                             GCallback    callback,
                             GObject      *callback_param,
                             GtkMenuShell *menu_to_append_item)
{
    GtkWidget *item = gtk_menu_item_new_with_mnemonic(label_text);

    _etk_menu_item_fill_base(item, tooltip_text, accel_path,
                             callback, callback_param, menu_to_append_item);

    return item;
}

GtkWidget* etk_image_menu_item_new_from_icon_name(const gchar  *label_text,
                                                  const gchar  *tooltip_text,
                                                  const gchar  *accel_path,
                                                  GCallback    callback,
                                                  GObject      *callback_param,
                                                  const gchar  *icon_name,
                                                  GtkMenuShell *menu_to_append_item)
{
    GtkWidget *image = gtk_image_new_from_icon_name(icon_name, GTK_ICON_SIZE_MENU);

    return etk_image_menu_item_new(label_text, tooltip_text, accel_path,
                                   callback, callback_param, image, menu_to_append_item);
}

GtkWidget* etk_image_menu_item_new(const gchar  *label_text,
                                   const gchar  *tooltip_text,
                                   const gchar  *accel_path,
                                   GCallback    callback,
                                   GObject      *callback_param,
                                   GtkWidget    *image,
                                   GtkMenuShell *menu_to_append_item)
{
    G_GNUC_BEGIN_IGNORE_DEPRECATIONS
    GtkWidget *item = gtk_image_menu_item_new_with_mnemonic(label_text);
    G_GNUC_END_IGNORE_DEPRECATIONS

    _etk_menu_item_fill_base(item, tooltip_text, accel_path,
                             callback, callback_param, menu_to_append_item);

    if (image != NULL)
    {
        G_GNUC_BEGIN_IGNORE_DEPRECATIONS
        gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(item), image);
        G_GNUC_END_IGNORE_DEPRECATIONS
    }

    return item;
}

GtkWidget* etk_check_menu_item_new(const gchar  *label_text,
                                   const gchar  *tooltip_text,
                                   const gchar  *accel_path,
                                   GCallback    callback,
                                   GObject      *callback_param,
                                   gboolean     active,
                                   GtkMenuShell *menu_to_append_item)
{
    GtkWidget *item = gtk_check_menu_item_new_with_mnemonic(label_text);

    _etk_menu_item_fill_base(item, tooltip_text, accel_path, NULL, NULL, menu_to_append_item);

    /* 'gtk_check_menu_item_set_active' has to be done before 'g_signal_connect_swapped', to don't trigger the callback */
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(item), active);

    if (callback != NULL)
        g_signal_connect_swapped(G_OBJECT(item), "toggled", callback, callback_param);

    return item;
}

GtkWidget* etk_radio_menu_item_new(const gchar  *label_text,
                                   const gchar  *tooltip_text,
                                   const gchar  *accel_path,
                                   GCallback    callback,
                                   GObject      *callback_param,
                                   gboolean     active,
                                   GtkMenuShell *menu_to_append_item)
{
    /* It's simpler to just use a gtk_check_menu_item and display it with a radio button */
    GtkWidget *item = etk_check_menu_item_new(label_text, tooltip_text, accel_path,
                                              callback, callback_param, active,
                                              menu_to_append_item);

    gtk_check_menu_item_set_draw_as_radio(GTK_CHECK_MENU_ITEM(item), TRUE);

    return item;
}

GtkWidget* etk_menu_item_new_from_action_entry(const EtkActionEntry *action_entry,
                                               GObject              *callback_param,
                                               GtkMenuShell         *menu_to_append_item)
{
    g_return_val_if_fail(action_entry != NULL, NULL);

    if (action_entry->menu_item_type == ETK_IMAGE_MENU_ITEM)
    {
        return etk_image_menu_item_new_from_icon_name(action_entry->menu_item_label_text,
                                                      action_entry->menu_item_tooltip_text,
                                                      action_entry->accel_path,
                                                      action_entry->callback,
                                                      callback_param,
                                                      action_entry->menu_item_icon_name,
                                                      menu_to_append_item);
    }

    if (action_entry->menu_item_type == ETK_MENU_ITEM)
    {
        return etk_menu_item_new(action_entry->menu_item_label_text,
                                 action_entry->menu_item_tooltip_text,
                                 action_entry->accel_path,
                                 action_entry->callback,
                                 callback_param,
                                 menu_to_append_item);
    }

    g_warning("etk_menu_item_new_from_action_entry: Unknown item_type");

    return NULL;
}

GtkWidget* etk_toggle_menu_item_new_from_action_entry(const EtkActionEntry *action_entry,
                                                      GObject              *callback_param,
                                                      gboolean             active,
                                                      GtkMenuShell         *menu_to_append_item)
{
    g_return_val_if_fail(action_entry != NULL, NULL);

    if (action_entry->menu_item_type == ETK_CHECK_MENU_ITEM)
    {
        return etk_check_menu_item_new(action_entry->menu_item_label_text,
                                       action_entry->menu_item_tooltip_text,
                                       action_entry->accel_path,
                                       action_entry->callback,
                                       callback_param,
                                       active,
                                       menu_to_append_item);
    }

    if (action_entry->menu_item_type == ETK_RADIO_MENU_ITEM)
    {
        return etk_radio_menu_item_new(action_entry->menu_item_label_text,
                                       action_entry->menu_item_tooltip_text,
                                       action_entry->accel_path,
                                       action_entry->callback,
                                       callback_param,
                                       active,
                                       menu_to_append_item);
    }

    g_warning("etk_toggle_menu_item_new_from_action_entry: Unknown item_type");

    return NULL;
}

GtkWidget* etk_tool_button_new_from_action_entry(const EtkActionEntry *action_entry,
                                                 GObject              *callback_param,
                                                 GtkToolbar           *toolbar)
{
    g_return_val_if_fail(action_entry != NULL, NULL);

    GtkWidget *image = gtk_image_new_from_icon_name(action_entry->menu_item_icon_name,
                                                    GTK_ICON_SIZE_LARGE_TOOLBAR);

    GtkToolItem *tool_item = gtk_tool_button_new(image, action_entry->menu_item_label_text);
    g_signal_connect_swapped(G_OBJECT(tool_item), "clicked", action_entry->callback, callback_param);
    gtk_widget_set_tooltip_text(GTK_WIDGET(tool_item), action_entry->menu_item_tooltip_text);
    gtk_toolbar_insert(toolbar, tool_item, -1);

    return GTK_WIDGET(tool_item);
}

GtkWidget* etk_toggle_tool_button_new_from_action_entry(const EtkActionEntry *action_entry,
                                                        GObject              *callback_param,
                                                        gboolean             active,
                                                        GtkToolbar           *toolbar)
{
    g_return_val_if_fail(action_entry != NULL, NULL);

    GtkToolButton *tool_item = GTK_TOOL_BUTTON(gtk_toggle_tool_button_new());
    GtkWidget *image = gtk_image_new_from_icon_name(action_entry->menu_item_icon_name,
                                                    GTK_ICON_SIZE_LARGE_TOOLBAR);

    gtk_tool_button_set_label(tool_item, action_entry->menu_item_label_text);
    gtk_tool_button_set_icon_widget(tool_item, image);
    gtk_widget_set_tooltip_text(GTK_WIDGET(tool_item), action_entry->menu_item_tooltip_text);
    gtk_toolbar_insert(toolbar, GTK_TOOL_ITEM(tool_item), -1);

    /* 'gtk_check_menu_item_set_active' has to be done before 'g_signal_connect_swapped', to don't trigger the callback */
    gtk_toggle_tool_button_set_active(GTK_TOGGLE_TOOL_BUTTON(tool_item), active);
    g_signal_connect_swapped(G_OBJECT(tool_item), "toggled", action_entry->callback, callback_param);

    return GTK_WIDGET(tool_item);
}

void etk_menu_append_separator(GtkMenuShell *menu)
{
    g_return_if_fail(GTK_IS_MENU_SHELL(menu));

    gtk_menu_shell_append(menu, gtk_separator_menu_item_new());
}

void etk_accel_map_add_entries(const EtkActionEntry *action_entries,
                               guint                n_action_entries)
{
    for (size_t i = 0; i < n_action_entries; ++i)
    {
        if (action_entries[i].accel_path == NULL || g_strcmp0(action_entries[i].accel_path, "") == 0)
            continue;

        GtkAccelKey key;

        /* If the accel path was not loaded to the acel_map via file, we add the default key for it to the accel_map */
        if (gtk_accel_map_lookup_entry(action_entries[i].accel_path, &key) == FALSE)
        {
            gtk_accelerator_parse(action_entries[i].default_accelerator,
                                  &key.accel_key, &key.accel_mods);

            gtk_accel_map_add_entry(action_entries[i].accel_path,
                                    key.accel_key, key.accel_mods);
        }
    }
}

void etk_accel_group_connect_action_entries(GtkAccelGroup        *accel_group,
                                            const EtkActionEntry *action_entries,
                                            guint                n_action_entries,
                                            gpointer             callback_data)
{
    g_return_if_fail(GTK_IS_ACCEL_GROUP(accel_group));

    GClosure *closure = NULL;

    for (size_t i = 0; i < n_action_entries; i++)
    {
        if (action_entries[i].accel_path == NULL || g_strcmp0(action_entries[i].accel_path, "") == 0)
            continue;

        if (action_entries[i].callback != NULL)
        {
            closure = g_cclosure_new_swap(action_entries[i].callback, callback_data, NULL);
            gtk_accel_group_connect_by_path(accel_group, action_entries[i].accel_path, closure);
        }
    }
}

void etk_accel_group_disconnect_action_entries(GtkAccelGroup        *accel_group,
                                               const EtkActionEntry *action_entries,
                                               guint                n_action_entries)
{
    g_return_if_fail(GTK_IS_ACCEL_GROUP(accel_group));

    for (size_t i = 0; i < n_action_entries; ++i)
    {
        if (action_entries[i].accel_path == NULL || g_strcmp0(action_entries[i].accel_path, "") == 0)
            continue;

        if (action_entries[i].callback != NULL)
        {
            GtkAccelKey key;

            if (gtk_accel_map_lookup_entry(action_entries[i].accel_path, &key) == TRUE)
                gtk_accel_group_disconnect_key(accel_group, key.accel_key, key.accel_mods);
        }
    }
}

const EtkActionEntry* etk_get_action_entry_by_id(const EtkActionEntry *action_entries,
                                                 guint                n_action_entries,
                                                 guint                id)
{
    for (size_t i = 0; i <  n_action_entries; ++i)
    {
        if (action_entries[i].id == id)
            return &(action_entries[i]);
    }

    g_warning("There is no action with the id '%i'.", id);

    return NULL;
}

void etk_translate_action_entries(EtkActionEntry *action_entries,
                                  guint          n_action_entries)
{
    for (size_t i = 0; i <  n_action_entries; ++i)
    {
        action_entries[i].menu_item_label_text =
            g_strdup(g_dgettext(NULL, action_entries[i].menu_item_label_text));

        action_entries[i].menu_item_tooltip_text =
            g_strdup(g_dgettext(NULL, action_entries[i].menu_item_tooltip_text));
    }
}

