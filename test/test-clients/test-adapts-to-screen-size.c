/* This entire file is licensed under MIT
 *
 * Copyright 2020 William Wold
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "test-client-common.h"

void emit_expectations()
{
    // 1920 and 1080 is the output size defined in mock-server.h

    // Initial setup
    EXPECT_MESSAGE(zwlr_layer_surface_v1 .set_size 0 0);
    EXPECT_MESSAGE(.create_buffer 1920 1080);

    // After callback 1
    EXPECT_MESSAGE(zwlr_layer_surface_v1 .set_size 600 0);
    EXPECT_MESSAGE(.create_buffer 600 1080);

    // After callback 2
    EXPECT_MESSAGE(zwlr_layer_surface_v1 .set_size 600 700);
    EXPECT_MESSAGE(.create_buffer 600 700);
}

static gboolean callback_1(void* data)
{
    GtkWindow* window = data;
    gtk_layer_set_anchor(window, GTK_LAYER_SHELL_EDGE_LEFT, FALSE);
    return FALSE;
}

static gboolean callback_2(void* data)
{
    GtkWindow* window = data;
    gtk_layer_set_anchor(window, GTK_LAYER_SHELL_EDGE_BOTTOM, FALSE);
    add_quit_timeout();
    return FALSE;
}

void run_test()
{
    GtkWindow *window = GTK_WINDOW(gtk_window_new(GTK_WINDOW_TOPLEVEL));

    gtk_layer_init_for_window(window);
    gtk_layer_set_layer(window, GTK_LAYER_SHELL_LAYER_BOTTOM);
    gtk_layer_set_namespace(window, "foobar");
    gtk_layer_set_anchor(window, GTK_LAYER_SHELL_EDGE_BOTTOM, TRUE);
    gtk_layer_set_anchor(window, GTK_LAYER_SHELL_EDGE_TOP, TRUE);
    gtk_layer_set_anchor(window, GTK_LAYER_SHELL_EDGE_LEFT, TRUE);
    gtk_layer_set_anchor(window, GTK_LAYER_SHELL_EDGE_RIGHT, TRUE);

    setup_window(window);
    gtk_widget_set_size_request(GTK_WIDGET(window), 600, 700);
    gtk_window_resize(window, 1, 1);
    gtk_widget_show_all(GTK_WIDGET(window));

    g_timeout_add(50, callback_1, window);
    g_timeout_add(100, callback_2, window);
}
