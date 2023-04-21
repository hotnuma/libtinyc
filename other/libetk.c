/*
 * Copyright(c) 2007 The Xfce Development Team
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or(at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301 USA
 */

#include <libetk.h>

#include <gtk/gtk.h>
#include <string.h>
#include <gdk/gdkx.h>
#include <pango/pango.h>

/* Xfce frame padding */
#define PADDING (6)

/**
 * etk_handle_tab_accels
 * @key_event   : the #GdkEventKey that might trigger a shortcut
 * @accel_group : the #GtkAccelGroup that will be get queried
 * @data        : a pointer of data that will be passed to the callback if a tab-shortcut is found
 * @entries     : a #XfceGtkActionEntry[]
 * @entry_count : the number of entries in @entries
 *
 * The Tab key is used to navigate the interface by GTK+ so we need to handle shortcuts with the Tab accelerator manually.
 * Tab sometimes becomes ISO_Left_Tab(e.g. in Ctrl+Shift+Tab) so check both here.
 *
 * Return value: a boolean that is TRUE if the event was handled, otherwise it is FALSE
**/
gboolean
etk_handle_tab_accels(GdkEventKey        *key_event,
                       GtkAccelGroup      *accel_group,
                       gpointer            data,
                       EtkActionEntry *entries,
                       size_t              entry_count)
{
    const guint modifiers = key_event->state & gtk_accelerator_get_default_mod_mask();

    if (G_UNLIKELY(key_event->keyval == GDK_KEY_Tab || key_event->keyval == GDK_KEY_ISO_Left_Tab) && key_event->type == GDK_KEY_PRESS)
    {
        GtkAccelGroupEntry  *group_entries;
        guint                group_entries_count;

        group_entries = gtk_accel_group_query(accel_group, key_event->keyval, modifiers, &group_entries_count);
        if (group_entries_count > 1)
        {
            g_error("Found multiple shortcuts that include the Tab key and the same modifiers.");
        }
        else if (group_entries_count == 1)
        {
            const gchar *path = g_quark_to_string(group_entries[0].accel_path_quark);
            return etk_execute_tab_accel(path, data, entries, entry_count);
        }
    }

    return GDK_EVENT_PROPAGATE;
}



/**
 * etk_execute_tab_accel
 * @accel_path : the accelerator path of the action that we want to activate
 * @data        : a pointer of data that will be passed to the callback if a tab-shortcut is found
 * @entries     : a #XfceGtkActionEntry[]
 * @entry_count : the number of entries in @entries
 *
 * Activates the callback function of the #XfceGtkActionEntry that corresponds to @accel_path. If no such action
 * exists in @entries, then nothing happens.
 *
 * Return value: a boolean that is TRUE if the action was found, otherwise it is FALSE
**/
gboolean
etk_execute_tab_accel(const gchar        *accel_path,
                       gpointer            data,
                       EtkActionEntry *entries,
                       size_t              entry_count)
{
    for(size_t i = 0; i < entry_count; i++)
    {
        if (g_strcmp0(accel_path, entries[i].accel_path) == 0)
        {
           ((void(*)(void *)) entries[i].callback)(data);
            return GDK_EVENT_STOP;
        }
    }

    return GDK_EVENT_PROPAGATE;
}



/**
 * etk_button_new_mixed:
 * @stock_id : the name of the stock item.
 * @label    : the text of the button, with an underscore in front of
 *             the mnemonic character.
 *
 * Creates a new #GtkButton containing a mnemonic label and a stock icon.
 * The @stock_id could be something like #GTK_STOCK_OK or #GTK_STOCK_APPLY.
 *
 * When the @stock_id is %NULL a normal mnemonic button will be created,
 * when @label is %NULL a stock button will be created. This behaviour
 * is added for xfce_message_dialog_new().
 *
 * Return value:(transfer full): the newly created #GtkButton widget.
 **/
GtkWidget *
etk_button_new_mixed(const gchar *stock_id,
                      const gchar *label)
{
    GtkWidget *button;
    GtkWidget *image;

    g_return_val_if_fail(stock_id != NULL || label != NULL, NULL);

    if (label != NULL)
    {
        button = gtk_button_new_with_mnemonic(label);

        if (stock_id != NULL && strlen(stock_id) > 0)
        {
            /* create image widget */
            image = gtk_image_new_from_icon_name(stock_id, GTK_ICON_SIZE_BUTTON);
            gtk_button_set_image(GTK_BUTTON(button), image);
        }
    }
    else
    {
        button = gtk_button_new_with_label(label);
    }

    return button;
}



