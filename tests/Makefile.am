## Makefile.am -- Process this file with automake to produce Makefile.in
LDADD = libmpc-tests.la $(top_builddir)/src/libmpc.la
AM_CPPFLAGS = -I$(top_srcdir)/src -D__SRCDIR=$(srcdir)

check_PROGRAMS = tabs tadd tadd_fr tadd_ui targ test tconj tcos tcosh tdiv	\
tdiv_2exp tdiv_fr tdiv_ui texp tfr_div tfr_sub timag tlog tmul tmul_2exp	\
tmul_fr tmul_i tmul_si tmul_ui tneg tnorm tproj treal tsin tsinh tsqr tsqrt	\
tsub tsub_fr tsub_ui ttan ttanh tui_div tui_ui_sub

check_LTLIBRARIES=libmpc-tests.la
libmpc_tests_la_SOURCES=mpc-tests.h random.c tgeneric.c read_data.c	\
comparisons.c

EXTRA_DIST = abs.dat add.dat add_fr.dat conj.dat cos.dat cosh.dat      \
div.dat div_fr.dat exp.dat fr_div.dat log.dat mul.dat mul_fr.dat       \
neg.dat norm.dat proj.dat sin.dat sinh.dat sqr.dat sqrt.dat sub.dat    \
tan.dat tanh.dat

TESTS = $(check_PROGRAMS)
CLEANFILES = mpc_test

