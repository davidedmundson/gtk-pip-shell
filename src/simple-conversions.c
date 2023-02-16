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

#include "simple-conversions.h"

enum xdg_pip_v1_resize_edge gdk_get_resize_edge(GdkWindowEdge edge)
{
    switch (edge)
    {
    case GDK_WINDOW_EDGE_NORTH_WEST:
        return XDG_PIP_V1_RESIZE_EDGE_TOP_LEFT;
    case GDK_WINDOW_EDGE_NORTH:
        return XDG_PIP_V1_RESIZE_EDGE_TOP;
    case GDK_WINDOW_EDGE_NORTH_EAST:
        return XDG_PIP_V1_RESIZE_EDGE_TOP_RIGHT;
    case GDK_WINDOW_EDGE_WEST:
        return XDG_PIP_V1_RESIZE_EDGE_LEFT;
    case GDK_WINDOW_EDGE_EAST:
        return XDG_PIP_V1_RESIZE_EDGE_RIGHT;
    case GDK_WINDOW_EDGE_SOUTH_WEST:
        return XDG_PIP_V1_RESIZE_EDGE_BOTTOM_LEFT;
    case GDK_WINDOW_EDGE_SOUTH:
        return XDG_PIP_V1_RESIZE_EDGE_BOTTOM;
    case GDK_WINDOW_EDGE_SOUTH_EAST:
        return XDG_PIP_V1_RESIZE_EDGE_BOTTOM_RIGHT;
    default:
        g_critical("Invalid GdkWindowEdge %d", edge);
        return XDG_PIP_V1_RESIZE_EDGE_NONE;
    }
}

enum xdg_positioner_gravity
gdk_gravity_get_xdg_positioner_gravity(GdkGravity gravity)
{
    switch (gravity)
    {
    case GDK_GRAVITY_NORTH_WEST:
        return XDG_POSITIONER_GRAVITY_BOTTOM_RIGHT;
    case GDK_GRAVITY_NORTH:
        return XDG_POSITIONER_GRAVITY_BOTTOM;
    case GDK_GRAVITY_NORTH_EAST:
        return XDG_POSITIONER_GRAVITY_BOTTOM_LEFT;
    case GDK_GRAVITY_WEST:
        return XDG_POSITIONER_GRAVITY_RIGHT;
    case GDK_GRAVITY_CENTER:
        return XDG_POSITIONER_GRAVITY_NONE;
    case GDK_GRAVITY_EAST:
        return XDG_POSITIONER_GRAVITY_LEFT;
    case GDK_GRAVITY_SOUTH_WEST:
        return XDG_POSITIONER_GRAVITY_TOP_RIGHT;
    case GDK_GRAVITY_SOUTH:
        return XDG_POSITIONER_GRAVITY_TOP;
    case GDK_GRAVITY_SOUTH_EAST:
        return XDG_POSITIONER_GRAVITY_TOP_LEFT;
    case GDK_GRAVITY_STATIC:
        return XDG_POSITIONER_GRAVITY_NONE;
    default:
        g_critical("Invalid GdkGravity %d", gravity);
        return XDG_POSITIONER_GRAVITY_NONE;
    }
}

enum xdg_positioner_anchor
gdk_gravity_get_xdg_positioner_anchor(GdkGravity anchor)
{
    switch (anchor)
    {
    case GDK_GRAVITY_NORTH_WEST:
        return XDG_POSITIONER_ANCHOR_TOP_LEFT;
    case GDK_GRAVITY_NORTH:
        return XDG_POSITIONER_ANCHOR_TOP;
    case GDK_GRAVITY_NORTH_EAST:
        return XDG_POSITIONER_ANCHOR_TOP_RIGHT;
    case GDK_GRAVITY_WEST:
        return XDG_POSITIONER_ANCHOR_LEFT;
    case GDK_GRAVITY_CENTER:
        return XDG_POSITIONER_ANCHOR_NONE;
    case GDK_GRAVITY_EAST:
        return XDG_POSITIONER_ANCHOR_RIGHT;
    case GDK_GRAVITY_SOUTH_WEST:
        return XDG_POSITIONER_ANCHOR_BOTTOM_LEFT;
    case GDK_GRAVITY_SOUTH:
        return XDG_POSITIONER_ANCHOR_BOTTOM;
    case GDK_GRAVITY_SOUTH_EAST:
        return XDG_POSITIONER_ANCHOR_BOTTOM_RIGHT;
    case GDK_GRAVITY_STATIC:
        return XDG_POSITIONER_ANCHOR_NONE;
    default:
        g_critical("Invalid GdkGravity %d", anchor);
        return XDG_POSITIONER_ANCHOR_NONE;
    }
}

enum xdg_positioner_constraint_adjustment
gdk_anchor_hints_get_xdg_positioner_constraint_adjustment(GdkAnchorHints hints)
{
    enum xdg_positioner_constraint_adjustment adjustment = 0;
    if (hints & GDK_ANCHOR_FLIP_X)
        adjustment |= XDG_POSITIONER_CONSTRAINT_ADJUSTMENT_FLIP_X;
    if (hints & GDK_ANCHOR_FLIP_Y)
        adjustment |= XDG_POSITIONER_CONSTRAINT_ADJUSTMENT_FLIP_Y;
    if (hints & GDK_ANCHOR_SLIDE_X)
        adjustment |= XDG_POSITIONER_CONSTRAINT_ADJUSTMENT_SLIDE_X;
    if (hints & GDK_ANCHOR_SLIDE_Y)
        adjustment |= XDG_POSITIONER_CONSTRAINT_ADJUSTMENT_SLIDE_Y;
    if (hints & GDK_ANCHOR_RESIZE_X)
        adjustment |= XDG_POSITIONER_CONSTRAINT_ADJUSTMENT_RESIZE_X;
    if (hints & GDK_ANCHOR_RESIZE_Y)
        adjustment |= XDG_POSITIONER_CONSTRAINT_ADJUSTMENT_RESIZE_Y;
    return adjustment;
}