/**
 * etk_frame_box_new:
 * @label            : the text to use as the label of the frame.
 * @container_return :(out)(allow-none): return location for the frame's container.
 *
 * Creates an Xfce-styled frame. The frame is a #GtkFrame, without
 * outline and an optional bolded text label.  The contents of the
 * frame are indented on the left.
 * The return value is the #GtkFrame itself.  The @container_return is
 * a #GtkAlignment widget to which children of the frame should be added.
 *
 * See also: etk_frame_box_new_with_content().
 *
 * Return value:(transfer full): the newly created #GtkFrame widget.
 **/
GtkWidget *
etk_frame_box_new(const gchar  *label,
                   GtkWidget   **container_return)
{
    GtkWidget *frame;
    GtkWidget *frame_label;
    GtkWidget *container;
    gchar     *markup_label;

    g_return_val_if_fail(container_return != NULL, NULL);

    frame = gtk_frame_new(NULL);
    gtk_frame_set_shadow_type(GTK_FRAME(frame), GTK_SHADOW_NONE);
    gtk_frame_set_label_align(GTK_FRAME(frame), 0.0, 1.0);

    if (G_LIKELY(label != NULL))
    {
        /* create bold label */
        markup_label = g_markup_printf_escaped("<b>%s</b>", label);
        frame_label = gtk_label_new(markup_label);
        gtk_label_set_use_markup(GTK_LABEL(frame_label), TRUE);
        g_free(markup_label);
        gtk_label_set_yalign(GTK_LABEL(frame_label), 0.5);
        gtk_frame_set_label_widget(GTK_FRAME(frame), frame_label);
        gtk_widget_show(frame_label);
    }

    /* We're ignoring this for now because we directly return the alignment
     * and who knows if our consumers want to poke at it. */
    G_GNUC_BEGIN_IGNORE_DEPRECATIONS
    container = gtk_alignment_new(0.0, 0.0, 1.0, 1.0);
    gtk_alignment_set_padding(GTK_ALIGNMENT(container), PADDING, PADDING, PADDING * 3, PADDING);
    gtk_container_add(GTK_CONTAINER(frame), container);
    gtk_widget_show(container);
    G_GNUC_END_IGNORE_DEPRECATIONS

    if (G_LIKELY(container_return != NULL))
        *container_return = container;

    return frame;
}



/**
 * etk_frame_box_new_with_content:
 * @label   : the text to use as the label of the frame.
 * @content : the #GtkWidget to put inside the frame.
 *
 * Creates a widget with etk_frame_box_new() and adds the
 * @content #GtkWidget to the frame.
 *
 * Return value:(transfer full): the newly created #GtkFrame widget.
 **/
GtkWidget *
etk_frame_box_new_with_content(const gchar *label,
                                GtkWidget   *content)
{
    GtkWidget *frame;
    GtkWidget *container;

    frame = etk_frame_box_new(label, &container);
    gtk_container_add(GTK_CONTAINER(container), content);

    return frame;
}



/**
 * etk_menu_popup_until_mapped:
 * @menu: a #GtkMenu.
 * @parent_menu_shell:(allow-none): the menu shell containing the triggering menu item, or %NULL.
 * @parent_menu_item:(allow-none): the menu item whose activation triggered the popup, or %NULL.
 * @func:(scope call)(allow-none): a user supplied function used to position the menu, or %NULL.
 * @data:(allow-none): user supplied data to be passed to func.
 * @button: the mouse button which was pressed to initiate the event.
 * @activate_time: the time at which the activation event occurred.
 *
 * Attempts to pop up a #GtkMenu for a short duration. Unlike the original
 * gtk_menu_popup(), this function will verify that the menu has been mapped
 * or will keep trying for up to 250ms. It will also return a value indicating
 * whether the menu was eventually mapped or not. Following is an excerpt from
 * the GTK+ Documentation on #GtkMenu.
 *
 * Displays a menu and makes it available for selection.
 *
 * Applications can use this function to display context-sensitive menus, and will
 * typically supply %NULL for the @parent_menu_shell, @parent_menu_item, @func and
 * @data parameters. The default menu positioning function will position the menu
 * at the current mouse cursor position.
 *
 * The @button parameter should be the mouse button pressed to initiate the menu
 * popup. If the menu popup was initiated by something other than a mouse button
 * press, such as a mouse button release or a keypress, button should be 0.
 *
 * The @activate_time parameter is used to conflict-resolve initiation of concurrent
 * requests for mouse/keyboard grab requests. To function properly, this needs to
 * be the timestamp of the user event(such as a mouse click or key press) that
 * caused the initiation of the popup. Only if no such event is available,
 * gtk_get_current_event_time() can be used instead.
 *
 * Return value: %TRUE if the menu could be mapped, %FALSE otherwise.
 *
 * Since: 4.14
 *
 */
