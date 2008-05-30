#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(mpc, 0.4.7, mpc-discuss@lists.gforge.inria.fr)
AC_CONFIG_SRCDIR([src/mpc-impl.h])
AC_CONFIG_HEADER([config.h])

AM_INIT_AUTOMAKE([1.9 -Wall -Werror])
AM_MAINTAINER_MODE

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
AC_CHECK_LIB([mpfr], [mpfr_clear],,
             [AC_MSG_ERROR([MPC library needs the MPFR library])])

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([limits.h stdlib.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_SIZE_T

# Checks for library functions.


# Checks for CC flags.
# Modification PT: move in the last place so as to avoid test failure in
# AC_C_CONST due to additional flags "-Wall -Werror"

# Variadic macros (used by logging macros) were introduced in C99
#AX_C_CHECK_FLAG(ansi,,,CFLAGS="$CFLAGS -ansi",)
AX_C_CHECK_FLAG(-std=c99,,,CFLAGS="$CFLAGS -std=c99",)

AX_C_CHECK_FLAG(-pedantic,,,CFLAGS="$CFLAGS -pedantic",)
AX_C_CHECK_FLAG(-Wall,,,CFLAGS="$CFLAGS -Wall",)
AX_C_CHECK_FLAG(-Wextra,,,CFLAGS="$CFLAGS -Wextra",)
AX_C_CHECK_FLAG(-Werror,,,CFLAGS="$CFLAGS -Werror",)


AC_CONFIG_FILES([Makefile src/Makefile tests/Makefile doc/Makefile])
AC_OUTPUT
