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
        print_usage("win32_MessageBox");
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
