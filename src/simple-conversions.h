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

#ifndef SIMPLE_CONVERSIONS_H
#define SIMPLE_CONVERSIONS_H

#include "xdg-shell-client.h"
#include "xdg-pip-v1-client.h"
#include "gtk-pip-shell.h"
#include <gdk/gdk.h>

enum xdg_positioner_gravity gdk_gravity_get_xdg_positioner_gravity (GdkGravity gravity);
enum xdg_positioner_anchor gdk_gravity_get_xdg_positioner_anchor (GdkGravity anchor);
enum xdg_positioner_constraint_adjustment gdk_anchor_hints_get_xdg_positioner_constraint_adjustment (GdkAnchorHints hints);

#endif // SIMPLE_CONVERSIONS_H
