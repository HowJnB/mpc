/* mpc.m4

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


#
# SYNOPSIS
#
#
# MPC_COMPLEX_H
#
# DESCRIPTION
#
# Check whether complex.h is usable; if yes, define HAVE_COMPLEX_H.
# On solaris, one also needs the libm; in that case, it is added to LIB,
# and HAVE_LIBM is defined.
#
AC_DEFUN([MPC_COMPLEX_H], [
   AC_CHECK_HEADER(
      [complex.h],
      [
         m4_define(
            [MPC_CONFTEST],
            [
               AC_LANG_PROGRAM(
                  [[#include <complex.h>]],
                  [[complex double x = 1.0 + 2.0 * I; return (creal (x) + cimag (x));]]
               )
            ]
         )

         AC_MSG_CHECKING(whether creal and cimag can be used without libm)
         AC_LINK_IFELSE(
            [MPC_CONFTEST],
            [
               AC_MSG_RESULT([yes])
               AC_DEFINE([HAVE_COMPLEX_H], [1], [complex.h present and usable])
            ],
            [
               AC_MSG_RESULT([no])
               AC_CHECK_LIB([m], [creal],
                  [
                     AC_MSG_CHECKING(whether creal and cimag can be used with libm)
                     AC_LINK_IFELSE(
                        [MPC_CONFTEST],
                        [
                           AC_MSG_RESULT([yes])
                           LIBS="-lm $LIBS"
                           AC_DEFINE([HAVE_LIBM], [1], [libm needed for creal])
                           AC_DEFINE([HAVE_COMPLEX_H], [1], [complex.h present and usable])
                        ],
                        [
                           AC_MSG_RESULT([no])
                        ]
                     )
                  ]
               )
            ]
         )
      ]
   )
])


#
# SYNOPSIS
#
#
# MPC_C_CHECK_FLAG([FLAG,ACCUMULATOR])
#
# DESCRIPTION
#
# Checks if the C compiler accepts the flag FLAG
# If yes, adds it to CFLAGS.

AC_DEFUN([MPC_C_CHECK_FLAG], [
   AX_C_CHECK_FLAG($1,,,[CFLAGS="$CFLAGS $1"])
])


#
# SYNOPSIS
#
#
# MPC_C_CHECK_WARNINGFLAGS
#
# DESCRIPTION
#
# For development version only: Checks if gcc accepts warning flags.
# Adds accepted ones to CFLAGS.
#
AC_DEFUN([MPC_C_CHECK_WARNINGCFLAGS], [
  AC_REQUIRE([AC_PROG_GREP])
  if echo $VERSION | grep -c dev >/dev/null 2>&1 ; then
    if test "x$GCC" = "xyes" -a "x$compiler" != "xicc" -a "x$compiler" != "xg++"; then
      case $host in
         *darwin*) ;;
         *) MPC_C_CHECK_FLAG(-D_FORTIFY_SOURCE=2,$1) ;;
      esac
      MPC_C_CHECK_FLAG(-g)
      MPC_C_CHECK_FLAG(-std=c99)
      MPC_C_CHECK_FLAG(-pedantic)
      MPC_C_CHECK_FLAG(-Wno-long-long)
      MPC_C_CHECK_FLAG(-Wall)
      MPC_C_CHECK_FLAG(-Wextra)
      MPC_C_CHECK_FLAG(-Werror)
      MPC_C_CHECK_FLAG(-Wdeclaration-after-statement)
      MPC_C_CHECK_FLAG(-Wundef)
      MPC_C_CHECK_FLAG(-Wshadow)
      MPC_C_CHECK_FLAG(-Wstrict-prototypes)
      MPC_C_CHECK_FLAG(-Wmissing-prototypes)
      MPC_C_CHECK_FLAG(-Wno-unused-value)
    fi
  fi
])


#
# SYNOPSIS
#
#
MPC_GMP_CC_CFLAGS
#
# DESCRIPTION
#
# Checks if CC and CFLAGS can be extracted from gmp.h
#
AC_DEFUN([MPC_GMP_CC_CFLAGS], [
   AC_MSG_CHECKING(for CC and CFLAGS in gmp.h)
   AC_PROG_CPP
   AC_PROG_EGREP
   AC_PROG_SED
   # Get CC
   echo "#include \"gmp.h\"" >  conftest.c
   echo "MPC_OPTION __GMP_CC"           >> conftest.c
   GMP_CC=`$CPP $CPPFLAGS conftest.c 2> /dev/null | $EGREP MPC_OPTION | $SED -e 's/MPC_OPTION //g' | $SED -e 's/"//g'`
   #Get CFLAGS
   echo "#include \"gmp.h\"" >  conftest.c
   echo "MPC_OPTION __GMP_CFLAGS"           >> conftest.c
   GMP_CFLAGS=`$CPP $CPPFLAGS conftest.c 2> /dev/null | $EGREP MPC_OPTION | $SED -e 's/MPC_OPTION //g'| $SED -e 's/"//g'`
   rm -f conftest.c
   if test "x$GMP_CFLAGS" = "x__GMP_CFLAGS" -o "x$GMP_CC" = "x__GMP_CC" ; then
      AC_MSG_RESULT(no)
      GMP_CFLAGS=
      GMP_CC=
   else
      AC_MSG_RESULT(yes [CC=$GMP_CC CFLAGS=$GMP_CFLAGS])
   fi

   dnl Check for validity of CC and CFLAGS obtained from gmp.h
   if test -n "$GMP_CFLAGS" ; then
      old_cflags=$CFLAGS
      old_cc=$CC
      CFLAGS=$GMP_CFLAGS
      CC=$GMP_CC
      AC_MSG_CHECKING(for CC=$GMP_CC and CFLAGS=$GMP_CFLAGS)
      AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[/*hello*/]],[[/*world*/]])], [
         AC_MSG_RESULT(yes)
         ], [
         AC_MSG_RESULT(no)
         CFLAGS=$old_cflags
         CC=$old_cc
      ])
      dnl CC may have changed. Recheck for GCC.
      AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
   #ifndef __GNUC__
   #error "GCC Not Found"
   error
   #endif
      ]])], [
      GCC=yes
      ], [
      GCC=no
      ])
   fi

])