gboolean
etk_menu_popup_until_mapped(GtkMenu *menu,
                             GtkWidget *parent_menu_shell,
                             GtkWidget *parent_menu_item,
                             GtkMenuPositionFunc func,
                             gpointer data,
                             guint button,
                             guint32 activate_time)
{
    gint i = 0;

    g_return_val_if_fail(GTK_IS_MENU(menu), FALSE);

    while((i++ < 2500) &&(!gtk_widget_get_mapped(GTK_WIDGET(menu))))
    {
        G_GNUC_BEGIN_IGNORE_DEPRECATIONS
        gtk_menu_popup(GTK_MENU(menu),
                        parent_menu_shell,
                        parent_menu_item,
                        func,
                        data,
                        button,
                        activate_time);
        G_GNUC_END_IGNORE_DEPRECATIONS

        g_usleep(100);
    }

    return gtk_widget_get_mapped(GTK_WIDGET(menu));
}



/**
 * xfce_widget_reparent:
 * @widget: a #GtkWidget.
 * @new_parent: a #GtkContainer to move the widget into
 *
 * Moves a widget from one GtkContainer to another, handling reference
 * count issues to avoid destroying the widget.
 *
 * Return value: %TRUE if the widget could be moved, %FALSE otherwise.
 *
 * Since: 4.14
 */
gboolean
etk_widget_reparent(GtkWidget *widget,
                      GtkWidget *new_parent)
{
    GtkWidget *parent;

    g_return_val_if_fail(GTK_IS_WIDGET(widget), FALSE);
    g_return_val_if_fail(GTK_IS_WIDGET(new_parent), FALSE);

    if (!GTK_IS_CONTAINER(new_parent))
        return FALSE;

    parent = gtk_widget_get_parent(widget);
    if (parent)
    {
        g_object_ref(widget);
        gtk_container_remove(GTK_CONTAINER(parent), widget);
        gtk_container_add(GTK_CONTAINER(new_parent), widget);
        g_object_unref(widget);

        return TRUE;
    }

    return FALSE;
}







/**
 * etk_label_set_a11y_relation:
 * @label  : a #GtkLabel.
 * @widget : a #GtkWidget.
 *
 * Sets the %ATK_RELATION_LABEL_FOR relation on @label for @widget, which means
 * accessiblity tools will identify @label as descriptive item for the specified
 * @widget.
 **/
void
etk_label_set_a11y_relation(GtkLabel  *label,
                             GtkWidget *widget)
{
    AtkRelationSet *relations;
    AtkRelation    *relation;
    AtkObject      *object;

    g_return_if_fail(GTK_IS_WIDGET(widget));
    g_return_if_fail(GTK_IS_LABEL(label));

    object = gtk_widget_get_accessible(widget);
    relations = atk_object_ref_relation_set(gtk_widget_get_accessible(GTK_WIDGET(label)));
    relation = atk_relation_new(&object, 1, ATK_RELATION_LABEL_FOR);
    atk_relation_set_add(relations, relation);
    g_object_unref(G_OBJECT(relation));
}


#if 0

gchar* xfce_icon_name_from_desktop_id(const gchar *desktop_id);
GIcon* xfce_gicon_from_name(const gchar *name);
gboolean xfce_has_gtk_frame_extents(GdkWindow *window, GtkBorder *extents);
void etk_window_center_on_active_screen(GtkWindow *window);

#include <X11/Xlib.h>
#include <X11/Xatom.h>

/**
 * xfce_has_gtk_frame_extents:
 * @window : A #GdkWindow
 * @extents : A pointer to a #GtkBorder to copy to.
 *
 * This function can be called to determine if a #GdkWindow is using client-side decorations
 * which is indicated by the _GTK_FRAME_EXTENTS X11 atom. It furthermore sets a pointer
 * of type #GtkBorder to the actual extents.
 *
 * Return value: TRUE if a #GdkWindow has the _GTK_FRAME_EXTENTS atom set.
 *
 * Since: 4.16
 **/
