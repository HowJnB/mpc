/* configure.ac

Copyright (C) 2008, 2009 Andreas Enge, Philippe Th\'eveny

This file is part of the MPC Library.

The MPC Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2.1 of the License, or (at your
option) any later version.

The MPC Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the MPC Library; see the file COPYING.LIB.  If not, write to
the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
MA 02111-1307, USA. */

#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT(mpc, 0.6-dev, mpc-discuss@lists.gforge.inria.fr)
AC_CONFIG_SRCDIR([src/mpc-impl.h])
AC_CONFIG_HEADER([config.h])

AM_INIT_AUTOMAKE([1.9 -Wall -Werror])
AM_MAINTAINER_MODE

AC_CANONICAL_HOST

dnl Extra arguments to configure
AC_ARG_WITH([mpfr_include],
            [AC_HELP_STRING([--with-mpfr-include=DIR],
                            [MPFR include directory])],
            [CPPFLAGS="-I$withval $CPPFLAGS"])
AC_ARG_WITH([mpfr_lib],
            [AC_HELP_STRING([--with-mpfr-lib=DIR],
                            [MPFR lib directory])],
            [LDFLAGS="-L$withval $LDFLAGS"])
AC_ARG_WITH([mpfr],
            [AC_HELP_STRING([--with-mpfr=DIR],
                           [MPFR install directory])],
            [
            if test -z "$with_mpfr_include" -a -z "$with_mpfr_lib" ; then
               CPPFLAGS="-I$withval/include $CPPFLAGS"
               LDFLAGS="-L$withval/lib $LDFLAGS"
            else
               AC_MSG_FAILURE([Do not use --with-mpfr and --with-mpfr-include/--with-mpfr-lib options simultaneously.])
            fi
            ])
AC_ARG_WITH([gmp_include],
            [AC_HELP_STRING([--with-gmp-include=DIR],
                            [GMP include directory])],
            [CPPFLAGS="-I$withval $CPPFLAGS"])
AC_ARG_WITH([gmp_lib],
            [AC_HELP_STRING([--with-gmp-lib=DIR],
                            [GMP lib directory])],
            [LDFLAGS="-L$withval $LDFLAGS"])
AC_ARG_WITH([gmp],
            [AC_HELP_STRING([--with-gmp=DIR],
                           [GMP install directory])],
            [
            if test -z "$with_gmp_lib" -a -z "$with_gmp_include" ; then
               CPPFLAGS="-I$withval/include $CPPFLAGS"
               LDFLAGS="-L$withval/lib $LDFLAGS"
            else
               AC_MSG_FAILURE([Do not use --with-gmp and --with-gmp-include/--with-gmp-lib options simultaneously.])
            fi
            ])
AC_ARG_VAR([MPC_USE_LOGGING], [defined if logging support wanted])
AC_ARG_ENABLE([logging],
              [AC_HELP_STRING([--enable-logging],
                              [enable MPC logging [default=no]])],
              [
               case $enableval in
                yes) AC_DEFINE([MPC_USE_LOGGING],1,[Logging support]) ;;
                no)  ;;
                *)   AC_MSG_ERROR([bad value for --enable-logging: yes or no])
                     ;;
               esac
               ])

dnl
dnl Setup CC and CFLAGS
dnl

dnl Check if user request its CC and CFLAGS
if test -n "$CFLAGS" || test -n "$CC" ; then
 user_redefine_cc=yes
fi
# Checks for programs.
AC_PROG_CC
AC_PROG_CPP
AC_PROG_EGREP
AC_PROG_SED
AC_LANG(C)

dnl Check for GMP CFLAGS in gmp.h
dnl copied from MPFR's configure.in
if test -z "$GMP_CFLAGS" -a -z "$user_redefine_cc" ; then
   AC_MSG_CHECKING(for CC and CFLAGS in gmp.h)
   # FIXME: Move this in aclocal ?
   # Get CC
   echo "#include \"gmp.h\"" >  conftest.c
   echo "MPC_OPTION __GMP_CC"           >> conftest.c
   GMP_CC="`$CPP $CPPFLAGS conftest.c 2> /dev/null | $EGREP MPC_OPTION | $SED -e 's/MPC_OPTION //g' | $SED -e 's/"//g'`"
   #Get CFLAGS
   echo "#include \"gmp.h\"" >  conftest.c
   echo "MPC_OPTION __GMP_CFLAGS"           >> conftest.c
   GMP_CFLAGS="`$CPP $CPPFLAGS conftest.c 2> /dev/null | $EGREP MPC_OPTION | $SED -e 's/MPC_OPTION //g'| $SED -e 's/"//g'`"
   rm -f conftest*
   if test "x$GMP_CFLAGS" = "x__GMP_CFLAGS" -o "x$GMP_CC" = "x__GMP_CC" ; then
      AC_MSG_RESULT(no)
      GMP_CFLAGS=
      GMP_CC=
   else
      AC_MSG_RESULT(yes [CC=$GMP_CC CFLAGS=$GMP_CFLAGS])
   fi
fi

