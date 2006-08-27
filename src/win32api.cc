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

#include <octave/oct.h>

DEFUN_DLD (win32_MessageBox, args, ,
           "rv= win32_MessageBox (...)\n"
           "\n"
           "Usage:\n"
           "   win32_MessageBox( 'title', 'text' )\n"
           "   win32_MessageBox( 'title', 'text', MboxType )\n"
           "\n"
           "MBoxType can be an integer or a string,\n"
           "   For integer values, consult <windows.h>\n"
           "   The following string values are recognized:\n"
           "       MB_OK\n"
           "       MB_OKCANCEL\n"
           "       MB_ABORTRETRYIGNORE\n"
           "       MB_YESNOCANCEL\n"
           "       MB_YESNO\n"
           "       MB_RETRYCANCEL\n"
           "   Default is MB_OK\n"
           "Output values are:\n"
           "       User Clicked OK         1\n"
           "       User Clicked Cancel     2\n"
           "       User Clicked Abort      3\n"
           "       User Clicked Retry      4\n"
           "       User Clicked Ignore     5\n"
           "       User Clicked Yes        6\n"
           "       User Clicked No         7\n"
           "       User Clicked Try Again 10\n"
           "       User Clicked Continue  11\n"
          )
{
    int nargin = args.length();
    octave_value_list retval;
    if ( nargin < 2 || nargin >=4 ||
         !args(0).is_string() ||
         !args(1).is_string() 
       ) {
        print_usage ();
        return retval;
    }

    std::string titleparam = args(0).string_value();
    std::string textparam  = args(1).string_value();
    int  boxtype =0;
    if (nargin==3) 
    {
        if (!args(2).is_string() )
            boxtype = (int) args(2).double_value();
        else {
            std::string mboxtype= args(2).string_value();
            if (mboxtype == "MB_OK")               boxtype=0;
            else
            if (mboxtype == "MB_OKCANCEL")         boxtype=1;
            else
            if (mboxtype == "MB_ABORTRETRYIGNORE") boxtype=2;
            else
            if (mboxtype == "MB_YESNOCANCEL")      boxtype=3;
            else
            if (mboxtype == "MB_YESNO")            boxtype=4;
            else
            if (mboxtype == "MB_RETRYCANCEL")      boxtype=4;
            else {
                error(
                 "mboxtype does not correspond to a registed MB type");
                return retval;
            }
        }
    }

    int rv=
    win32_MessageBox( textparam.c_str(), titleparam.c_str(), boxtype);

    retval(0)= (double) rv;
    return retval;
}

DEFUN_DLD (win32_ReadRegistry, args, ,
           "[rv,code]= win32_ReadRegistry (key,subkey,value)\n"
           "\n"
           "Usage:\n"
           "   key='SOFTWARE\\\\Cygnus Solutions\\\\Cygwin\\\\mounts v2';\n"
           "   win32_ReadRegistry('HKLM',key,'cygdrive prefix')\n"
           "\n"
           "key must be one of the following strings\n"
           "  HKCR  % -> HKEY_CLASSES_ROOT\n"
           "  HKCU  % -> HKEY_CURRENT_USER\n"
           "  HKLM  % -> HKEY_LOCAL_MACHINE\n"
           "  HKU   % -> HKEY_USERS\n"
           "\n"
           "'rv' is an octave string of the returned bytes.\n"
           "This is a natural format for REG_SZ data; however, \n"
           "if the registry data was in another format, REG_DWORD\n"
           "then the calling program will need to process them\n"
           "\n"
           "'code' is the success code. Values correspond to the\n"
           "codes in the winerror.h header file. The code of 0 is\n"
           "success, while other codes indicate failure\n"
           "In the case of failure, 'rv' will be empty\n"
          )
{
    octave_value_list retval;
    int nargin = args.length();
    if( nargin != 3 ||
        !args(0).is_string() ||
        !args(1).is_string() ||
        !args(2).is_string()
      ) {
        print_usage ();
        return retval;
    }

    const char * key   = args(0).string_value().c_str();
    const char * subkey= args(1).string_value().c_str();
    const char * value = args(2).string_value().c_str();

    // call registry first time to get size and existance
    int buffer_sz=0;
    int retcode=
    win32_ReadRegistry(key,subkey,value,NULL, &buffer_sz);
    if (retcode != 0) {
        retval(0)= new Matrix(0,0);
        retval(1)= (double) retcode;
        error("asdf");
    } else {
        char * buffer= new char[ buffer_sz ];
        int retcode=
        win32_ReadRegistry(key,subkey,value,buffer, &buffer_sz);
        retval(0)= string_vector( buffer );
        retval(1)= (double) retcode;
        retval(2)= (double) buffer_sz;
        delete buffer;
    }

    return retval;
}
