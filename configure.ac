#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(mpc, 0.4.7, mpc-discuss@lists.gforge.inria.fr)
AC_CONFIG_SRCDIR([src/mpc-impl.h])
AC_CONFIG_HEADER([config.h])

AM_INIT_AUTOMAKE([1.9 -Wall -Werror])
AM_MAINTAINER_MODE

CFLAGS="-O2 -g -Wall"

dnl Extra arguments to configure
AC_ARG_WITH([mpfr],
            [AC_HELP_STRING([--with-mpfr=DIR],
                           [MPFR install directory])],
            [
            CPPFLAGS="$CPPFLAGS -I$withval/include"
            LDFLAGS="$LDFLAGS -L$withval/lib"
            ])
AC_ARG_WITH([gmp],
            [AC_HELP_STRING([--with-gmp=DIR],
                           [GMP install directory])],
            [
            CPPFLAGS="$CPPFLAGS -I$withval/include"
            LDFLAGS="$LDFLAGS -L$withval/lib"
            ])

# Checks for programs.
AC_PROG_CC
AC_PROG_LIBTOOL

# Checks for libraries.
AC_CHECK_LIB([mpfr], [mpfr_clear],,
             [AC_MSG_ERROR([MPC library needs the MPFR library])])

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([limits.h stdlib.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_SIZE_T

# Checks for library functions.

AC_CONFIG_FILES([Makefile src/Makefile tests/Makefile doc/Makefile])
AC_OUTPUT
