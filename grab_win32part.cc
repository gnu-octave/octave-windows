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

static HANDLE hStdin;
static DWORD fdwSaveOldMode;

int
grab_win32_initialize() {
  hStdin = GetStdHandle(STD_INPUT_HANDLE); 
  if (hStdin == INVALID_HANDLE_VALUE)
     return -1;

  if (! GetConsoleMode(hStdin, &fdwSaveOldMode))
     return -2;

  DWORD fdwMode = ENABLE_WINDOW_INPUT;
  if (! SetConsoleMode(hStdin, fdwMode) )
     return -3;

  return 0;
}

int
grab_win32_restore() {
  if (! SetConsoleMode(hStdin, fdwSaveOldMode))
      return -4;

  return 0;
}
 
 
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
grab_win32_getcharandmouse ( char * ch, int * xpt, int * ypt )
{
  DWORD cNumRead;
  INPUT_RECORD irin;
  if (! ReadConsoleInput( 
          hStdin, &irin, 1, &cNumRead) )
     return -6;
 
  if (cNumRead ==1                 &&
      irin.EventType == KEY_EVENT  &&
      irin.Event.KeyEvent.bKeyDown) {
    KEY_EVENT_RECORD ke= irin.Event.KeyEvent;
    *ch= ke.uChar.AsciiChar;
    POINT pt;
    GetCursorPos( &pt );
    *xpt= pt.x;
    *ypt= pt.y;
    return 1;
  }
  else 
    return 0;
}

