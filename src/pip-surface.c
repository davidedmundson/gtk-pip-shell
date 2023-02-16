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

#include "pip-surface.h"

#include "gtk-pip-shell.h"
#include "simple-conversions.h"
#include "custom-shell-surface.h"
#include "gtk-wayland.h"

#include "xdg-pip-v1-client.h"
#include "xdg-shell-client.h"

#include <gtk/gtk.h>
#include <gdk/gdkwayland.h>

/*
 * Sets the window's geometry hints (used to force the window to be a specific size)
 * Needs to be called whenever last_configure_size or anchors are changed
 * Lets windows decide their own size along any axis the surface is not stretched along
 * Forces window (by setting the max and min hints) to be of configured size along axes they are stretched along
 */
static void
pip_surface_update_size(PipSurface *self)
{
    GtkWindow *gtk_window = custom_shell_surface_get_gtk_window((CustomShellSurface *)self);

    gint width = -1;
    gint height = -1;

    width = self->last_configure_size.width;

    height = self->last_configure_size.height;

    GdkGeometry hints;
    hints.min_width = width;
    hints.max_width = width;
    hints.min_height = height;
    hints.max_height = height;

    gtk_window_set_geometry_hints(gtk_window,
                                  NULL,
                                  &hints,
                                  GDK_HINT_MIN_SIZE | GDK_HINT_MAX_SIZE);

    // This will usually get called in a moment by the pip_surface_on_size_allocate () triggered by the above
    // gtk_window_set_geometry_hints (). However in some cases (such as a streatching a window after a size request has
    // been set), an allocate will not be triggered but the set size does need to change. For this reason we make the
    // call here as well and let the later call clean up any mistakes this one makes. This makes the flicker problem
    // worse, but I think it's more important that the end result is correct.
    // pip_surface_send_set_size(self);
}

static void
pip_surface_handle_configure(void *data,
                             struct xdg_pip_v1 *_surface,
                             int32_t w,
                             int32_t h,
                             struct wl_array *_states)
{
    PipSurface *self = data;

    self->last_configure_size = (GtkRequisition){
        .width = w,
        .height = h,
    };

    pip_surface_update_size(self);
}

static void
pip_surface_handle_configure_bounds(void *data,
                                    struct xdg_pip_v1 *surface,
                                    int32_t w,
                                    int32_t h)
{
        PipSurface *self = data;

}

static void
pip_surface_handle_dismissed(void *data,
                             struct xdg_pip_v1 *_surface)
{
    PipSurface *self = data;
    (void)_surface;

    GtkWindow *gtk_window = custom_shell_surface_get_gtk_window((CustomShellSurface *)self);
    gtk_window_close(gtk_window);
}

static const struct xdg_pip_v1_listener pip_surface_listener = {
    .configure_bounds = pip_surface_handle_configure_bounds,
    .configure = pip_surface_handle_configure,
    .dismissed = pip_surface_handle_dismissed,
};

static void
xdg_surface_handle_configure(void *data,
                             struct xdg_surface *_xdg_surface,
                             uint32_t serial)
{
    PipSurface *self = data;
    (void)_xdg_surface;

    xdg_surface_ack_configure(self->xdg_surface, serial);
}

static const struct xdg_surface_listener xdg_surface_listener = {
    .configure = xdg_surface_handle_configure,
};

static void
pip_surface_map(CustomShellSurface *super, struct wl_surface *wl_surface)
{
    PipSurface *self = (PipSurface *)super;

    g_return_if_fail(!self->pip_surface);

    struct xdg_wm_pip_v1 *pip_shell_global = gtk_wayland_get_pip_shell_global();
    g_return_if_fail(pip_shell_global);

    struct xdg_wm_base *xdg_shell_global = gtk_wayland_get_xdg_wm_base_global();
    g_return_if_fail(xdg_shell_global);

    self->xdg_surface = xdg_wm_base_get_xdg_surface(xdg_shell_global, wl_surface);
    self->pip_surface = xdg_wm_pip_v1_get_xdg_pip(pip_shell_global,
                                                  self->xdg_surface);

    g_return_if_fail(self->pip_surface);

    // set some properties here

    const char *app_id = pip_surface_get_app_id(self);

    xdg_surface_add_listener(self->xdg_surface, &xdg_surface_listener, self);
    xdg_pip_v1_add_listener(self->pip_surface, &pip_surface_listener, self);
}

static void
pip_surface_unmap(CustomShellSurface *super)
{
    PipSurface *self = (PipSurface *)super;

    if (self->pip_surface)
    {
        xdg_pip_v1_destroy(self->pip_surface);
        self->pip_surface = NULL;
    }
}

static void
pip_surface_finalize(CustomShellSurface *super)
{
    PipSurface *self = (PipSurface *)super;
    pip_surface_unmap(super);
    g_free((gpointer)self->app_id);
}

static GdkRectangle
pip_surface_get_logical_geom(CustomShellSurface *super)
{
    (void)super;
    return (GdkRectangle){0, 0, 0, 0};
}

static const CustomShellSurfaceVirtual pip_surface_virtual = {
    .map = pip_surface_map,
    .unmap = pip_surface_unmap,
    .finalize = pip_surface_finalize,
    .get_logical_geom = pip_surface_get_logical_geom,
};

static void
pip_surface_on_size_allocate(GtkWidget *_gtk_window,
                             GdkRectangle *allocation,
                             PipSurface *self)
{
    (void)_gtk_window;

    if (self->current_allocation.width != allocation->width ||
        self->current_allocation.height != allocation->height)
    {

        self->current_allocation = (GtkRequisition){
            .width = allocation->width,
            .height = allocation->height,
        };
    }
}

PipSurface *
pip_surface_new(GtkWindow *gtk_window)
{
    g_return_val_if_fail(gtk_wayland_get_pip_shell_global(), NULL);

    PipSurface *self = g_new0(PipSurface, 1);
    self->super.virtual = &pip_surface_virtual;
    custom_shell_surface_init((CustomShellSurface *)self, gtk_window);

    self->current_allocation = (GtkRequisition){
        .width = 0,
        .height = 0,
    };
    self->cached_pip_size = self->current_allocation;
    self->last_configure_size = self->current_allocation;
    self->app_id = NULL;
    self->pip_surface = NULL;

    gtk_window_set_decorated(gtk_window, FALSE);
    g_signal_connect(gtk_window, "size-allocate", G_CALLBACK(pip_surface_on_size_allocate), self);

    return self;
}

void pip_surface_set_app_id(PipSurface *self, char const *app_id)
{
    if (g_strcmp0(self->app_id, app_id) != 0)
    {
        g_free((gpointer)self->app_id);
        self->app_id = g_strdup(app_id);
        if (self->pip_surface)
        {
            xdg_pip_v1_set_app_id(self->pip_surface, app_id);
        }
    }
}

const char *
pip_surface_get_app_id(PipSurface *self)
{
    if (self && self->app_id)
        return self->app_id;
    else
        return "gtk-pip-shell";
}

PipSurface *
custom_shell_surface_get_pip_surface(CustomShellSurface *shell_surface)
{
    if (shell_surface && shell_surface->virtual == &pip_surface_virtual)
        return (PipSurface *)shell_surface;
    else
        return NULL;
}
