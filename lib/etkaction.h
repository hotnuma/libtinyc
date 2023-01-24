#ifndef LIBEMENU_H
#define LIBEMENU_H

#include <gtk/gtk.h>

typedef enum
{
    ETK_MENU_ITEM,
    ETK_IMAGE_MENU_ITEM,
    ETK_CHECK_MENU_ITEM,
    ETK_RADIO_MENU_ITEM,

} EtkMenuItem;

/**
 * Replacement for the deprecated #GtkActionEntry.
 * The idea is to provide a fixed list of #XfceGtkActionEntrys:
 * - use etk_translate_action_entries() once to translate the list
 * - use etk_accel_map_add_entries() once to register the provided accelerators
 * - use etk_get_action_entry_by_id() to find a single entry, e.g. by using a enumeration
 * - use etk_*_new_from_action_entry() to create the specific menu- or tool-items from the entry
 **/
struct _EtkActionEntry
{
    guint       id;
    const gchar *accel_path;
    const gchar *default_accelerator;

    // menu_item data is optional, only relevant
    // if there exists a menu_item for that accelerator
    EtkMenuItem menu_item_type;
    gchar       *menu_item_label_text;
    gchar       *menu_item_tooltip_text;
    const gchar *menu_item_icon_name;

    GCallback   callback;
};

typedef struct _EtkActionEntry EtkActionEntry;

void etk_menu_item_set_accel_label(GtkMenuItem *menu_item, const gchar *accel_path);

GtkWidget* etk_menu_item_new(const gchar  *label_text,
                             const gchar  *tooltip_text,
                             const gchar  *accel_path,
                             GCallback    callback,
                             GObject      *callback_param,
                             GtkMenuShell *menu_to_append_item);
GtkWidget* etk_image_menu_item_new(const gchar  *label_text,
                                   const gchar  *tooltip_text,
                                   const gchar  *accel_path,
                                   GCallback    callback,
                                   GObject      *callback_param,
                                   GtkWidget    *image,
                                   GtkMenuShell *menu_to_append_item);
GtkWidget* etk_image_menu_item_new_from_icon_name(const gchar  *label_text,
                                                  const gchar  *tooltip_text,
                                                  const gchar  *accel_path,
                                                  GCallback    callback,
                                                  GObject      *callback_param,
                                                  const gchar  *icon_name,
                                                  GtkMenuShell *menu_to_append_item);
GtkWidget* etk_check_menu_item_new(const gchar  *label_text,
                                   const gchar  *tooltip_text,
                                   const gchar  *accel_path,
                                   GCallback    callback,
                                   GObject      *callback_param,
                                   gboolean     active,
                                   GtkMenuShell *menu_to_append_item);
GtkWidget* etk_radio_menu_item_new(const gchar  *label_text,
                                   const gchar  *tooltip_text,
                                   const gchar  *accel_path,
                                   GCallback    callback,
                                   GObject      *callback_param,
                                   gboolean     active,
                                   GtkMenuShell *menu_to_append_item);
GtkWidget* etk_menu_item_new_from_action_entry(const EtkActionEntry *action_entry,
                                               GObject              *callback_param,
                                               GtkMenuShell         *menu_to_append_item);
GtkWidget* etk_toggle_menu_item_new_from_action_entry(const EtkActionEntry *action_entry,
                                                      GObject              *callback_param,
                                                      gboolean             active,
                                                      GtkMenuShell         *menu_to_append_item);
GtkWidget* etk_tool_button_new_from_action_entry(const EtkActionEntry *action_entry,
                                                 GObject              *callback_param,
                                                 GtkToolbar           *toolbar);
GtkWidget* etk_toggle_tool_button_new_from_action_entry(const EtkActionEntry *action_entry,
                                                        GObject              *callback_param,
                                                        gboolean             active,
                                                        GtkToolbar           *toolbar);
void etk_accel_map_add_entries(const EtkActionEntry *action_entries, guint n_action_entries);
void etk_accel_group_connect_action_entries(GtkAccelGroup        *accel_group,
                                            const EtkActionEntry *action_entries,
                                            guint                n_action_entries,
                                            gpointer             callback_data);
void etk_accel_group_disconnect_action_entries(GtkAccelGroup        *accel_group,
                                               const EtkActionEntry *action_entries,
                                               guint                n_action_entries);
const EtkActionEntry *etk_get_action_entry_by_id(const EtkActionEntry *action_entries,
                                                 guint                n_action_entries,
                                                 guint                id);
void etk_translate_action_entries(EtkActionEntry *action_entries, guint n_action_entries);
gboolean etk_handle_tab_accels(GdkEventKey    *key_event,
                               GtkAccelGroup  *accel_group,
                               gpointer       data,
                               EtkActionEntry *entries,
                               size_t         entry_count);

#endif // LIBEMENU_H


