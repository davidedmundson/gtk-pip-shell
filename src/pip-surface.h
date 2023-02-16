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

#ifndef LAYER_SHELL_SURFACE_H
#define LAYER_SHELL_SURFACE_H

#include "custom-shell-surface.h"
#include "xdg-pip-v1-client.h"
#include "gtk-pip-shell.h"
#include <gtk/gtk.h>

// A LayerSurface * can be safely cast to a CustomShellSurface *
typedef struct _LayerSurface PipSurface;

// Functions that mutate this structure should all be in pip-surface.c to make the logic easier to understand
// Struct is declared in this header to prevent the need for excess getters
struct _LayerSurface
{
    CustomShellSurface super;

    const char *app_id; // Can be null, freed on destruction

    // Not set by user requests
    struct xdg_pip_v1 *pip_surface; // The actual pip surface Wayland object (can be NULL)
    struct xdg_surface *xdg_surface; // the Wayland object for the underlying xdg_surface(can be NULL)

    GtkRequisition current_allocation; // Last size allocation, or (0, 0) if there hasn't been one
    GtkRequisition last_configure_size; // Last size received from a configure event
};

PipSurface *pip_surface_new (GtkWindow *gtk_window);

// Safe cast, returns NULL if wrong type sent
PipSurface *custom_shell_surface_get_pip_surface (CustomShellSurface *shell_surface);

// Surface is remapped in order to set
void pip_surface_set_app_id (PipSurface *self, char const* name_space); // Makes a copy of the string, can be null

// Returns the effective namespace (default if unset). Does not return ownership. Never returns NULL. Handles null self.
const char* pip_surface_get_app_id (PipSurface *self);

void pip_surface_move(PipSurface *self);

void pip_surface_resize(PipSurface *self, GdkWindowEdge edge);

#endif // LAYER_SHELL_SURFACE_H
