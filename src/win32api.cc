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
                    int  * buffer_sz
                  );

bool win32_IsValidRootKey(const char *key);

int
win32_ScanRegistry (const char *key,
                    const char *subkey,
                    std::list<std::string> &fields);

#include <octave/oct.h>
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
@end deftypefn"
          )
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

  if (win32_IsValidRootKey(args (0).string_value ().c_str ()))
    {
      error ("win32_ReadRegistry: invalid reg key");
      return retval;
    }

  char * key   = strdup (args (0).string_value ().c_str ());
  char * subkey= strdup (args (1).string_value ().c_str ());
  char * value = strdup (args (2).string_value ().c_str ());

  // call registry first time to get size and existance
  int buffer_sz=0;
  int retcode= win32_ReadRegistry (key, subkey, value, NULL, &buffer_sz);
  if (retcode != 0)
    {
      retval (0)= new Matrix (0,0);
      retval (1)= (double) retcode;
      error ("win32_ReadRegistry: error reading registry value");
    }
  else
    {
      char * buffer= new char[ buffer_sz ];
      int retcode= win32_ReadRegistry (key,subkey,value,buffer, &buffer_sz);
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

#ifndef OCTAVE__WINQUERYREG
// PKG_ADD: autoload ("winqueryreg", "win32api.oct");
DEFUN_DLD (winqueryreg, args, ,
  "-*- texinfo -*-\n \
@deftypefn {Loadable Function} {@var{valnames} =} winqueryreg ('name', @var{rootkey}, @var{subkey})\n \
@deftypefnx {Loadable Function} {@var{value} =} winqueryreg (@var{rootkey}, @var{subkey}, @var{valuename})\n \
@deftypefnx {Loadable Function} {@var{value} =} winqueryreg (@var{rootkey}, @var{subkey})\n \
\n \
Read a value from the Windows registry.\n \
\n \
@var{valuenames}= winqueryreg ('name', @var{rootkey}, @var{subkey}) will return the value names for the rootkey/subkey.\n \
\n \
@var{value} = winqueryreg (@var{rootkey}, @var{subkey}, @var{valuename}) will return the value of the valuename within the rootkey/subkey\n \
\n \
@var{value} = winqueryreg (@var{rootkey}, @var{subkey}) will return the default value (if it exists) of the rootkey/subkey\n \
\n \
rootkey must be one of the following strings:\n \
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
subkey is a string name representing the subkey path.\n \
\n \
@var{value} is an octave string of the returned bytes.\n \
This is a natural format for REG_SZ data; however, \n \
if the registry data was in another format, REG_DWORD\n \
then the calling program will need to process them\n \
\n \
@var{valuenames} is an array of octave strings of the value names within the registry key.\n \
\n \
Examples:\n \
\n \
Get a list of ket value names from the User\Environment registry:\n \
@example\n \
valuenames = winqueryreg ('name', 'HKEY_CURRENT_USER', 'Environment');\n \
@end example\n \
\n \
For each valuenames, display the value:\n \
@example\n \
for k = 1:length (valuenames)\n \
   setting = winqueryreg ('HKEY_CURRENT_USER', 'Environment', valuenames{k});\n \
   str = sprintf ('%s = %s', valuenames{k}, num2str(setting));\n \
   disp (str)\n \
endfor\n \
@end example\n \
\n \
@seealso{win32_ReadRegistry}\n \
\n \
@end deftypefn")
{
  octave_value_list retval;
#ifndef USING_WINDOWS
  error ("winqueryreg: Your system doesn't support the windows interface");
#else
  int nargin = args.length();
  if ( nargin != 2 && nargin != 3)
    {
      print_usage ();
      return retval;
    }
  if ( !args (0).is_string () ||
       !args (1).is_string ())
    {
      print_usage ();
      return retval;
    }
  if ( nargin == 2 &&
       strcmp(args (0).string_value ().c_str (), "name") == 0)
    {
      print_usage ();
      return retval;
    }
  if ( nargin == 3 &&
       !args (2).is_string ())
    {
      print_usage ();
      return retval;
    }

  if ((strcmp(args (0).string_value ().c_str (), "name") == 0 &&
       !win32_IsValidRootKey(args (1).string_value ().c_str ())) ||
      (win32_IsValidRootKey(args (0).string_value ().c_str ())))
    {
      error ("winqueryreg: invalid root key provided");
      return retval;
    }

  char * rootkey   = strdup (args (0).string_value ().c_str ());
  char * subkey= strdup (args (1).string_value ().c_str ());
  char * value = strdup (nargin != 3 ? "<null>" : args (2).string_value ().c_str ());

  if (0 == strcmp(rootkey, "name")) {
    // want enumerate the values
    char * tmp = rootkey;
    rootkey = subkey;
    subkey = value;
    value = tmp;

    std::list<std::string> fields;

    int retcode = win32_ScanRegistry (rootkey, subkey, fields);
    if (retcode != 0)
      {
        retval (0) = new Matrix (0,0);
        error ("winqueryreg: failed reading from registry");
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
  }
  // else check valid rootkey name
  else {

    // call registry first time to get size and existance
    int buffer_sz=0;
    int retcode= win32_ReadRegistry (rootkey, subkey, nargin == 3 ? value : NULL, NULL, &buffer_sz);
    if (retcode != 0)
      {
        retval (0) = new Matrix (0,0);
        error ("winqueryreg: failed reading registry");
      }
    else
      {
        char * buffer= new char[ buffer_sz + 1 ];
        int retcode= win32_ReadRegistry (rootkey,subkey,nargin == 3 ? value : NULL,buffer, &buffer_sz);
        retval (0) = string_vector (buffer);
        delete buffer;
      }
  }

  free (rootkey);
  free (subkey);
  free (value);
#endif
  return retval;
}
#endif

