/*
 * Interface to win32 APIs
 * 
 * Copyright (C) 2002 Andy Adler <adler@ncf.ca>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * $Id$
 */


#include <windows.h>

int
win32_MessageBox( const char * text,
                  const char * title,
                  int boxtype)
{
    return
    MessageBox( NULL, text, title, boxtype | MB_SETFOREGROUND );
}
