# Makefile for the MPC library.
#
# Copyright (C) 2002 Andreas Enge, Paul Zimmermann
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

# directory where GMP is installed (you need GMP 4.1 or later, configured
# with MPFR installed, i.e. ./configure --enable-mpfr):
# headers are expected in $(GMP)/include, lib in $(GMP)/lib
GMP=/usr/local
MPFR=$(GMP)

######################## do not edit below this line ##########################

VERSION=0.4.1

.SUFFIXES: .c .o

OBJECTS= abs.o add.o add_fr.o add_ui.o clear.o cmp.o conj.o div.o div_2exp.o div_fr.o div_ui.o exp.o init.o init2.o init3.o inp_str.o mul.o mul_2exp.o mul_fr.o mul_ui.o neg.o norm.o out_str.o random.o random2.o set.o set_d_d.o set_dfl_prec.o set_prec.o set_ui_fr.o set_si_si.o set_ui_ui.o sqr.o sqrt.o sub.o sub_ui.o ui_div.o
SOURCES= abs.c add.c add_fr.c add_ui.c clear.c cmp.c conj.c div.c div_2exp.c div_fr.c div_ui.c exp.c init.c init2.c init3.c inp_str.c mul.c mul_2exp.c mul_fr.c mul_ui.c neg.c norm.c out_str.c random.c random2.c set.c set_d_d.c set_dfl_prec.c set_prec.c set_ui_fr.c set_si_si.c set_ui_ui.c sqr.c sqrt.c sub.c sub_ui.c ui_div.c
TESTS= test.c tmul.c tsqr.c tdiv.c texp.c
DIST= $(SOURCES) $(TESTS) makefile mpc.h mpc-impl.h COPYING.LIB mpc.texi INSTALL

CFLAGS= -g -O2 -Wall -Wmissing-prototypes -ansi -pedantic

.c.o:
	gcc -I$(MPFR)/include -I$(GMP)/include -I. $(CFLAGS) -c $<

libmpc.a: $(OBJECTS)
	ar cru libmpc.a $(OBJECTS)
	ranlib libmpc.a

check: test tmul tsqr tdiv texp
	@echo Testing all functions
	rm -f mpc_test
	./test
	rm -f mpc_test
	@echo Testing mpc_mul
	./tmul
	@echo Testing mpc_sqr
	./tsqr
	@echo Testing mpc_div
	./tdiv
	@echo Testing mpc_exp
	./texp

test: test.c libmpc.a
	gcc $(CFLAGS) -I$(MPFR)/include -I$(GMP)/include -I. -L. test.c -o test -lmpc $(MPFR)/lib/libmpfr.a $(GMP)/lib/libgmp.a

tmul: tmul.c libmpc.a
	gcc $(CFLAGS) -I$(MPFR)/include -I$(GMP)/include -I. -L. tmul.c -o tmul -lmpc $(MPFR)/lib/libmpfr.a $(GMP)/lib/libgmp.a

tsqr: tsqr.c libmpc.a
	gcc $(CFLAGS) -I$(MPFR)/include -I$(GMP)/include -I. -L. tsqr.c -o tsqr -lmpc $(MPFR)/lib/libmpfr.a $(GMP)/lib/libgmp.a

tdiv: tdiv.c libmpc.a
	gcc $(CFLAGS) -I$(MPFR)/include -I$(GMP)/include -I. -L. tdiv.c -o tdiv -lmpc $(MPFR)/lib/libmpfr.a $(GMP)/lib/libgmp.a

texp: texp.c libmpc.a
	gcc $(CFLAGS) -I$(MPFR)/include -I$(GMP)/include -I. -L. texp.c -o texp -lmpc $(MPFR)/lib/libmpfr.a $(GMP)/lib/libgmp.a

clean:
	rm -f *.o *~ libmpc.a test tmul tsqr tdiv texp mpc-$(VERSION).tar.gz mpc.aux mpc.cp mpc.cps mpc.dvi mpc.fn mpc.fns mpc.ky mpc.log mpc.pg mpc.ps mpc.toc mpc.tp mpc.vr mpc.vrs

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