#
# SYNOPSIS
#
#
# MPC_WINDOWS
#
# DESCRIPTION
#
# Additional checks on windows
# libtool requires "-no-undefined" for win32 dll
# It also disables the tests involving the linking with LIBGMP if DLL
#
AC_DEFUN([MPC_WINDOWS], [
   if test "$enable_shared" = yes; then
     LDFLAGS="$LDFLAGS -Wl,-no-undefined"
     AC_MSG_CHECKING(for DLL/static gmp)
     AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include "gmp.h"
#if !__GMP_LIBGMP_DLL
#error
error
#endif
     ]], [[]])],[AC_MSG_RESULT(DLL)],[
  AC_MSG_RESULT(static)
  AC_MSG_ERROR([gmp is not available as a DLL: use --enable-static --disable-shared]) ])
     AC_MSG_CHECKING(for DLL/static mpfr)
     AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include "mpfr.h"
#if !__GMP_LIBGMP_DLL
#error
error
#endif
     ]], [[]])],[AC_MSG_RESULT(DLL)],[
  AC_MSG_RESULT(static)
  AC_MSG_ERROR([mpfr is not available as a DLL: use --enable-static --disable-shared]) ])
   else
     AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include "gmp.h"
#if __GMP_LIBGMP_DLL
#error
error
#endif
     ]], [[]])],[AC_MSG_RESULT(static)],[
  AC_MSG_RESULT(DLL)
  AC_MSG_ERROR([gmp is only available as a DLL: use --disable-static --enable-shared]) ])
  fi
  ;;
])
