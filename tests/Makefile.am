## Makefile.am -- Process this file with automake to produce Makefile.in
LDADD = $(top_builddir)/src/libmpc.la


AM_CPPFLAGS = -I$(top_srcdir)/src -I$(top_srcdir)/tests
check_PROGRAMS = test tcos tdiv texp tmul tsin tsqr tsqrt

TESTS = $(check_PROGRAMS)
CLEANFILES = mpc_test

