## Copyright (C) 2026 John Donoghue <john.donoghue@ieee.org>
##
## This file is part of Octave.
##
## Octave is free software: you can redistribute it and/or modify it
## under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
## Octave is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with Octave; see the file COPYING.  If not, see
## <https://www.gnu.org/licenses/>.

## -*- texinfo -*-
## @deftypefn  {} {@var{S} =} fieldnames (@var{comobj})
## A fieldnames override for octave_com_object objects.
##
## The function will return a list of property names in @var{S}.
##
## @seealso{com_get, get}
## @end deftypefn

function output = fieldnames (comobj)

  # TODO: add processing optional arg of "-full" 

  output = com_get (comobj);

endfunction

%!testif HAVE_WINDOWS_H
%! wshell = actxserver ("WScript.Shell");
%! f = fieldnames(wshell));
%! assert(!isempty(f));
%! delete (wshell)