gboolean
xfce_has_gtk_frame_extents(GdkWindow *window,
                            GtkBorder *extents)
{
    /* Code adapted from gnome-flashback:
     * Copyright(C) 2015-2017 Alberts Muktupāvels
     * https://gitlab.gnome.org/GNOME/gnome-flashback/-/commit/f884127
     */

    GdkDisplay *display;
    Display *xdisplay;
    Window xwindow;
    Atom gtk_frame_extents;
    Atom type;
    gint format;
    gulong n_items;
    gulong bytes_after;
    gulong *data;
    gint result;

    display = gdk_display_get_default();
    xdisplay = gdk_x11_display_get_xdisplay(display);
    xwindow = gdk_x11_window_get_xid(window);
    gtk_frame_extents = XInternAtom(xdisplay, "_GTK_FRAME_EXTENTS", False);

    gdk_x11_display_error_trap_push(display);
    result = XGetWindowProperty(xdisplay, xwindow, gtk_frame_extents,
                                 0, G_MAXLONG, False, XA_CARDINAL,
                                 &type, &format, &n_items, &bytes_after,(guchar **)&data);
    gdk_x11_display_error_trap_pop_ignored(display);

    if (data == NULL)
        return FALSE;

    if (result != Success || type != XA_CARDINAL || format != 32 || n_items != 4)
    {
        XFree(data);
        return FALSE;
    }

    extents->left = data[0];
    extents->right = data[1];
    extents->top = data[2];
    extents->bottom = data[3];

    XFree(data);
    return TRUE;
}

/**
 * etk_window_center_on_active_screen:
 * @window: the #GtkWindow to center.
 *
 * Determines the screen that contains the pointer and centers the
 * @window on it. If it failes to determine the current pointer position,
 * @window is centered on the default screen.
 *
 * This function only works properly if you call it before realizing the
 * window and you haven't set a fixed window position using gtk_window_move().
 *
 * See also: xfce_gdk_screen_get_active().
 */
void etk_window_center_on_active_screen(GtkWindow *window)
{
    GdkScreen *screen;

    g_return_if_fail(GTK_IS_WINDOW(window));

    /* get the screen with the pointer */
    screen = xfce_gdk_screen_get_active(NULL);

    gtk_window_set_screen(window, screen);

    /* gtk+ handles the centering of the window properly after resize */
    gtk_window_set_position(window, GTK_WIN_POS_CENTER);
}

/**
 * xfce_icon_name_from_desktop_id:
 * @desktop_id : Name of the desktop file.
 *
 * Return value: %NULL on error, else the string value of the "Icon" property.
 *
 * Since: 4.16
 **/
gchar* xfce_icon_name_from_desktop_id(const gchar *desktop_id)
{
    gchar *icon_file = NULL;
    gchar *resource;
    XfceRc *rcfile;

    resource = g_strdup_printf("applications%c%s.desktop",
                                G_DIR_SEPARATOR,
                                desktop_id);
    rcfile = xfce_rc_config_open(XFCE_RESOURCE_DATA,
                                  resource, TRUE);
    g_free(resource);

    if (rcfile != NULL)
    {
        if (xfce_rc_has_group(rcfile, "Desktop Entry"))
        {
            xfce_rc_set_group(rcfile, "Desktop Entry");
            icon_file = g_strdup(xfce_rc_read_entry(rcfile, "Icon", NULL));
        }

        xfce_rc_close(rcfile);
    }

    return icon_file;
}

/**
 * xfce_gicon_from_name:
 * @name : Name of the application.
 *
 * This function will first look for a desktop file of @name and if successful
 * use the value of the "Icon" property to return a #GIcon.
 * If no desktop file of @name is found it will fallback to returning a #GIcon
 * based on #g_themed_icon_new_with_default_fallbacks and
 * #gtk_icon_theme_lookup_by_gicon.
 *
 * Return value:(transfer full): a new #GThemedIcon.
 *
 * Since: 4.16
 **/
GIcon* xfce_gicon_from_name(const gchar *name)
{
    gchar *icon_name;
    GIcon *gicon = NULL;
    GtkIconInfo *icon_info;
    GFile *path = NULL;

    /* Check if there is a desktop file of 'name' */
    icon_name = xfce_icon_name_from_desktop_id(name);
    if (icon_name)
    {
        if (g_path_is_absolute(icon_name))
        {
            path = g_file_new_for_path(icon_name);
        }
        else if (g_str_has_prefix(icon_name, "file://"))
        {
            path = g_file_new_for_uri(icon_name);
        }
        else
        {
            gicon = g_themed_icon_new_with_default_fallbacks(icon_name);
        }

        if (path)
        {
            gicon = g_file_icon_new(path);
            g_object_unref(path);
        }
        g_free(icon_name);
    }
    else
    {
        gicon = g_themed_icon_new_with_default_fallbacks(name);
    }

    /* As g_themed_icon_new_with_default_fallbacks always returns 'something'
       check if there's anything that matches in the icon theme */
    if (gicon)
    {
        icon_info = gtk_icon_theme_lookup_by_gicon(gtk_icon_theme_get_default(),
                    gicon,
                    GTK_ICON_SIZE_BUTTON,
                    GTK_ICON_LOOKUP_FORCE_REGULAR);

        if (icon_info)
        {
            g_object_unref(icon_info);
            return gicon;
        }
        else
        {
            g_object_unref(gicon);
        }
    }

    return NULL;
}
#endif


