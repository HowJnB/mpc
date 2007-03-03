# Makefile for the MPC library.
#
# Copyright (C) 2002, 2004, 2005 Andreas Enge, Paul Zimmermann
#
# This file is part of the MPC Library.
#
# The MPC Library is free software; you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation; either version 2.1 of the License, or (at your
# option) any later version.
#
# The MPC Library is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
# or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
# License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with the MPC Library; see the file COPYING.LIB.  If not, write to
# the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
# MA 02111-1307, USA.

AR=ar
CC=gcc
RANLIB=ranlib
CFLAGS= -g -O2 -Wall -Wmissing-prototypes -ansi -pedantic
RM=rm -f

GMP=/usr/local
MPFR=$(GMP)

######################## do not edit below this line ##########################

VERSION=0.4.5

.SUFFIXES: .c .o

INCLUDES=-I. -I$(MPFR)/include -I$(GMP)/include
LIBS=$(MPFR)/lib/libmpfr.a $(GMP)/lib/libgmp.a

OBJECTS= abs.o add.o add_fr.o add_ui.o clear.o cmp.o cmp_si_si.o conj.o div.o div_2exp.o div_fr.o div_ui.o exp.o init.o init2.o init3.o inp_str.o mul.o mul_2exp.o mul_fr.o mul_ui.o mul_si.o mul_i.o neg.o norm.o out_str.o random.o random2.o set.o set_d_d.o set_dfl_prec.o set_prec.o set_ui_fr.o set_si_si.o set_ui_ui.o sqr.o sqrt.o sub.o sub_ui.o ui_div.o uceil_log2.o ui_ui_sub.o sin.o
SOURCES= abs.c add.c add_fr.c add_ui.c clear.c cmp.c cmp_si_si.c conj.c div.c div_2exp.c div_fr.c div_ui.c exp.c init.c init2.c init3.c inp_str.c mul.c mul_2exp.c mul_fr.c mul_ui.c mul_si.c mul_i.c neg.c norm.c out_str.c random.c random2.c set.c set_d_d.c set_dfl_prec.c set_prec.c set_ui_fr.c set_si_si.c set_ui_ui.c sqr.c sqrt.c sub.c sub_ui.c ui_div.c uceil_log2.c ui_ui_sub.c sin.c
TESTS= test.c tmul.c tsqr.c tdiv.c texp.c tabs.c tsin.c
DIST= $(SOURCES) $(TESTS) makefile mpc.h mpc-impl.h COPYING.LIB mpc.texi INSTALL

.c.o:
	$(CC) $(INCLUDES) $(CFLAGS) -c $<

libmpc.a: $(OBJECTS)
	$(AR) cru libmpc.a $(OBJECTS)
	$(RANLIB) libmpc.a

check: test tmul tsqr tdiv texp tabs tsin
	@echo Testing all functions
	$(RM) -f mpc_test
	./test
	$(RM) -f mpc_test
	@echo Testing mpc_mul
	./tmul
	@echo Testing mpc_sqr
	./tsqr
	@echo Testing mpc_div
	./tdiv
	@echo Testing mpc_exp
	./texp
	@echo Testing mpc_sin
	./tsin
	@echo Testing mpc_abs
	./tabs

# Building scheme is complicated: we first try to build with explicit static lib, and if it fails with -l <= -Ldirectory is scanned AFTER system directory 
test: test.c libmpc.a
	@echo "Building test" && ((test -e $(GMP)/lib/libgmp.a && test -e $(MPFR)/lib/libmpfr.a && $(CC) $(CFLAGS) $(INCLUDES) test.c -o test ./libmpc.a $(MPFR)/lib/libmpfr.a $(GMP)/lib/libgmp.a) || $(CC) $(CFLAGS) $(INCLUDES) -L. -L$(MPFR)/lib -L$(GMP)/lib test.c -o test -lmpc -lmpfr -lgmp)

