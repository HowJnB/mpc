## Makefile.am -- Process this file with automake to produce Makefile.in
LDADD = $(top_builddir)/src/libmpc.la


AM_CPPFLAGS = -I$(top_srcdir)/src
check_PROGRAMS = tadd test tcos tcosh tdiv texp tlog tmul tsin tsinh tsqr	\
tsqrt tsub ttan ttanh

TESTS = $(check_PROGRAMS)
CLEANFILES = mpc_test

