/*
 * clipboard
 * 
 * Copyright (C) 2020-2026 John Donoghue <john.donoghue@ieee.org>
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

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#ifdef USING_WINDOWS
#include <windows.h>
#endif

static std::string
mat2str (const octave_value &ov)
{
  if (ov.ndims() > 2)
    {
      error ("Can not convert more than 2 dimensions");
      return "";
    }

  std::stringstream str;
  
  bool append_el = true;
  if(ov.is_uint8_type())
    str << "uint8(";
  else if(ov.is_uint16_type())
    str << "uint16(";
  else if(ov.is_uint32_type())
    str << "uint32(";
  else if(ov.is_int8_type())
    str << "int8(";
  else if(ov.is_int16_type())
    str << "int16(";
  else if(ov.is_int32_type())
    str << "int32(";
  else
    append_el = false;
  
  str << "[";

  Matrix mat=ov.matrix_value() ; //to matrix of doubles
  octave_idx_type nr = mat.rows();
  octave_idx_type nc = mat.cols();

  for (octave_idx_type r = 0; r < nr ;r++)
    {
      for (octave_idx_type c = 0; c < nc ;c++)
        {
            if(c != 0) str << " ";
            str << mat(r, c);
        }
      str << ";";
    }
  str << "]";
  if(append_el)
    str << ")";
  return str.str();
}

DEFUN_DLD (clipboard, args, ,
  "-*- texinfo -*-\n \
@deftypefn {} clipboard (@var{'copy'}, @var{data})\n \
@deftypefnx {} @var{txt} = clipboard (@var{'paste'})\n \
\n \
Insert or get data from the clipboard.\n \
\n \
@var{'copy'} or @var{'paste'} is the required operation to perform.\n \
where 'copy' will copy data to the clipboard, and paste will paste data from\n \
the clipboard to a variable.\n \
\n \
@var{data} is the data to copy to the clipboard.\n \
\n \
@var{txt} is the text from the clipboard or an empty string it it can  not convert to text..\n \
\n \
Examples:\n \
\n \
Copy a string to the clipboard:\n \
@example\n \
clipboard('copy', 'hello world');\n \
@end example\n \
\n \
Get a string from the clipboard:\n \
@example\n \
txt = clipboard('paste');\n \
@end example\n \
\n \
@end deftypefn")
{
  octave_value_list retval;
#ifndef USING_WINDOWS
  error ("clipboard: Your system doesn't support the windows interface");
#else
  int nargin = args.length();
  if ( nargin != 1 && nargin != 2)
    {
      print_usage ();
      return retval;
    }

  if ( !args (0).is_string ())
    {
      error ("clipboard: expected first argument to be a string.");
      return retval;
    }

  std::string op = args(0).string_value();

  if (op == "copy")
    {
       if (nargin < 2)
         {
           error ("clipboard: expected data input");
         }
       else if ( ! OpenClipboard(NULL))
         {
           error ("clipboard: could not open clipboard.");
         }
       else
         {
           std::string data = "";
   
           if (args(1).is_string())
             data = args(1).string_value ();
           else
             data = mat2str(args(1));

           HGLOBAL hglb = GlobalAlloc(GMEM_MOVEABLE, data.length()+1);

           if (hglb != NULL)
             {
               CHAR * ptr = (CHAR*)GlobalLock(hglb);
               if (ptr)
                 {
                   memcpy(ptr, data.c_str(), data.length());
                   ptr[data.length()] = 0;
                   GlobalUnlock(hglb);
                   EmptyClipboard();
                   SetClipboardData(CF_TEXT, hglb);
                 }
             }

           CloseClipboard();
         }
    }
  else if (op == "paste")
    {
      if (nargin != 1)
        {
          error ("clipboard: unexpected data input");
        }
      else if (!IsClipboardFormatAvailable(CF_TEXT))
        {
          retval(0) = octave_value(std::string(""));
        }
      else if ( ! OpenClipboard(NULL))
        {
          error ("clipboard: could not open clipboard.");
        }
      else
        {
          retval(0) = octave_value(std::string(""));

          HGLOBAL hglb = GetClipboardData(CF_TEXT);

          if (hglb !=  NULL)
            {
              CHAR *ptr = (CHAR*)GlobalLock(hglb);
              retval(0) = octave_value(std::string(ptr));
              GlobalUnlock(hglb);
            }

          CloseClipboard();
        }
    }
  else if (op == "pastespecial")
    {
      warning ("clipboard: pastespecial not implemented yet");
      retval(0) = octave_value(std::string(""));
    }
  else
    {
      error ("clipboard: unknown command '%s'", op.c_str());
    }
#endif
  return retval;
}

#if 0
%!xtest
%! # using xtest as windows may not allow us to get the clipboard
%! if (! __have_feature__ ("WINDOWS_H"))
%!   return;
%! endif
%! fail ("clipboard(1)", "expected first argument to be a string");
%! fail ("clipboard('invalid')", "unknown command");
%! fail ("clipboard('copy')", "expected data input");
%! fail ("clipboard('paste', 'hello')", "unexpected data input");

%!xtest
%! if (! __have_feature__ ("WINDOWS_H"))
%!   return;
%! endif
%! clipboard("copy", "hello");
%! txt = clipboard("paste");
%! assert(txt, "hello");

%!xtest
%! if (! __have_feature__ ("WINDOWS_H"))
%!   return;
%! endif
%! a = [1 2 3; 4 5 6];
%! clipboard("copy", a);
%! txt = clipboard("paste");
%! assert(eval(txt), a);
%!
%! clipboard("copy", uint16(a));
%! txt = clipboard("paste");
%! assert(eval(txt), uint16(a));

%!xtest
%! if (! __have_feature__ ("WINDOWS_H"))
%!   return;
%! endif
%! # external clipboard access
%! system("echo externalcopy|clip");
%! txt = clipboard("paste");
%! assert(txt(1:12), "externalcopy");
%!
%! # TODO: how prove copy to external

#endif