tmul: tmul.c libmpc.a
	@echo "Building tmul" && ((test -e $(GMP)/lib/libgmp.a && test -e $(MPFR)/lib/libmpfr.a && $(CC) $(CFLAGS) $(INCLUDES) tmul.c -o tmul ./libmpc.a $(MPFR)/lib/libmpfr.a $(GMP)/lib/libgmp.a) || $(CC) $(CFLAGS) $(INCLUDES) -L. -L$(MPFR)/lib -L$(GMP)/lib tmul.c -o tmul -lmpc -lmpfr -lgmp)

tsqr: tsqr.c libmpc.a
	@echo "Building tsqr" && ((test -e $(GMP)/lib/libgmp.a && test -e $(MPFR)/lib/libmpfr.a && $(CC) $(CFLAGS) $(INCLUDES) tsqr.c -o tsqr ./libmpc.a $(MPFR)/lib/libmpfr.a $(GMP)/lib/libgmp.a) || $(CC) $(CFLAGS) $(INCLUDES) -L. -L$(MPFR)/lib -L$(GMP)/lib tsqr.c -o tsqr -lmpc -lmpfr -lgmp)

tdiv: tdiv.c libmpc.a
	@echo "Building tdiv" && ((test -e $(GMP)/lib/libgmp.a && test -e $(MPFR)/lib/libmpfr.a && $(CC) $(CFLAGS) $(INCLUDES) tdiv.c -o tdiv ./libmpc.a $(MPFR)/lib/libmpfr.a $(GMP)/lib/libgmp.a) || $(CC) $(CFLAGS) $(INCLUDES) -L. -L$(MPFR)/lib -L$(GMP)/lib tdiv.c -o tdiv -lmpc -lmpfr -lgmp)

texp: texp.c libmpc.a
	@echo "Building texp" && ((test -e $(GMP)/lib/libgmp.a && test -e $(MPFR)/lib/libmpfr.a && $(CC) $(CFLAGS) $(INCLUDES) texp.c -o texp ./libmpc.a $(MPFR)/lib/libmpfr.a $(GMP)/lib/libgmp.a) || $(CC) $(CFLAGS) $(INCLUDES) -L. -L$(MPFR)/lib -L$(GMP)/lib texp.c -o texp -lmpc -lmpfr -lgmp)

tsin: tsin.c libmpc.a
	@echo "Building tsin" && ((test -e $(GMP)/lib/libgmp.a && test -e $(MPFR)/lib/libmpfr.a && $(CC) $(CFLAGS) $(INCLUDES) tsin.c -o tsin ./libmpc.a $(MPFR)/lib/libmpfr.a $(GMP)/lib/libgmp.a) || $(CC) $(CFLAGS) $(INCLUDES) -L. -L$(MPFR)/lib -L$(GMP)/lib tsin.c -o tsin -lmpc -lmpfr -lgmp)

tabs: tabs.c libmpc.a
	@echo "Building tabs" && ((test -e $(GMP)/lib/libgmp.a && test -e $(MPFR)/lib/libmpfr.a && $(CC) $(CFLAGS) $(INCLUDES) tabs.c -o tabs ./libmpc.a $(MPFR)/lib/libmpfr.a $(GMP)/lib/libgmp.a) || $(CC) $(CFLAGS) $(INCLUDES) -L. -L$(MPFR)/lib -L$(GMP)/lib tabs.c -o tabs -lmpc -lmpfr -lgmp)

clean:
	$(RM) *.o *~ libmpc.a test tmul tsqr tdiv texp tsin tabs mpc-$(VERSION).tar.gz mpc.aux mpc.cp mpc.cps mpc.dvi mpc.fn mpc.fns mpc.ky mpc.log mpc.pg mpc.ps mpc.toc mpc.tp mpc.vr mpc.vrs

dist: $(DIST)
	rm -fr mpc-$(VERSION)
	mkdir mpc-$(VERSION)
	cp $(DIST) mpc-$(VERSION)
	tar cf mpc-$(VERSION).tar mpc-$(VERSION)
	gzip --best mpc-$(VERSION).tar
	rm -fr mpc-$(VERSION)

mpc.dvi: mpc.texi
	texi2dvi mpc.texi

mpc.ps: mpc.dvi
	dvips mpc -o
