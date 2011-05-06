/* configure.ac

Copyright (C) INRIA, 2008, 2009, 2010, 2011

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
AC_INIT(mpc, 0.9.1-dev, mpc-discuss@lists.gforge.inria.fr)
AC_CONFIG_SRCDIR([src/mpc-impl.h])
AC_CONFIG_HEADER([config.h])

AM_INIT_AUTOMAKE([1.9 -Wall -Werror])
AM_MAINTAINER_MODE

AC_CANONICAL_HOST
AC_CONFIG_MACRO_DIR([m4])

# Extra arguments to configure
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
AC_ARG_ENABLE([logging],
             [AC_HELP_STRING([--enable-logging],
                             [enable logging of function calls to stderr (default = no)])],
             [case $enableval in
                 yes) AS_IF([test "x$enable_shared" = "xno"],
                            AC_MSG_ERROR([Logging works only with shared libraries; do not mix --enable-logging and --disable-shared])) ;;
                 no)  ;;
                 *)   AC_MSG_ERROR([Bad value for --enable-logging: Use yes or no]) ;;
              esac
             ]
             )


# Setup CC and CFLAGS
AC_PROG_CC
AC_LANG(C)

# Set up LibTool
AC_PROG_LIBTOOL

# Check for user specification of CC or CFLAGS
if test -n "$CFLAGS" || test -n "$CC" ; then
   user_redefine_cc=yes
fi

# Check GMP Header
AC_MSG_CHECKING(for gmp.h)
AC_COMPILE_IFELSE([AC_LANG_SOURCE([[
#include "gmp.h"
]])],[AC_MSG_RESULT(yes)],[
 AC_MSG_RESULT(no)
 AC_MSG_ERROR([gmp.h can't be found, or is unusable.])
])

# Check for GMP CFLAGS in gmp.h
if test -z "$user_redefine_cc" ; then
   MPC_GMP_CC_CFLAGS
fi


# Configs for Windows DLLs
case $host in
  *-*-cygwin* | *-*-mingw* | *-*-pw32* | *-*-os2*)
     MPC_WINDOWS
esac


# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([complex.h locale.h inttypes.h stdint.h limits.h unistd.h sys/time.h])
AC_HEADER_TIME

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_SIZE_T

# Checks for libraries.
AC_CHECK_HEADERS(
   [complex.h],
   [
   AC_MSG_CHECKING(whether creal and cimag can be used without libm)
   AC_LINK_IFELSE(
      [AC_LANG_PROGRAM(
         [[#include <complex.h>]],
         [[complex double x = 1.0 + 2.0 * I; return (creal (x) + cimag (x));]]
      )],
      [AC_MSG_RESULT([yes])],
      [
         AC_MSG_RESULT([no])
         AC_CHECK_LIB([m], [creal])
      ]
      )
   ]
   )

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

# Check for a recent GMP
AC_MSG_CHECKING(for recent GMP)
AC_COMPILE_IFELSE(
        [AC_LANG_SOURCE(
        [[
#include "gmp.h"
#if (__GNU_MP_VERSION*100 + __GNU_MP_VERSION_MINOR*10 + __GNU_MP_VERSION_PATCHLEVEL < 432)
# error "Minimal GMP version is 4.3.2"
error
#endif
        ]])],
        [AC_MSG_RESULT(yes)],
        [
        AC_MSG_RESULT(no)
        AC_MSG_ERROR([GMP version >= 4.3.2 required])
        ])

# Check for a recent MPFR: we require MPFR 2.4.2 so that the tests
# in log.dat pass
AC_MSG_CHECKING(for recent MPFR)
AC_COMPILE_IFELSE(
        [AC_LANG_SOURCE(
        [[
#include "mpfr.h"
#if (MPFR_VERSION < MPFR_VERSION_NUM (2,4,2))
# error "Minimal MPFR version is 2.4.2"
error
#endif
        ]])],
        [AC_MSG_RESULT(yes)],
        [
         AC_MSG_RESULT(no)
         AC_MSG_ERROR([MPFR version >= 2.4.2 required])
         ])

# Check for logging feature
AS_IF([test "x$enable_logging" = "xyes"],
       [AC_CHECK_HEADERS([dlfcn.h])
        AC_CHECK_LIB([dl],[dlsym],
           [
            AC_DEFINE(HAVE_LIBDL, 1, [Library dl present])
            LIBS="-ldl $LIBS"
            AC_TYPE_INTPTR_T
            AC_LIBOBJ([logging])
            AC_SUBST([MPC_LOG_H],[src/mpc-log.h])
           ],
           [AC_MSG_ERROR([Library dl not found, logging impossible])])
       ]
     )

# Checks if compiler accepts warning flags.
MPC_PROG_CC_WARNINGCFLAGS([WARNINGCFLAGS])

# Checks for gcc version, result in GCC_VERSION inside config.log, for instance
AX_GCC_VERSION
AC_DEFINE_UNQUOTED([MPC_GCC], ["$GCC"], [Gcc yes or no])
AC_DEFINE_UNQUOTED([MPC_CC], ["$CC"], [C compiler])
AC_DEFINE_UNQUOTED([MPC_GCC_VERSION], ["$GCC_VERSION"], [Version of gcc])

# Checks for library functions with warning flags.
old_cflags=$CFLAGS
CFLAGS="$CFLAGS $WARNINGCFLAGS"
AC_CHECK_FUNCS([gettimeofday localeconv setlocale])
AC_CHECK_FUNCS([dup dup2 fileno],,
        [AC_DEFINE([NO_STREAM_REDIRECTION],1,[Do not check mpc_out_str on stdout])])
CFLAGS=$old_cflags

AC_CONFIG_FILES([Makefile src/Makefile tests/Makefile doc/Makefile])
AC_OUTPUT
