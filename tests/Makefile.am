## Makefile.am -- Process this file with automake to produce Makefile.in
LDADD = $(top_builddir)/src/libmpc.la


AM_CPPFLAGS = -I$(top_srcdir)/src
check_PROGRAMS = tadd tadd_fr tadd_ui test tconj tcos tcosh tdiv tdiv_2exp	\
tdiv_fr tdiv_ui texp tfr_div tlog tmul tmul_2exp tmul_fr tmul_ui tneg tsin	\
tsinh tsqr tsqrt tsub tsub_fr tsub_ui ttan ttanh
EXTRA_DIST = random.c tgeneric_cc.c tgeneric_ccc.c tgeneric_ccu.c	\
tgeneric_ccf.c

TESTS = $(check_PROGRAMS)
CLEANFILES = mpc_test