dnl Check for GMP CFLAGS and CC (More optimized for the target)
dnl But they may be invalid, so we must check them first
dnl Don't redefine it if the user force them!
if test -n "$GMP_CFLAGS" && test -z "$user_redefine_cc" ; then
 old_cflags=$CFLAGS
 old_cc=$CC
 CFLAGS=$GMP_CFLAGS
 CC=$GMP_CC
 AC_MSG_CHECKING(for CC=$GMP_CC and CFLAGS=$GMP_CFLAGS)
 AC_RUN_IFELSE(AC_LANG_PROGRAM([[/*hello*/]],[[/*world*/]]), [
    AC_MSG_RESULT(yes)
  ], [
    AC_MSG_RESULT(no)
    CFLAGS=$old_cflags
    CC=$old_cc
  ], [
    AC_MSG_RESULT([can't test])
    CFLAGS=$old_cflags
    CC=$old_cc
  ])
  dnl CC may have change. Recheck for GCC.
  GCC=
  AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#ifndef __GNUC__
# error "GCC Not Found"
error
#endif
  ]])], [
  GCC=yes
  ])
fi

dnl Configs for Windows DLLs (copied from MPFR).
dnl libtool requires "-no-undefined" for win32 dll
dnl It also disables the tests involving the linking with LIBGMP if DLL
case $host in
  *-*-cygwin* | *-*-mingw* | *-*-pw32* | *-*-os2*)
   AC_MSG_CHECKING(for DLL/static GMP)
   if test "$enable_shared" = yes; then
     LDFLAGS="$LDFLAGS -no-undefined"
     AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include "gmp.h"
#if !__GMP_LIBGMP_DLL
# error "Dead man"
error
#endif
     ]], [[]])],[AC_MSG_RESULT(DLL)],[
  AC_MSG_RESULT(static)
  AC_MSG_ERROR([gmp.h isn't a DLL: use --enable-static --disable-shared]) ])
     AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include "mpfr.h"
#if !__GMP_LIBGMP_DLL
# error "Dead man"
error
#endif
     ]], [[]])],[AC_MSG_RESULT(DLL)],[
  AC_MSG_RESULT(static)
  AC_MSG_ERROR([gmp.h isn't a DLL: use --enable-static --disable-shared]) ])
   else
     AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include "gmp.h"
#if __GMP_LIBGMP_DLL
# error "Dead man"
error
#endif
     ]], [[]])],[AC_MSG_RESULT(static)],[
  AC_MSG_RESULT(DLL)
  AC_MSG_ERROR([gmp.h is a DLL: use --disable-static --enable-shared]) ])
  fi
  ;;
esac
dnl Finally set up LibTool
AC_PROG_LIBTOOL

# Checks for libraries.
AC_CHECK_LIB([gmp], [__gmpz_init],
             [LIBS="-lgmp $LIBS"],
             [AC_MSG_ERROR([libgmp not found or uses a different ABI.])])
AC_MSG_CHECKING(for MPFR)
LIBS="-lmpfr $LIBS"
AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
                [[#include "mpfr.h"]],
                [[mpfr_t x;  mpfr_init(x) ; mpfr_clear(x);]]
        )],
        [AC_MSG_RESULT(yes)],
        [
        AC_MSG_RESULT(no)
        AC_MSG_ERROR([libmpfr not found or uses a different ABI.])
        ])

# Check for a recent GMP: we need GMP 4.2 because we use the
# mp_get_memory_functions which appeared in GMP 4.2
AC_MSG_CHECKING(for recent GMP)
AC_COMPILE_IFELSE(
        [AC_LANG_SOURCE(
        [[
#include "gmp.h"
#if (__GNU_MP_VERSION*100 + __GNU_MP_VERSION_MINOR*10 < 420)
# error "min GMP version is 4.2"
error
#endif
        ]])],
        [AC_MSG_RESULT(yes)],
        [
        AC_MSG_RESULT(no)
        AC_MSG_ERROR([GMP version >= 4.2 required])
        ])

# Check for a recent MPFR: we require MPFR 2.3.1 because of a bug in
# mpfr_div_ui which makes mpc_div_ui returns the wrong sign of the zero part
# when applied on a pure real or pure imaginary parameter (see mpfr r4950)
AC_MSG_CHECKING(for recent MPFR)
AC_COMPILE_IFELSE(
        [AC_LANG_SOURCE(
        [[
#include "mpfr.h"
#if (MPFR_VERSION < MPFR_VERSION_NUM (2,3,1))
# error "min MPFR version is 2.3.1"
error
#endif
        ]])],
        [AC_MSG_RESULT(yes)],
        [
         AC_MSG_RESULT(no)
         AC_MSG_ERROR([MPFR version >= 2.3.1 required])
         ])

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([locale.h inttypes.h limits.h unistd.h sys/time.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_SIZE_T
AC_HEADER_TIME

# Checks if compiler accepts warning flags.
MPC_PROG_CC_WARNINGCFLAGS([WARNINGCFLAGS])

# Checks for library functions with warning flags.
old_cflags=$CFLAGS
CFLAGS="$CFLAGS $WARNINGCFLAGS"
AC_CHECK_FUNCS([gettimeofday])
AC_CHECK_FUNCS([dup dup2 fileno],,
        [AC_DEFINE([NO_STREAM_REDIRECTION],1,[Do not check mpc_out_str on stdout])])
CFLAGS=$old_cflags

AC_CONFIG_FILES([Makefile src/Makefile tests/Makefile doc/Makefile])
AC_OUTPUT
