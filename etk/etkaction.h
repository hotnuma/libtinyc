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
    EtkMenuItem item_type;
    gchar       *label_text;
    gchar       *tooltip_text;
    const gchar *icon_name;

    GCallback   callback;
};

typedef struct _EtkActionEntry EtkActionEntry;

void etk_actions_translate(EtkActionEntry *action_entries);
void etk_actions_map_accels(const EtkActionEntry *action_entries);
void etk_actions_connect_accels(const EtkActionEntry *action_entries,
                                GtkAccelGroup        *accel_group,
                                gpointer             callback_data);
void etk_actions_disconnect_accels(const EtkActionEntry *action_entries,
                                   GtkAccelGroup        *accel_group);


const EtkActionEntry* etk_actions_get_entry(const EtkActionEntry *action_entries,
                                            guint                id);

void etk_menu_item_set_accel_label(GtkMenuItem *menu_item, const gchar *accel_path);

GtkWidget* etk_menu_item_new(GtkMenuShell *menu,
                             const gchar  *label_text,
                             const gchar  *tooltip_text,
                             const gchar  *accel_path,
                             GCallback    callback,
                             GObject      *callback_param);

GtkWidget* etk_image_menu_item_new(GtkMenuShell *menu,
                                   const gchar  *label_text,
                                   const gchar  *tooltip_text,
                                   GtkWidget    *image,
                                   const gchar  *accel_path,
                                   GCallback    callback,
                                   GObject      *callback_param);

GtkWidget* etk_image_menu_item_new_from_icon_name(GtkMenuShell *menu,
                                                  const gchar  *label_text,
                                                  const gchar  *tooltip_text,
                                                  const gchar  *icon_name,
                                                  const gchar  *accel_path,
                                                  GCallback    callback,
                                                  GObject      *callback_param);

GtkWidget* etk_check_menu_item_new(GtkMenuShell *menu,
                                   const gchar  *label_text,
                                   const gchar  *tooltip_text,
                                   gboolean     active,
                                   const gchar  *accel_path,
                                   GCallback    callback,
                                   GObject      *callback_param);

GtkWidget* etk_radio_menu_item_new(GtkMenuShell *menu,
                                   const gchar  *label_text,
                                   const gchar  *tooltip_text,
                                   gboolean     active,
                                   const gchar  *accel_path,
                                   GCallback    callback,
                                   GObject      *callback_param);

GtkWidget* etk_menu_item_new_from_action(GtkMenuShell         *menu,
                                         int                  id,
                                         const EtkActionEntry *action_entries,
                                         GObject              *callback_param);

GtkWidget* etk_toggle_menu_item_new_from_action(GtkMenuShell         *menu,
                                                int                  id,
                                                const EtkActionEntry *action_entries,
                                                gboolean             active,
                                                GObject              *callback_param);

GtkWidget* etk_tool_button_new_from_action(GtkToolbar           *toolbar,
                                           int                  id,
                                           const EtkActionEntry *action_entries,
                                           GObject              *callback_param);

GtkWidget* etk_toggle_tool_button_new_from_action(GtkToolbar           *toolbar,
                                                  int                  id,
                                                  const EtkActionEntry *action_entries,
                                                  gboolean             active,
                                                  GObject              *callback_param);

#endif // LIBEMENU_H


