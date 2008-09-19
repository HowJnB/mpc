## Makefile.am -- Process this file with automake to produce Makefile.in
LDADD = $(top_builddir)/src/libmpc.la


AM_CPPFLAGS = -I$(top_srcdir)/src
check_PROGRAMS = tadd test tconj tcos tcosh tdiv texp tfr_div tlog tmul tneg	\
tsin tsinh tsqr tsqrt tsub ttan ttanh
EXTRA_DIST = random.c tgeneric_cc.c

TESTS = $(check_PROGRAMS)
CLEANFILES = mpc_test

