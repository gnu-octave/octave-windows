/*
 * Interface to win32 APIs
 * 
 * Copyright (C) 2002-2020 Andy Adler <adler@ncf.ca>
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

// Load Headers from win32api_win32part.cc
#include <string>
#include <list>


int
win32_MessageBox( const char * text,
                  const char * title,
                  int boxtype);
int
win32_ReadRegistry( const char *key,
                    const char *subkey,
                    const char *value,
                    char * buffer,
                    int  * buffer_sz,
		    int  * type
                  );

int
win32_WriteRegistry(const char *key,
                    const char *subkey,
                    const char *value,
                    char * buffer,
                    int  buffer_sz,
		    int  type
                  );

bool win32_IsValidRootKey(const char *key);

int
win32_ScanRegistry (const char *key,
                    const char *subkey,
                    std::list<std::string> &fields);

int
win32_ScanRegistryKeys (const char *key,
                    const char *subkey,
                    std::list<std::string> &fields);

#include <octave/oct.h>

char *
octave_to_win32_reg(const octave_value &v, int *sz, int *type);

#include <octave/Cell.h>

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif


DEFUN_DLD (win32api, args, , "internal function")
{
  return octave_value ();
}

// PKG_ADD: autoload ("win32_MessageBox", "win32api.oct");
DEFUN_DLD (win32_MessageBox, args, ,
  "-*- texinfo -*-\n \
@deftypefn {Loadable Function} {@var{rv} =} win32_MessageBox (@var{title}, @var{text})\n \
@deftypefnx {Loadable Function} {@var{rv} =} win32_MessageBox (@var{title}, @var{text}, @var{MboxType})\n \
\n \
Display a message box using the win32 API.\n \
\n \
@var{title} MessageBox title string\n \
\n \
@var{text} MessageBox text string\n \
\n \
@var{MBoxType} can be an integer or a string. \n \
\n \
For integer values, consult <windows.h>\n \
\n \
The following string values are recognized:\n \
@itemize\n \
@item @code{MB_OK}\n \
@item @code{MB_OKCANCEL}\n \
@item @code{MB_ABORTRETRYIGNORE}\n \
@item @code{MB_YESNOCANCEL}\n \
@item @code{MB_YESNO}\n \
@item @code{MB_RETRYCANCEL}\n \
@end itemize\n \
Default is MB_OK\n \
\n \
Returns a value @var{rv}: \n \
@table @asis\n \
@item 1\n \
User Clicked OK\n \
@item 2\n \
User Clicked Cancel\n \
@item 3\n \
User Clicked Abort\n \
i@item 4\n \
User Clicked Retry\n \
@item 5\n \
User Clicked Ignore\n \
@item 6 \n \
User Clicked Yes\n \
@item 7\n \
User Clicked No\n \
@item 10 \n \
User Clicked Try Again\n \
@item 11\n \
User Clicked Continue\n \
@end table\n \
@end deftypefn")
{
#ifndef USING_WINDOWS
  error ("win32api: Your system doesn't support the COM interface");
  return octave_value ();
#else
  int nargin = args.length ();
  octave_value_list retval;
  if ( nargin < 2 || nargin >=4 ||
       !args (0).is_string () ||
       !args (1).is_string () )
    {
      print_usage ();
      return retval;
    }

  std::string titleparam = args(0).string_value ();
  std::string textparam  = args(1).string_value ();
  int  boxtype = 0;
  if (nargin == 3) 
    {
      if (!args(2).is_string ())
        boxtype = (int) args (2).double_value ();
      else
        {
          std::string mboxtype= args(2).string_value ();
          if (mboxtype == "MB_OK")
            boxtype=0;
          else if (mboxtype == "MB_OKCANCEL")
            boxtype=1;
          else if (mboxtype == "MB_ABORTRETRYIGNORE")
            boxtype=2;
          else if (mboxtype == "MB_YESNOCANCEL")
            boxtype=3;
          else if (mboxtype == "MB_YESNO")
            boxtype=4;
          else if (mboxtype == "MB_RETRYCANCEL")
            boxtype=4;
          else
            {
              error ("mboxtype does not correspond to a registed MB type");
              return retval;
            }
        }
    }

  int rv= win32_MessageBox (textparam.c_str (), titleparam.c_str (), boxtype);

  retval (0)= (double) rv;
  return retval;
#endif
}

// PKG_ADD: autoload ("win32_ReadRegistry", "win32api.oct");
DEFUN_DLD (win32_ReadRegistry, args, ,
  "-*- texinfo -*-\n \
@deftypefn {Loadable Function} {[ @var{rv}, @var{code} ] =} win32_ReadRegistry (@var{key}, @var{subkey}, @var{value})\n \
\n \
Read a value from the Windows registry.\n \
\n \
Example:\n \
@example\n \
key='SOFTWARE\\\\Cygnus Solutions\\\\Cygwin\\\\mounts v2';\n \
win32_ReadRegistry('HKLM',key,'cygdrive prefix')\n \
@end example\n \
\n \
key must be one of the following strings:\n \
@table @asis\n \
@item HKCR\n \
HKEY_CLASSES_ROOT\n \
@item HKCU\n \
HKEY_CURRENT_USER\n \
@item HKLM\n \
HKEY_LOCAL_MACHINE\n \
@item HKU\n \
HKEY_USERS\n \
@end table\n \
\n \
@var{rv} is an octave string of the returned bytes.\n \
This is a natural format for REG_SZ data; however, \n \
if the registry data was in another format, REG_DWORD\n \
then the calling program will need to process them\n \
\n \
@var{code} is the success code. Values correspond to the\n \
codes in the winerror.h header file. The code of 0 is\n \
success, while other codes indicate failure\n \
In the case of failure, 'rv' will be empty\n \
@end deftypefn")
{
  octave_value_list retval;
#ifndef USING_WINDOWS
  error ("win32api: Your system doesn't support the COM interface");
#else
  int nargin = args.length();
  if ( nargin != 3 ||
       !args (0).is_string () ||
       !args (1).is_string () ||
       !args (2).is_string ())
    {
      print_usage ();
      return retval;
    }

  if (! win32_IsValidRootKey(args (0).string_value ().c_str ()))
    {
      error ("win32_ReadRegistry: invalid reg key");
      return retval;
    }

  char * key   = strdup (args (0).string_value ().c_str ());
  char * subkey= strdup (args (1).string_value ().c_str ());
  char * value = strdup (args (2).string_value ().c_str ());

  // call registry first time to get size and existance
  int buffer_sz=0;
  int type;
  int retcode= win32_ReadRegistry (key, subkey, value, NULL, &buffer_sz, &type);
  if (retcode != 0)
    {
      retval (0)= new Matrix (0,0);
      retval (1)= (double) retcode;
      error ("win32_ReadRegistry: error reading registry value");
    }
  else
    {
      char * buffer= new char[ buffer_sz ];
      int retcode= win32_ReadRegistry (key,subkey,value,buffer, &buffer_sz, &type);
      retval(0)= string_vector (buffer);
      retval(1)= (double) retcode;
      retval(2)= (double) buffer_sz;
      delete buffer;
    }

  free (key);
  free (subkey);
  free (value);
#endif
  return retval;
}

// PKG_ADD: autoload ("win32_WriteRegistry", "win32api.oct");
DEFUN_DLD (win32_WriteRegistry, args, ,
  "-*- texinfo -*-\n \
@deftypefn {Loadable Function} {@var{code} =} win32_WriteRegistry (@var{key}, @var{subkey}, @var{valuename}, @var{value})\n \
\n \
Write a value to the Windows registry.\n \
\n \
Example:\n \
@example\n \
key='test\\\\temp';\n \
win32_WriteRegistry('HKLM',key,'test_value', 0)\n \
@end example\n \
\n \
key must be one of the following strings:\n \
@table @asis\n \
@item HKCR\n \
HKEY_CLASSES_ROOT\n \
@item HKCU\n \
HKEY_CURRENT_USER\n \
@item HKLM\n \
HKEY_LOCAL_MACHINE\n \
@item HKU\n \
HKEY_USERS\n \
@end table\n \
\n \
@var{subkey} is the subkey to the registry value.\n \
\n \
@var{valuename} is the name of the value to write to the registry.\n \
\n \
@var{value} is the value to write. It must be a a string or an integer value.\n \
\n \
@var{code} is the success code. Values correspond to the\n \
codes in the winerror.h header file. The code of 0 is\n \
success, while other codes indicate failure\n \
@end deftypefn")
{
  octave_value_list retval;
#ifndef USING_WINDOWS
  error ("win32api: Your system doesn't support the COM interface");
#else
  int nargin = args.length();
  if ( nargin != 4 ||
       !args (0).is_string () ||
       !args (1).is_string () ||
       !args (2).is_string ())
    {
      print_usage ();
      return retval;
    }

  if (! win32_IsValidRootKey(args (0).string_value ().c_str ()))
    {
      error ("win32_ReadRegistry: invalid reg key");
      return retval;
    }

  char * key   = strdup (args (0).string_value ().c_str ());
  char * subkey= strdup (args (1).string_value ().c_str ());
  char * value = strdup (args (2).string_value ().c_str ());

  // call registry first time to get size and existance
  int buffer_sz=0;
  int type;

  octave_value v = args(3);

  char * buffer = octave_to_win32_reg (v, &buffer_sz, &type);

  if (!buffer)
    {
      error ("win32_ReadRegistry: unsupported type to registry conversion");
    }
  else
    {
      int retcode = win32_WriteRegistry (key, subkey, value, buffer, buffer_sz, type);
      retval (0)= (double) retcode;

      free(buffer);
    }

  free (key);
  free (subkey);
  free (value);
#endif
  return retval;
}


// PKG_ADD: autoload ("win32_RegEnumValue", "win32api.oct");
DEFUN_DLD (win32_RegEnumValue, args, ,
  "-*- texinfo -*-\n \
@deftypefn {Loadable Function} {[ @var{rv}, @var{code} ] =} win32_RegEnumValue (@var{key}, @var{subkey})\n \
\n \
Read value names from from the Windows registry.\n \
\n \
Example:\n \
@example\n \
key='SOFTWARE\\\\Cygnus Solutions\\\\Cygwin\\\\mounts v2';\n \
win32_RegEnumValue('HKLM',key)\n \
@end example\n \
\n \
key must be one of the following strings:\n \
@table @asis\n \
@item HKCR\n \
HKEY_CLASSES_ROOT\n \
@item HKCU\n \
HKEY_CURRENT_USER\n \
@item HKLM\n \
HKEY_LOCAL_MACHINE\n \
@item HKU\n \
HKEY_USERS\n \
@end table\n \
\n \
@var{rv} is an array of value strings for the name of values\n \
for a given key and subkey.\n \
\n \
@var{code} is the success code. Values correspond to the\n \
codes in the winerror.h header file. The code of 0 is\n \
success, while other codes indicate failure\n \
In the case of failure, 'rv' will be empty\n \
\n \
@seealso{winqueryreg}\n \
@end deftypefn")
{
  octave_value_list retval;
#ifndef USING_WINDOWS
  error ("win32api: Your system doesn't support the COM interface");
#else
  int nargin = args.length();
  if ( nargin != 2 ||
       !args (0).is_string () ||
       !args (1).is_string ())
    {
      print_usage ();
      return retval;
    }

  if (! win32_IsValidRootKey(args (0).string_value ().c_str ()))
    {
      error ("win32_RegEnumValue: invalid reg key");
      return retval;
    }

  char * key   = strdup (args (0).string_value ().c_str ());
  char * subkey= strdup (args (1).string_value ().c_str ());

  std::list<std::string> fields;

  int retcode = win32_ScanRegistry (key, subkey, fields);
  if (retcode != 0)
    {
      retval (0) = new Matrix (0,0);
      error ("win32_RegEnumValue: error reading registry values");
    }
  else
    {
      Cell fieldnames (dim_vector (1, fields.size ()));
      int i;
      std::list<std::string>::const_iterator iterator;
      for (i=0, iterator = fields.begin ();
           iterator != fields.end (); ++iterator, ++i)
        {
          fieldnames (i) = *iterator;
        }
      retval (0) = octave_value (fieldnames);
    }

  retval (1) = (double) retcode;

  free (key);
  free (subkey);
#endif
  return retval;
}

// PKG_ADD: autoload ("win32_RegEnumKey", "win32api.oct");
DEFUN_DLD (win32_RegEnumKey, args, ,
  "-*- texinfo -*-\n \
@deftypefn {Loadable Function} {[ @var{rv}, @var{code} ] =} win32_RegEnumKey (@var{key}, @var{subkey})\n \
\n \
Read the keys of a given subkey from the Windows registry.\n \
\n \
Example:\n \
@example\n \
key='SOFTWARE\\\\Cygnus Solutions\\\\Cygwin\\\\mounts v2';\n \
win32_RegEnumKey('HKLM',key)\n \
@end example\n \
\n \
key must be one of the following strings:\n \
@table @asis\n \
@item HKCR\n \
HKEY_CLASSES_ROOT\n \
@item HKCU\n \
HKEY_CURRENT_USER\n \
@item HKLM\n \
HKEY_LOCAL_MACHINE\n \
@item HKU\n \
HKEY_USERS\n \
@end table\n \
\n \
@var{rv} is an array of value strings for the name of keys\n \
for a given key and subkey.\n \
\n \
@var{code} is the success code. Values correspond to the\n \
codes in the winerror.h header file. The code of 0 is\n \
success, while other codes indicate failure\n \
In the case of failure, 'rv' will be empty\n \
\n \
@seealso{winqueryreg}\n \
@end deftypefn")
{
  octave_value_list retval;
#ifndef USING_WINDOWS
  error ("win32api: Your system doesn't support the COM interface");
#else
  int nargin = args.length();
  if ( nargin != 2 ||
       !args (0).is_string () ||
       !args (1).is_string ())
    {
      print_usage ();
      return retval;
    }

  if (! win32_IsValidRootKey(args (0).string_value ().c_str ()))
    {
      error ("win32_RegEnumKey: invalid reg key");
      return retval;
    }

  char * key   = strdup (args (0).string_value ().c_str ());
  char * subkey= strdup (args (1).string_value ().c_str ());

  std::list<std::string> fields;

  int retcode = win32_ScanRegistryKeys (key, subkey, fields);
  if (retcode != 0)
    {
      retval (0) = new Matrix (0,0);
      error ("win32_RegEnumKey: error reading registry values");
    }
  else
    {
      Cell fieldnames (dim_vector (1, fields.size ()));
      int i;
      std::list<std::string>::const_iterator iterator;
      for (i=0, iterator = fields.begin ();
           iterator != fields.end (); ++iterator, ++i)
        {
          fieldnames (i) = *iterator;
        }
      retval (0) = octave_value (fieldnames);
    }

  retval (1) = (double) retcode;

  free (key);
  free (subkey);
#endif
  return retval;
}


#if 0
%!testif HAVE_WINDOWS_H
%! fail ("win32_ReadRegistry('X','Y','Z')", "invalid reg key");
%! fail ("win32_ReadRegistry('HKLM','Y','Z')", "error reading registry value");

%!testif HAVE_WINDOWS_H
%! val = win32_ReadRegistry('HKLM', 'SOFTWARE\Microsoft\Windows NT\CurrentVersion', "CurrentVersion");
%! assert (ischar(val));
%! assert (length(val) > 0);

%!testif HAVE_WINDOWS_H
%! fail ("win32_RegEnumValue('X','Y')", "invalid reg key");
%! fail ("win32_RegEnumValue('HKLM','Y')", "error reading registry values");

%!testif HAVE_WINDOWS_H
%! val = win32_RegEnumValue('HKLM', 'SOFTWARE\Microsoft\Windows NT\CurrentVersion');
%! assert (iscellstr(val));
%! qval = winqueryreg("name", 'HKLM', 'SOFTWARE\Microsoft\Windows NT\CurrentVersion');
%! assert (val, qval);

%!testif HAVE_WINDOWS_H
%! fail ("win32_RegEnumKey('X','Y')", "invalid reg key");
%! fail ("win32_RegEnumKey('HKLM','Y')", "error reading registry values");

%!testif HAVE_WINDOWS_H
%! val = win32_RegEnumKey('HKLM', 'SOFTWARE\Microsoft\Windows NT\CurrentVersion');
%! assert (iscellstr(val));

%!testif HAVE_WINDOWS_H
%! fail ("win32_WriteRegistry('HKCX','Environment','test_value', 'x')", "invalid reg key");
%! err = win32_WriteRegistry('HKCU','Environment','test_value', 0);
%! assert(err, 0);
%! qval = winqueryreg('HKCU', 'Environment', 'test_value');
%! assert(qval, int32(0));
%! err = win32_WriteRegistry('HKCU','Environment','test_value', 1);
%! qval = winqueryreg('HKCU', 'Environment', 'test_value');
%! assert(qval, int32(1));
%! err = win32_WriteRegistry('HKCU','Environment','test_value', "string");
%! qval = winqueryreg('HKCU', 'Environment', 'test_value');
%! assert(strcmp(qval, "string"));
%! err = win32_WriteRegistry('HKCU','Environment\\\\Notvalid','test_value', "string");
%! assert(err != 0);
%! fail ("win32_WriteRegistry('HKCU','Environment','test_value', {})", "unsupported type to registry conversion");
#endif
