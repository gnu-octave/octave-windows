function output = set (varargin)

  output = com_set (varargin{:});

endfunction

%!testif HAVE_WINDOWS_H
%! wshell = actxserver ("WScript.Shell");
%! currdir = pwd ();
%! set(wshell, "CurrentDirectory", getenv("SYSTEMROOT"));
%! assert(pwd(), getenv("SYSTEMROOT"));
%! cd(currdir);
%! delete (wshell)


