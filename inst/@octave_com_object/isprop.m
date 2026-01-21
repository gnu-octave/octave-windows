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
## @deftypefn  {} {@var{S} =} isprop (@var{comobj}, @var{property})
## A isprop override for octave_com_object objects.
##
## For a string property, the function will return true or false if the 
## property exists for the com object.
##
## If property is a string array, the function will return an array of same size
## with true/false for each string in the array that is a property.
##
## @seealso{fieldnames}
## @end deftypefn

function output = isprop (comobj, property)

  props = com_get (comobj);

  output = ismember(property, props);

endfunction

%!testif HAVE_WINDOWS_H
%! wshell = actxserver ("WScript.Shell");
%! assert(isprop(wshell, "CurrentDirectory"), true);
%! assert(isprop(wshell, ["CurrentDirectory"; "NoMatch"]), [true; false]);
%! delete (wshell)


