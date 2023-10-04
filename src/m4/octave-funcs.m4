# Copyright (C) 2020 John Donoghue <john.donoghue@ieee.org>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, see <http://www.gnu.org/licenses/>.

# Attempt to verify if the function NAME exists in octave.
# 
# For a given name, it will search in order for NAME.m,
# NAME.oct, reference to NAME in PKG_ADD and then if
# still not found, attempt to compile a call to the function
# to determine if it is a inbuild function.
#
# If found it will create OCTAVE_HAS_XXXXXXX subst value.
#
# arguments of OCTAVE_HAS_FUNCTION
#
# $1: function to check for
# $2: code to run if found
# $3: code to run if not found

AC_DEFUN([OCTAVE_HAS_FUNCTION], [
AC_REQUIRE([AC_PROG_GREP])
AC_REQUIRE([AC_PROG_SED])

if test "X${IGNORE_MINGW_PATH_MODIFICATION}" == "X"; then
  of_MSYSTEM="${MSYSTEM}"
else
  of_MSYSTEM="none"
fi

if test -z "$MKOCTFILE"; then
  AC_CHECK_PROG(MKOCTFILE,mkoctfile,mkoctfile)
fi
if test -z "$OCTAVE_CONFIG"; then
  AC_CHECK_PROG(OCTAVE_CONFIG,octave-config,octave-config)
fi
if test -z "$OCTAVE_CONFIG"; then
  AS_VAR_SET(OCTAVE_CONFIG,$MKOCTFILE)
fi
case X$of_MSYSTEM in
  XMINGW*)
    MKOCTFILE="m4_patsubst($MKOCTFILE, '\\\\\ ','?')"
    OCTAVE_CONFIG="m4_patsubst($OCTAVE_CONFIG,'\\\\\ ','?')"
    ;;
  *)
    ;;
esac

AC_MSG_CHECKING([octave for $1])
AS_VAR_PUSHDEF([ac_var], m4_toupper(octave_have_$1))
AS_VAR_SET(of_ac_tmp_m_path,`$OCTAVE_CONFIG -p FCNFILEDIR`)
AS_VAR_SET(of_ac_tmp_oct_path,`$OCTAVE_CONFIG -p OCTFILEDIR`)
if test m4_index($1, [.]) != -1; then
  # a name like a namespace
  of_ac_tmp="m4_translit($1,[.],[/])"
  _AS_ECHO_LOG([try to find $of_ac_tmp.m])
  #of_ac_tmp=`find "$of_ac_tmp_m_path" -type f -path "*/$of_ac_tmp.m" | head -1`
  AS_VAR_SET(of_ac_tmp,`find "$of_ac_tmp_m_path" -type f -path "*/$of_ac_tmp.m" | head -1`)
else
  # a .m file ?
  _AS_ECHO_LOG([try to find $1.m])
  AS_VAR_SET(of_ac_tmp,`find "$of_ac_tmp_m_path" -type f -name $1.m | $GREP -v private | head -1`)
  # a .oct file
  if test -z "$of_ac_tmp"; then
   _AS_ECHO_LOG([try to find $1.oct])
   AS_VAR_SET(of_ac_tmp,`find "$of_ac_tmp_oct_path" -type f -name $1.oct | head -1`)
  fi
  # a file referenced from PKG_ADD
  if test -z "$of_ac_tmp"; then
   _AS_ECHO_LOG([try to find $1 in PKG_ADD])
   AS_VAR_SET(of_ac_tmp, `find "$of_ac_tmp_oct_path" -type f -name PKG_ADD -exec grep -il ".*\"$1\".*" {} \; -print | head -1`)
  fi
  # a builtin ?
  if test -z "$of_ac_tmp"; then
    _AS_ECHO_LOG([try to find $1 as builtin])
    AC_LANG_PUSH([C++])
    AC_REQUIRE_CPP()
    save_of_ac_tmp_CXX="$CXX"
    save_of_ac_tmp_LIBS="$LIBS"
    save_of_ac_tmp_LDFLAGS="$LDFLAGS"
    save_of_ac_tmp_CXXFLAGS="$CXXFLAGS"
    CXX=`$MKOCTFILE -p CXX`

    of_OCTINCLUDEDIR="`$MKOCTFILE -p OCTINCLUDEDIR`/.."
    of_OCTLIBDIR=`$MKOCTFILE -p OCTLIBDIR`
    case X$of_MSYSTEM in
      XMINGW64*)
        of_OCTAVE_HOME=`${MKOCTFILE} -p OCTAVE_HOME | $SED 's,\\\\,/,g'`
        # change \ to / and replace octave home part with mingw part
        of_OCTINCLUDEDIR=`echo $of_OCTINCLUDEDIR | $SED -e 's,\\\\,/,g' -e "s,${of_OCTAVE_HOME},/mingw64,g"`
        of_OCTLIBDIR=`echo $of_OCTLIBDIR | $SED -e 's,\\\\,/,g' -e "s,${of_OCTAVE_HOME},/mingw64,g"`
        ;;
      XMINGW32*)
        of_OCTAVE_HOME=`${MKOCTFILE} -p OCTAVE_HOME | $SED 's,\\\\,/,g'`
        # change \ to / and replace octave home part with mingw part
        of_OCTINCLUDEDIR=`echo $of_OCTINCLUDEDIR | $SED -e 's,\\\\,/,g' -e "s,${of_OCTAVE_HOME},/mingw32,g"`
        of_OCTLIBDIR=`echo of_$OCTLIBDIR | $SED -e 's,\\\\,/,g -e "s,${of_OCTAVE_HOME},/mingw32,g"'`
        ;;
      *)
        ;;
    esac

    CXXFLAGS="-I$of_OCTINCLUDEDIR $CXXFLAGS"
    LDFLAGS="-L$of_OCTLIBDIR $LDFLAGS"
    LIBS="`$MKOCTFILE -p OCTAVE_LIBS` $LIBS"
    AC_LINK_IFELSE(
      [AC_LANG_PROGRAM(
        [[#include <octave/oct.h>]
         [#include <octave/builtin-defun-decls.h>]
         [using namespace octave;]],
        [F$1()])],
      [of_ac_tmp=builtin],
      []
    )
    CXX=$save_of_ac_tmp_CXX
    LIBS=$save_of_ac_tmp_LIBS
    CXXFLAGS=$save_of_ac_tmp_CXXFLAGS
    LDFLAGS=$save_of_ac_tmp_LDFLAGS
    AC_LANG_POP([C++])
  fi
fi

if test -n "$of_ac_tmp"; then
 _AS_ECHO_LOG([found $1 as $of_ac_tmp])
 AC_MSG_RESULT([yes])
 AC_SUBST(ac_var,[1])
 #AC_DEFINE(ac_var,[],[Defined if $1 exists])
 $2
else
 _AS_ECHO_LOG([could not find $1])
 AC_MSG_RESULT([no])
 $3
fi
AS_VAR_POPDEF([ac_var])
]) 
