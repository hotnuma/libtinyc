/*
 * Copyright (c) 2007 The Xfce Development Team
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
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

#ifndef __LIBETK_H__
#define __LIBETK_H__

#include "libemenu.h"

gboolean etk_execute_tab_accel(const gchar    *accel_path,
                               gpointer       data,
                               EtkActionEntry *entries,
                               size_t         entry_count);
GtkWidget* etk_button_new_mixed(const gchar *stock_id, const gchar *label)
                                G_GNUC_MALLOC;
GtkWidget* etk_frame_box_new(const gchar *label, GtkWidget **container_return)
                             G_GNUC_MALLOC;
GtkWidget* etk_frame_box_new_with_content(const gchar *label, GtkWidget *content)
                                          G_GNUC_MALLOC;

gboolean etk_menu_popup_until_mapped(GtkMenu             *menu,
                                     GtkWidget           *parent_menu_shell,
                                     GtkWidget           *parent_menu_item,
                                     GtkMenuPositionFunc func,
                                     gpointer            data,
                                     guint               button,
                                     guint32             activate_time);

gboolean etk_widget_reparent(GtkWidget *widget, GtkWidget *new_parent);
void etk_label_set_a11y_relation(GtkLabel *label, GtkWidget *widget);

#endif // __LIBETK_H__


