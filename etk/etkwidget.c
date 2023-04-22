#include "etkwidget.h"

bool etk_window_is_last(GtkWindow *window)
{
    GtkWidget *widget = GTK_WIDGET(window);

    GList *list = gtk_window_list_toplevels();

    for (GList *i = list; i; i = i->next)
    {
        GtkWidget *current = GTK_WIDGET(i->data);

        if (!gtk_widget_is_visible(current))
            continue;

        //g_print("type = %s\n", G_OBJECT_TYPE_NAME(G_OBJECT(current)));

        if (current != widget)
            return false;
    }

    return true;
}

GtkWidget* etk_tab_button_new(const char *title, GCallback callback, GtkWidget *widget)
{
    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_container_set_border_width(GTK_CONTAINER(hbox), 2);

    GtkWidget *tab_label = gtk_label_new(title);
    gtk_widget_set_margin_start(tab_label, 2);
    gtk_box_pack_start(GTK_BOX(hbox), tab_label, true, true, 0);

    GtkWidget *button = gtk_button_new();
    gtk_widget_set_focus_on_click(button, false);
    gtk_button_set_relief(GTK_BUTTON(button), GTK_RELIEF_NONE);
    gtk_widget_set_can_focus(button, false);
    gtk_widget_set_can_default(button, false);
    gtk_widget_set_tooltip_text(button, "Close this tab");
    gtk_widget_set_halign(button, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(button, GTK_ALIGN_CENTER);
    gtk_container_add(GTK_CONTAINER(hbox), button);
    g_signal_connect_swapped(G_OBJECT(button), "clicked",
                             G_CALLBACK(callback), widget);

    GtkWidget *image = gtk_image_new_from_icon_name("window-close-symbolic",
                                                    GTK_ICON_SIZE_MENU);
    gtk_container_add(GTK_CONTAINER(button), image);

    gtk_widget_show_all(hbox);

    return hbox;
}

void etk_notebook_append(GtkWidget *notebook,
                         const char *title, GtkWidget *page, bool select,
                         GCallback callback, GtkWidget *widget)
{
    GtkWidget *box = etk_tab_button_new(title, callback, widget);

    int pos = gtk_notebook_append_page(GTK_NOTEBOOK(notebook), page, box);

    if (select)
        gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), pos);
}


