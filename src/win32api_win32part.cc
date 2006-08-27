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

int
win32_ReadRegistry( const char *key,
                    const char *subkey,
                    const char *value,
                    char * buffer,
                    int  * buffer_sz
                  )
{
    HKEY hprimkey, hsubkey;
    if ( 0== strcmp(key, "HKEY_CLASSES_ROOT") ||
         0== strcmp(key, "HKCR")) {
        hprimkey= HKEY_CLASSES_ROOT;
    } else
    if ( 0== strcmp(key, "HKEY_CURRENT_USER") ||
         0== strcmp(key, "HKCU")) {
        hprimkey= HKEY_CURRENT_USER;
    } else
    if ( 0== strcmp(key, "HKEY_LOCAL_MACHINE") ||
         0== strcmp(key, "HKLM")) {
        hprimkey= HKEY_LOCAL_MACHINE;
    } else
    if ( 0== strcmp(key, "HKEY_USERS") ||
         0== strcmp(key, "HKU")) {
        hprimkey= HKEY_USERS;
    } else {
        return -1; // We can't handle this key
    }
    int retval;

    retval=
    RegOpenKeyEx(hprimkey, subkey, 0, KEY_READ, &hsubkey);
    if (retval == NO_ERROR) {
        DWORD dwBuffSz= *buffer_sz;
        retval=
        RegQueryValueEx(hsubkey, value, NULL, NULL, 
                (BYTE *) buffer, & dwBuffSz);
        *buffer_sz = dwBuffSz;
    }

    RegCloseKey(hsubkey);
    return retval;
}
