function output = get (varargin)

  output = com_get (varargin{:});

endfunction

%!testif HAVE_WINDOWS_H
%! wshell = actxserver ("WScript.Shell");
%! assert(wshell.CurrentDirectory, get(wshell, "CurrentDirectory"));
%! delete (wshell)


