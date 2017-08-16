function release (varargin)

  com_release (varargin{:});

endfunction

%!testif HAVE_WINDOWS_H
%! wshell = actxserver ("WScript.Shell");
%! release (wshell);
%! delete (wshell)


