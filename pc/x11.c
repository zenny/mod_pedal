/*
 * Copyright (C) 2015  Alister Sanders
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include <X11/extensions/XTest.h>
#include "x11.h"

static Display* display;
static Window root_window;

int init_x11()
{
    /* Initialise X11 */
    display = XOpenDisplay(getenv("DISPLAY"));
    if (display == NULL) return 0;
    
    root_window = DefaultRootWindow(display);

    return 1;
}

void press_key(int keysym, Bool press)
{
    XTestFakeKeyEvent(display, XKeysymToKeycode(display, keysym), press, 0);
    XFlush(display);
}
