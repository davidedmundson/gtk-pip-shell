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

guint
gtk_pip_get_major_version ()
{
    return GTK_LAYER_SHELL_MAJOR;
}

guint
gtk_pip_get_minor_version ()
{
    return GTK_LAYER_SHELL_MINOR;
}

guint
gtk_pip_get_micro_version ()
{
    return GTK_LAYER_SHELL_MICRO;
}

gboolean
gtk_pip_is_supported ()
{
    if (!GDK_IS_WAYLAND_DISPLAY (gdk_display_get_default ()))
        return FALSE;
    gtk_wayland_init_if_needed ();
    return gtk_wayland_get_pip_shell_global () != NULL;
}

guint
gtk_pip_get_protocol_version ()
{
    if (!GDK_IS_WAYLAND_DISPLAY (gdk_display_get_default ()))
        return 0;
    gtk_wayland_init_if_needed ();
    struct zwlr_pip_shell_v1 *pip_shell_global = gtk_wayland_get_pip_shell_global ();
    if (!pip_shell_global)
        return 0;
    return zwlr_pip_shell_v1_get_version (pip_shell_global);
}

static LayerSurface*
gtk_window_get_pip_surface (GtkWindow *window)
{
    g_return_val_if_fail (window, NULL);
    CustomShellSurface *shell_surface = gtk_window_get_custom_shell_surface (window);
    if (!shell_surface) {
        g_critical ("GtkWindow is not a pip surface. Make sure you called gtk_pip_init_for_window ()");
        return NULL;
    }
    LayerSurface *pip_surface = custom_shell_surface_get_pip_surface (shell_surface);
    if (!pip_surface) {
        g_critical ("Custom wayland shell surface is not a pip surface, your Wayland compositor may not support Layer Shell");
        return NULL;
    }
    return pip_surface;
}

void
gtk_pip_init_for_window (GtkWindow *window)
{
    gtk_wayland_init_if_needed ();
    LayerSurface* pip_surface = pip_surface_new (window);
    if (!pip_surface) {
        g_warning ("Falling back to XDG shell instead of Layer Shell (surface should appear but pip features will not work)");
        XdgToplevelSurface* toplevel_surface = xdg_toplevel_surface_new (window);
        if (!toplevel_surface)
        {
            g_warning ("Shell does not support XDG shell stable. Falling back to default GTK behavior");
        }
    }
}

gboolean
gtk_pip_is_pip_window (GtkWindow *window)
{
    g_return_val_if_fail (window, FALSE);
    CustomShellSurface *shell_surface = gtk_window_get_custom_shell_surface (window);
    if (!shell_surface)
        return FALSE;
    LayerSurface *pip_surface = custom_shell_surface_get_pip_surface (shell_surface);
    return pip_surface != NULL;
}

struct zwlr_pip_surface_v1 *
gtk_pip_get_zwlr_pip_surface_v1 (GtkWindow *window)
{
    LayerSurface *pip_surface = gtk_window_get_pip_surface (window);
    if (!pip_surface) return NULL; // Error message already shown in gtk_window_get_pip_surface
    return pip_surface->pip_surface;
}

void
gtk_pip_set_namespace (GtkWindow *window, char const* name_space)
{
    LayerSurface *pip_surface = gtk_window_get_pip_surface (window);
    if (!pip_surface) return; // Error message already shown in gtk_window_get_pip_surface
    pip_surface_set_name_space (pip_surface, name_space);
}

const char *
gtk_pip_get_namespace (GtkWindow *window)
{
    LayerSurface *pip_surface = gtk_window_get_pip_surface (window);
    // If pip_surface is NULL, error message already shown in gtk_window_get_pip_surface
    return pip_surface_get_namespace (pip_surface); // NULL-safe
}

void
gtk_pip_set_pip (GtkWindow *window, GtkLayerShellLayer pip)
{
    LayerSurface *pip_surface = gtk_window_get_pip_surface (window);
    if (!pip_surface) return; // Error message already shown in gtk_window_get_pip_surface
    pip_surface_set_pip (pip_surface, pip);
}

GtkLayerShellLayer
gtk_pip_get_pip (GtkWindow *window)
{
    LayerSurface *pip_surface = gtk_window_get_pip_surface (window);
    if (!pip_surface) return GTK_LAYER_SHELL_LAYER_TOP; // Error message already shown in gtk_window_get_pip_surface
    return pip_surface->pip;
}

void
gtk_pip_set_monitor (GtkWindow *window, GdkMonitor *monitor)
{
    LayerSurface *pip_surface = gtk_window_get_pip_surface (window);
    if (!pip_surface) return; // Error message already shown in gtk_window_get_pip_surface
    pip_surface_set_monitor (pip_surface, monitor);
}

GdkMonitor *
gtk_pip_get_monitor (GtkWindow *window)
{
    LayerSurface *pip_surface = gtk_window_get_pip_surface (window);
    if (!pip_surface) return NULL; // Error message already shown in gtk_window_get_pip_surface
    return pip_surface->monitor;
}

void
gtk_pip_set_anchor (GtkWindow *window, GtkLayerShellEdge edge, gboolean anchor_to_edge)
{
    LayerSurface *pip_surface = gtk_window_get_pip_surface (window);
    if (!pip_surface) return; // Error message already shown in gtk_window_get_pip_surface
    pip_surface_set_anchor (pip_surface, edge, anchor_to_edge);
}

