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

#ifndef GTK_LAYER_SHELL_H
#define GTK_LAYER_SHELL_H

#include <gtk/gtk.h>

/**
 * SECTION:gtk-pip-shell
 * @title: Gtk Layer Shell
 * @short_description: A library to write GTK Applications using Layer Shell
 *
 * insert more general verbiage here
 *
 * # Forcing Window Size
 * If you wish to force your pip surface window to be a different size than it
 * is by default:
 * |[<!-- language="C" -->
 *   gtk_widget_set_size_request (GTK_WIDGET (pip_gtk_window), width, height);
 *   // force the window to resize to the new request
 *   gtk_window_resize (pip_gtk_window, 1, 1);
 * ]|
 * If width or height is -1, the default is used for that axis. If the window is
 * anchored to opposite edges of the output (see gtk_pip_set_anchor ()), the 
 * size request is ignored. If you later wish to use the default window size,
 * simply repeat the two calls but with both width and height as -1.
 */

G_BEGIN_DECLS

/**
 * gtk_pip_get_major_version:
 *
 * Returns: the major version number of the GTK Layer Shell library
 *
 * Since: 0.4
 */
guint gtk_pip_get_major_version();

/**
 * gtk_pip_get_minor_version:
 *
 * Returns: the minor version number of the GTK Layer Shell library
 *
 * Since: 0.4
 */
guint gtk_pip_get_minor_version();

/**
 * gtk_pip_get_micro_version:
 *
 * Returns: the micro/patch version number of the GTK Layer Shell library
 *
 * Since: 0.4
 */
guint gtk_pip_get_micro_version();

/**
 * gtk_pip_is_supported:
 *
 * May block for a Wayland roundtrip the first time it's called.
 *
 * Returns: %TRUE if the platform is Wayland and Wayland compositor supports the
 * zwlr_pip_shell_v1 protocol.
 *
 * Since: 0.5
 */
gboolean gtk_pip_is_supported();

/**
 * gtk_pip_get_protocol_version:
 *
 * May block for a Wayland roundtrip the first time it's called.
 *
 * Returns: version of the zwlr_pip_shell_v1 protocol supported by the
 * compositor or 0 if the protocol is not supported.
 *
 * Since: 0.6
 */
guint gtk_pip_get_protocol_version();

/**
 * gtk_pip_init_for_window:
 * @window: A #GtkWindow to be turned into a pip surface.
 *
 * Set the @window up to be a pip surface once it is mapped. this must be called before
 * the @window is realized.
 */
void gtk_pip_init_for_window(GtkWindow *window);

/**
 * gtk_pip_is_pip_window:
 * @window: A #GtkWindow that may or may not have a pip surface.
 *
 * Returns: if @window has been initialized as a pip surface.
 *
 * Since: 0.5
 */
gboolean gtk_pip_is_pip_window(GtkWindow *window);

/**
 * gtk_pip_get_zwlr_pip_surface_v1:
 * @window: A pip surface.
 *
 * Returns: The underlying pip surface Wayland object
 *
 */
struct xdg_pip_v1 *gtk_pip_get_zwlr_pip_surface_v1(GtkWindow *window);

/**
 * gtk_pip_set_app_id:
 * @window: A pip surface.
 * @name_space: The namespace of this pip surface.
 *
 * Set the "namespace" of the surface.
 *
 * No one is quite sure what this is for, but it probably should be something generic
 * ("panel", "osk", etc). The @name_space string is copied, and caller maintains
 * ownership of original. If the window is currently mapped, it will get remapped so
 * the change can take effect.
 *
 * Default is "gtk-pip-shell" (which will be used if set to %NULL)
 */
void gtk_pip_set_app_id(GtkWindow *window, char const *name_space);

/**
 * gtk_pip_get_app_id:
 * @window: A pip surface.
 *
 * NOTE: this function does not return ownership of the string. Do not free the returned string.
 * Future calls into the library may invalidate the returned string.
 *
 * Returns: a reference to the namespace property. If namespace is unset, returns the
 * default namespace ("gtk-pip-shell"). Never returns %NULL.
 *
 */
const char *gtk_pip_get_app_id(GtkWindow *window);

void gtk_pip_move(GtkWindow *window);

void gtk_pip_resize(GtkWindow *window, GdkWindowEdge edge);

G_END_DECLS

#endif // GTK_LAYER_SHELL_H
