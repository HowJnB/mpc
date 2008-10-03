## Makefile.am -- Process this file with automake to produce Makefile.in
LDADD = $(top_builddir)/src/libmpc.la
AM_CPPFLAGS = -I$(top_srcdir)/src -D__SRCDIR=$(srcdir)

check_PROGRAMS = tabs tadd tadd_fr tadd_ui test tconj tcos tcosh tdiv		\
tdiv_2exp tdiv_fr tdiv_ui texp tfr_div tfr_sub tlog tmul tmul_2exp tmul_fr	\
tmul_si tmul_ui tneg tnorm tsin tsinh tsqr tsqrt tsub tsub_fr tsub_ui ttan	\
ttanh tui_div tui_sub tui_ui_sub
EXTRA_DIST = random.c read_data_cc.c tgeneric_fc.c tgeneric_cc.c		\
tgeneric_ccc.c tgeneric_ccf.c tgeneric_cfc.c tgeneric_ccu.c tgeneric_cuc.c	\
tgeneric_cuuc.c tgeneric_ccs.c sin.dat sqr.dat

TESTS = $(check_PROGRAMS)
CLEANFILES = mpc_test

