/*
  Copyright (C) 2002 Andy Adler <adler@ncf.ca>
  Copyright (C) 2019 John Donoghue <john.donoghue@ieee.org>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; If not, see <http://www.gnu.org/licenses/>.

*/

/*
 * Get graphical coordinates from screen
 * The windows code needs to be separate from the octave
 * code because you can't #include oct.h and windows.h
 * together
 *
 * $Id$
 */

#include <octave/oct.h>

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#ifdef USING_WINDOWS
#include <windows.h>

 
// This is a really painful way to get mouse positions
//
// Windows does not allow a console mode applications easy
// access to windows events. If you want to get at the
// windows message loop you can:
//
// 1) create a window - but this means popping up an
// unnecessary window - or code to make it hide, and 
// means adding windows resources to the executable.
// (or using one of the windows predefined window
// types)
//
// 2) you can access console events as done here, 
// but you don't get the pointer position associated
// with keyboard events.
//
// The way I do it here is to block on keyboard events
// and then get the mouse position. The problem with
// this is that its intrinsically open to race conditions.

// returns 1 if points grabbed
// returns 0 if no points grabbed
// returns -ve if error

int
grab_win32_getmousepos (int *xpt, int *ypt)
{
    POINT pt;
    GetCursorPos (&pt);
    *xpt= pt.x;
    *ypt= pt.y;
    return 0;
}
#else
int
grab_win32_getmousepos (int *xpt, int *ypt )
{
  *xpt = *ypt = 0;
  return 0;
}
#endif
