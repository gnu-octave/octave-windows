/*
 * Get graphical coordinates from screen
 * The windows code needs to be separate from the octave
 * code because you can't #include oct.h and windows.h
 * together
 * 
 * Andy Adler <adler@ncf.ca> 2002
 *
 * $Id$
 */


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
grab_win32_getmousepos ( int * xpt, int * ypt )
{
    POINT pt;
    GetCursorPos( &pt );
    *xpt= pt.x;
    *ypt= pt.y;
    return 0;
}
