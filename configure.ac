/* configure.ac

Copyright (C) 2008 Andreas Enge, Philippe Th\'eveny

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

AC_PREREQ(2.59)
AC_INIT(mpc, 0.5.1, mpc-discuss@lists.gforge.inria.fr)
AC_CONFIG_SRCDIR([src/mpc-impl.h])
AC_CONFIG_HEADER([config.h])

AM_INIT_AUTOMAKE([1.9 -Wall -Werror])
AM_MAINTAINER_MODE

dnl Extra arguments to configure
AC_ARG_WITH([mpfr],
            [AC_HELP_STRING([--with-mpfr=DIR],
                           [MPFR install directory])],
            [
            CPPFLAGS="-I$withval/include $CPPFLAGS"
            LDFLAGS="-L$withval/lib $LDFLAGS"
            ])
AC_ARG_WITH([gmp],
            [AC_HELP_STRING([--with-gmp=DIR],
                           [GMP install directory])],
            [
            CPPFLAGS="-I$withval/include $CPPFLAGS"
            LDFLAGS="-L$withval/lib $LDFLAGS"
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

# Checks for programs.
AC_PROG_CC
AC_PROG_LIBTOOL

AC_PATH_PROG([SED], [sed], no)
if test "x$SED" = "xno"; then
   AC_MSG_ERROR([program sed needed])
fi

# Checks for libraries.
AC_CHECK_LIB([gmp], [__gmpz_init],
             [LIBS="-lgmp $LIBS"],
             [AC_MSG_ERROR([libgmp not found or uses a different ABI.])])
AC_CHECK_LIB([mpfr], [mpfr_clear],
             [LIBS="-lmpfr $LIBS"],
             [AC_MSG_ERROR([libmpfr not found or uses a different ABI.])])

# Check for a recent MPFR
AC_MSG_CHECKING(for recent MPFR)
AC_COMPILE_IFELSE([AC_LANG_SOURCE([[
#include "mpfr.h"
#if (MPFR_VERSION_MAJOR*100 + MPFR_VERSION_MINOR*10 < 230)
# error "min MPFR version is 2.3.0"
error
#endif
]])],[AC_MSG_RESULT(yes)],[
 AC_MSG_RESULT(no)
 AC_MSG_ERROR([MPFR 2.3.0 min required])
])

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([limits.h stdlib.h sys/time.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_SIZE_T
AC_HEADER_TIME

# Checks for library functions.
AC_CHECK_FUNCS([gettimeofday])

# Checks for CC flags.
# Modification PT: move in the last place so as to avoid test failure in
# AC_C_CONST due to additional flags "-Wall -Werror"

# Variadic macros (used by logging macros) were introduced in C99
AX_C_CHECK_FLAG(-ansi,,,CFLAGS="$CFLAGS -ansi",)
#AX_C_CHECK_FLAG(-std=c99,,,CFLAGS="$CFLAGS -std=c99",)

AX_C_CHECK_FLAG(-pedantic,,,CFLAGS="$CFLAGS -pedantic",)
AX_C_CHECK_FLAG(-Wall,,,CFLAGS="$CFLAGS -Wall",)
AX_C_CHECK_FLAG(-Wextra,,,CFLAGS="$CFLAGS -Wextra",)
AX_C_CHECK_FLAG(-Werror,,,CFLAGS="$CFLAGS -Werror",)


AC_CONFIG_FILES([Makefile src/Makefile tests/Makefile doc/Makefile])
AC_OUTPUT