gboolean
gtk_pip_get_anchor (GtkWindow *window, GtkLayerShellEdge edge)
{
    LayerSurface *pip_surface = gtk_window_get_pip_surface (window);
    if (!pip_surface) return FALSE; // Error message already shown in gtk_window_get_pip_surface
    g_return_val_if_fail(edge >= 0 && edge < GTK_LAYER_SHELL_EDGE_ENTRY_NUMBER, FALSE);
    return pip_surface->anchors[edge];
}

void
gtk_pip_set_margin (GtkWindow *window, GtkLayerShellEdge edge, int margin_size)
{
    LayerSurface *pip_surface = gtk_window_get_pip_surface (window);
    if (!pip_surface) return; // Error message already shown in gtk_window_get_pip_surface
    pip_surface_set_margin (pip_surface, edge, margin_size);
}

int
gtk_pip_get_margin (GtkWindow *window, GtkLayerShellEdge edge)
{
    LayerSurface *pip_surface = gtk_window_get_pip_surface (window);
    if (!pip_surface) return 0; // Error message already shown in gtk_window_get_pip_surface
    g_return_val_if_fail(edge >= 0 && edge < GTK_LAYER_SHELL_EDGE_ENTRY_NUMBER, FALSE);
    return pip_surface->margins[edge];
}

void
gtk_pip_set_exclusive_zone (GtkWindow *window, int exclusive_zone)
{
    LayerSurface *pip_surface = gtk_window_get_pip_surface (window);
    if (!pip_surface) return; // Error message already shown in gtk_window_get_pip_surface
    pip_surface_set_exclusive_zone (pip_surface, exclusive_zone);
}

int
gtk_pip_get_exclusive_zone (GtkWindow *window)
{
    LayerSurface *pip_surface = gtk_window_get_pip_surface (window);
    if (!pip_surface) return 0; // Error message already shown in gtk_window_get_pip_surface
    return pip_surface->exclusive_zone;
}

void
gtk_pip_auto_exclusive_zone_enable (GtkWindow *window)
{
    LayerSurface *pip_surface = gtk_window_get_pip_surface (window);
    if (!pip_surface) return; // Error message already shown in gtk_window_get_pip_surface
    pip_surface_auto_exclusive_zone_enable (pip_surface);
}

gboolean
gtk_pip_auto_exclusive_zone_is_enabled (GtkWindow *window)
{
    LayerSurface *pip_surface = gtk_window_get_pip_surface (window);
    if (!pip_surface) return FALSE; // Error message already shown in gtk_window_get_pip_surface
    return pip_surface->auto_exclusive_zone;
}

void
gtk_pip_set_keyboard_interactivity (GtkWindow *window, gboolean interactivity)
{
    if (interactivity != TRUE && interactivity != FALSE) {
        g_warning (
            "boolean with value %d sent to gtk_pip_set_keyboard_interactivity (), "
            "perhaps gtk_pip_set_keyboard_mode () was intended?",
            interactivity);
    }
    gtk_pip_set_keyboard_mode (
        window,
        interactivity ? GTK_LAYER_SHELL_KEYBOARD_MODE_EXCLUSIVE : GTK_LAYER_SHELL_KEYBOARD_MODE_NONE);
}

gboolean
gtk_pip_get_keyboard_interactivity (GtkWindow *window)
{
    GtkLayerShellKeyboardMode mode = gtk_pip_get_keyboard_mode (window);
    if (mode != GTK_LAYER_SHELL_KEYBOARD_MODE_NONE && mode != GTK_LAYER_SHELL_KEYBOARD_MODE_EXCLUSIVE) {
        g_warning (
            "gtk_pip_get_keyboard_interactivity () used after keyboard mode set to %d,"
            "consider using gtk_pip_get_keyboard_mode ().",
            mode);
    }
    return mode != GTK_LAYER_SHELL_KEYBOARD_MODE_NONE;
}

void
gtk_pip_set_keyboard_mode (GtkWindow *window, GtkLayerShellKeyboardMode mode)
{
    g_return_if_fail(mode < GTK_LAYER_SHELL_KEYBOARD_MODE_ENTRY_NUMBER);
    LayerSurface *pip_surface = gtk_window_get_pip_surface (window);
    if (!pip_surface) return; // Error message already shown in gtk_window_get_pip_surface
    if (mode != GTK_LAYER_SHELL_KEYBOARD_MODE_NONE && mode != GTK_LAYER_SHELL_KEYBOARD_MODE_EXCLUSIVE) {
        if (gtk_pip_get_protocol_version () < 4) {
            g_warning (
                "Compositor does not support requested keyboard interactivity mode, using exclusive mode.");
            mode = GTK_LAYER_SHELL_KEYBOARD_MODE_EXCLUSIVE;
        }
    }
    pip_surface_set_keyboard_mode (pip_surface, mode);
}

GtkLayerShellKeyboardMode
gtk_pip_get_keyboard_mode (GtkWindow *window)
{
    LayerSurface *pip_surface = gtk_window_get_pip_surface (window);
    if (!pip_surface) return GTK_LAYER_SHELL_KEYBOARD_MODE_NONE; // Error message already shown in gtk_window_get_pip_surface
    return pip_surface->keyboard_mode;
}
