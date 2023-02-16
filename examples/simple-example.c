/* This entire file is licensed under MIT
 *
 * Copyright 2020 Sophie Winter
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "gtk-pip-shell.h"
#include <gtk/gtk.h>

void on_button_clicked(GtkButton *button, gpointer data)
{

    GtkWindow *window;
    window = (GtkWindow *)data;
    if (window == NULL)
        return;

    gtk_pip_move(window);
    return;
}

static void
activate(GtkApplication *app, void *_data)
{
    (void)_data;

    // Create a normal GTK window however you like
    GtkWindow *gtk_window = GTK_WINDOW(gtk_application_window_new(app));

    // Before the window is first realized, set it up to be a pip surface
    gtk_pip_init_for_window(gtk_window);

    gtk_pip_set_app_id(gtk_window, "dave");

    // Set up a widget
    GtkWidget *label = gtk_label_new("");
    gtk_label_set_markup(GTK_LABEL(label),
                         "<span font_desc=\"20.0\">"
                         "GTK PIP Shell example!"
                         "</span>");
    // gtk_container_add(GTK_CONTAINER(gtk_window), label);

    GtkWidget *button = gtk_button_new_with_label("Move");
    g_signal_connect(G_OBJECT(button),
                     "clicked", G_CALLBACK(on_button_clicked), gtk_window);
    gtk_container_add(GTK_CONTAINER(gtk_window), button);

    gtk_container_set_border_width(GTK_CONTAINER(gtk_window), 12);
    gtk_widget_show_all(GTK_WIDGET(gtk_window));
}

int main(int argc, char **argv)
{
    GtkApplication *app = gtk_application_new("sh.wmww.gtk-layer-example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
