/*
 * winopen
 * 
 * Copyright (C) 2019 John Donoghue <john.donoghue@ieee.org>
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

DEFUN_DLD (winopen, args, ,
  "-*- texinfo -*-\n \
@deftypefn {Loadable Function} winopen (@var{name})\n \
\n \
Open the file or directory @var{name} in the windows registered\n \
application for the file, using shell open command.\n \
\n \
Examples:\n \
\n \
Open file document.docx in the docx viewer:\n \
@example\n \
winopen (\"document.docx\");\n \
@end example\n \
\n \
Open the current directory in explorer:\n \
@example\n \
winopen (pwd);\n \
@end example\n \
\n \
\n \
@end deftypefn")
{
  octave_value_list retval;
#ifndef USING_WINDOWS
  error ("winopen: Your system doesn't support the windows interface");
#else
  int nargin = args.length();
  if ( nargin != 1)
    {
      print_usage ();
      return retval;
    }
  if ( !args (0).is_string ())
    {
      error ("winopen: expected a string filename or directory");
      return retval;
    }

  ShellExecute (
    NULL, "open", args(0).string_value().c_str(),
    NULL, NULL, SW_SHOWDEFAULT
  );
#endif
  return retval;
}
