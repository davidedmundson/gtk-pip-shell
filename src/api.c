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
#include "gtk-wayland.h"
#include "custom-shell-surface.h"
#include "simple-conversions.h"
#include "pip-surface.h"
#include "xdg-toplevel-surface.h"

#include <gdk/gdkwayland.h>

guint gtk_pip_get_major_version()
{
    return GTK_LAYER_SHELL_MAJOR;
}

guint gtk_pip_get_minor_version()
{
    return GTK_LAYER_SHELL_MINOR;
}

guint gtk_pip_get_micro_version()
{
    return GTK_LAYER_SHELL_MICRO;
}

gboolean
gtk_pip_is_supported()
{
    if (!GDK_IS_WAYLAND_DISPLAY(gdk_display_get_default()))
        return FALSE;
    gtk_wayland_init_if_needed();
    return gtk_wayland_get_pip_shell_global() != NULL;
}

guint gtk_pip_get_protocol_version()
{
    if (!GDK_IS_WAYLAND_DISPLAY(gdk_display_get_default()))
        return 0;
    gtk_wayland_init_if_needed();
    struct xdg_wm_pip_v1 *pip_shell_global = gtk_wayland_get_pip_shell_global();
    if (!pip_shell_global)
        return 0;
    return xdg_wm_pip_v1_get_version(pip_shell_global);
}

static PipSurface *
gtk_window_get_pip_surface(GtkWindow *window)
{
    g_return_val_if_fail(window, NULL);
    CustomShellSurface *shell_surface = gtk_window_get_custom_shell_surface(window);
    if (!shell_surface)
    {
        g_critical("GtkWindow is not a pip surface. Make sure you called gtk_pip_init_for_window ()");
        return NULL;
    }
    PipSurface *pip_surface = custom_shell_surface_get_pip_surface(shell_surface);
    if (!pip_surface)
    {
        g_critical("Custom wayland shell surface is not a pip surface, your Wayland compositor may not support PIP Shell");
        return NULL;
    }
    return pip_surface;
}

void gtk_pip_init_for_window(GtkWindow *window)
{
    gtk_wayland_init_if_needed();
    PipSurface *pip_surface = pip_surface_new(window);
    if (!pip_surface)
    {
        g_warning("Falling back to XDG shell instead of Layer Shell (surface should appear but pip features will not work)");
        XdgToplevelSurface *toplevel_surface = xdg_toplevel_surface_new(window);
        if (!toplevel_surface)
        {
            g_warning("Shell does not support XDG shell stable. Falling back to default GTK behavior");
        }
    }
}

gboolean
gtk_pip_is_pip_window(GtkWindow *window)
{
    g_return_val_if_fail(window, FALSE);
    CustomShellSurface *shell_surface = gtk_window_get_custom_shell_surface(window);
    if (!shell_surface)
        return FALSE;
    PipSurface *pip_surface = custom_shell_surface_get_pip_surface(shell_surface);
    return pip_surface != NULL;
}

struct xdg_pip_v1 *
gtk_pip_get_zwlr_pip_surface_v1(GtkWindow *window)
{
    PipSurface *pip_surface = gtk_window_get_pip_surface(window);
    if (!pip_surface)
        return NULL; // Error message already shown in gtk_window_get_pip_surface
    return pip_surface->pip_surface;
}

void gtk_pip_set_app_id(GtkWindow *window, char const *app_id)
{
    PipSurface *pip_surface = gtk_window_get_pip_surface(window);
    if (!pip_surface)
        return; // Error message already shown in gtk_window_get_pip_surface
    pip_surface_set_app_id(pip_surface, app_id);
}

const char *
gtk_pip_get_app_id(GtkWindow *window)
{
    PipSurface *pip_surface = gtk_window_get_pip_surface(window);
    return pip_surface_get_app_id(pip_surface); // NULL-safe
}

void gtk_pip_move(GtkWindow *window)
{
    PipSurface *pip_surface = gtk_window_get_pip_surface(window);
    if (!pip_surface)
        return; // Error message already shown in gtk_window_get_pip_surface
    return pip_surface_move(pip_surface);
}
