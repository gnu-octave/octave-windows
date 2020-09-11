/*
 * Interface to win32 APIs
 * 
 * Copyright (C) 2002-2018 Andy Adler <adler@ncf.ca>
 * Copyright (C) 2019-2020 John Donoghue <john.donoghue@ieee.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; If not, see <http://www.gnu.org/licenses/>.
 *
 * $Id$
 */

#include <octave/oct.h>
#include <octave/Cell.h>

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#ifdef USING_WINDOWS
#include <windows.h>

HKEY string_to_rootkey(const char *key)
{
  HKEY hprimkey = 0;

  if (0 == strcmp(key, "HKEY_CLASSES_ROOT") ||
      0 == strcmp(key, "HKCR"))
    {
      hprimkey= HKEY_CLASSES_ROOT;
    }
  else if (0== strcmp(key, "HKEY_CURRENT_USER") ||
           0== strcmp(key, "HKCU")) 
      {
        hprimkey= HKEY_CURRENT_USER;
      }
  else if (0== strcmp(key, "HKEY_LOCAL_MACHINE") ||
           0== strcmp(key, "HKLM")) 
      {
        hprimkey= HKEY_LOCAL_MACHINE;
      }
  else if ( 0== strcmp(key, "HKEY_USERS") ||
            0== strcmp(key, "HKU")) 
      {
        hprimkey= HKEY_USERS;
      }
  else if ( 0== strcmp(key, "HKEY_CURRENT_CONFIG") ||
            0== strcmp(key, "HKCC")) 
      {
        hprimkey= HKEY_CURRENT_CONFIG;
      }
  else if ( 0== strcmp(key, "HKEY_PERFORMANCE_DATA"))
      {
        hprimkey= HKEY_PERFORMANCE_DATA;
      }

  return hprimkey;
}

bool win32_IsValidRootKey (const char *key)
{
  return string_to_rootkey (key) != 0;
}

int
win32_MessageBox (const char * text,
                  const char * title,
                  int boxtype)
{
  return MessageBox (NULL, text, title, boxtype | MB_SETFOREGROUND );
}

char *octave_to_win32_reg(const octave_value &v, int *sz, int *type)
{
  if (v.is_string ())
    {
      std::string str = v.string_value();
      *sz = str.length();
      *type = REG_SZ;
      return strdup(str.c_str());
    }

  else if (v.length() == 1 && v.OV_ISNUMERIC())
    {
      DWORD val = v.int_value();

      char * buffer = new char[sizeof(DWORD)];
      *sz = sizeof(DWORD);
      *type =  REG_DWORD;
      memcpy (buffer, (void*)&val, sizeof(DWORD));

      return buffer;
    }

  return 0;
}

octave_value win32_reg_to_octave(char *buffer, int sz, int type)
{
  octave_value retval;

  if (type == REG_DWORD)
    {
      retval = octave_int32(*(reinterpret_cast<DWORD*>(buffer)));
    }
  else if (type == REG_SZ || type == REG_EXPAND_SZ)
    {
      retval = string_vector (std::string(buffer));
    }
  else
    {
      retval = Matrix(0,0);
    }

  return retval;
}

int
win32_ScanRegistry (const char *key,
                    const char *subkey,
                    std::list<std::string> &fields)
{
  HKEY hprimkey, hsubkey;

  fields.clear ();

  hprimkey = string_to_rootkey (key);
  if (hprimkey == 0)
    {
      return -1; // We can't handle this key
    }

  int retval;

  retval= RegOpenKeyEx (hprimkey, subkey, 0, KEY_READ, &hsubkey);
  if (retval == NO_ERROR)
    {
      int count = 0;
      #define MAX_KEYNAME_SIZE 256
      DWORD keynamesize = MAX_KEYNAME_SIZE;
      char keyname[MAX_KEYNAME_SIZE+1];

      while ( (retval = RegEnumValue (hsubkey, count, keyname, 
                                      &keynamesize, NULL, NULL, NULL, NULL
            )) == ERROR_SUCCESS)
        {
          fields.push_back(keyname);
          keynamesize = MAX_KEYNAME_SIZE;
          count ++;
        }

      if (retval == ERROR_NO_MORE_ITEMS)
        retval = NO_ERROR;

      RegCloseKey (hsubkey);
    }

  return retval;
}

int
win32_ScanRegistryKeys (const char *key,
                    const char *subkey,
                    std::list<std::string> &fields)
{
  HKEY hprimkey, hsubkey;

  fields.clear ();

  hprimkey = string_to_rootkey (key);
  if (hprimkey == 0)
    {
      return -1; // We can't handle this key
    }

  int retval;

  retval= RegOpenKeyEx (hprimkey, subkey, 0, KEY_READ, &hsubkey);
  if (retval == NO_ERROR)
    {
      int count = 0;
      #define MAX_KEYNAME_SIZE 256
      DWORD keynamesize = MAX_KEYNAME_SIZE;
      char keyname[MAX_KEYNAME_SIZE+1];

      while ( (retval = RegEnumKeyEx (hsubkey, count, keyname, 
                                      &keynamesize, NULL, NULL, NULL, NULL
            )) == ERROR_SUCCESS)
        {
          fields.push_back(keyname);
          keynamesize = MAX_KEYNAME_SIZE;
          count ++;
        }

      if (retval == ERROR_NO_MORE_ITEMS)
        retval = NO_ERROR;

      RegCloseKey (hsubkey);
    }

  return retval;
}

int
win32_ReadRegistry (const char *key,
                    const char *subkey,
                    const char *value,
                    char * buffer,
                    int  * buffer_sz,
		    int  * type)
{
  HKEY hprimkey, hsubkey;

  hprimkey = string_to_rootkey(key);

  if (hprimkey == 0)
    {
      return -1; // We can't handle this key
    }

  int retval;

  retval= RegOpenKeyEx (hprimkey, subkey, 0, KEY_READ, &hsubkey);
  if (retval == NO_ERROR)
    {
      DWORD dwBuffSz= *buffer_sz;
      DWORD reg_type;
      retval= RegQueryValueEx (hsubkey, value, NULL, &reg_type, 
                               (BYTE *) buffer, & dwBuffSz);
      *buffer_sz = dwBuffSz;
      if (type)
        *type = (int)reg_type;
    }

  RegCloseKey (hsubkey);
  return retval;
}

int
win32_WriteRegistry (const char *key,
                    const char *subkey,
                    const char *value,
                    char * buffer,
                    int  buffer_sz,
		    int  type)
{
  HKEY hprimkey, hsubkey;

  hprimkey = string_to_rootkey(key);

  if (hprimkey == 0)
    {
      return -1; // We can't handle this key
    }

  int retval;

  retval = RegOpenKeyEx (hprimkey, subkey, 0, KEY_SET_VALUE, &hsubkey);
  if (retval == NO_ERROR)
    {
      retval = RegSetValueEx (hsubkey, value, 0, type, reinterpret_cast<BYTE*>(buffer), buffer_sz);
    }

  RegCloseKey (hsubkey);
  return retval;
}

#endif
