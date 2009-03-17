/* mpc_set_fr -- Set the real part of a complex number from a real number
   (imaginary part equals +0 regardless of rounding mode).

Copyright (C) 2008, 2009 Philippe Th\'eveny

This file is part of the MPC Library.

The MPC Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2.1 of the License, or (at your
option) any later version.

The MPC Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the MPC Library; see the file COPYING.LIB.  If not, write to
the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
MA 02111-1307, USA. */

#include "mpc-impl.h"

int
mpc_set_fr (mpc_ptr a, mpfr_srcptr b, mpc_rnd_t rnd)
{
  int inex_re, inex_im;

  inex_re = mpfr_set (MPC_RE (a), b, MPC_RND_RE (rnd));
  inex_im = mpfr_set_ui (MPC_IM (a), 0, GMP_RNDN);

  return MPC_INEX(inex_re, inex_im);
}

int
mpc_set_d (mpc_ptr a, double b, mpc_rnd_t rnd)
{
  int inex_re, inex_im;

  inex_re = mpfr_set_d (MPC_RE (a), b, MPC_RND_RE (rnd));
  inex_im = mpfr_set_ui (MPC_IM (a), 0, GMP_RNDN);

  return MPC_INEX(inex_re, inex_im);
}

int
mpc_set_ld (mpc_ptr a, long double b, mpc_rnd_t rnd)
{
  int inex_re, inex_im;

  inex_re = mpfr_set_ld (MPC_RE (a), b, MPC_RND_RE (rnd));
  inex_im = mpfr_set_ui (MPC_IM (a), 0, GMP_RNDN);

  return MPC_INEX(inex_re, inex_im);
}

int
mpc_set_ui (mpc_ptr a, unsigned long int b, mpc_rnd_t rnd)
{
  int inex_re, inex_im;

  inex_re = mpfr_set_ui (MPC_RE (a), b, MPC_RND_RE (rnd));
  inex_im = mpfr_set_ui (MPC_IM (a), 0, GMP_RNDN);

  return MPC_INEX(inex_re, inex_im);
}

int
mpc_set_si (mpc_ptr a, long int b, mpc_rnd_t rnd)
{
  int inex_re, inex_im;

  inex_re = mpfr_set_si (MPC_RE (a), b, MPC_RND_RE (rnd));
  inex_im = mpfr_set_ui (MPC_IM (a), 0, GMP_RNDN);

  return MPC_INEX(inex_re, inex_im);
}

int
mpc_set_z (mpc_ptr a, mpz_srcptr b, mpc_rnd_t rnd)
{
  int inex_re, inex_im;

  inex_re = mpfr_set_z (MPC_RE (a), b, MPC_RND_RE (rnd));
  inex_im = mpfr_set_ui (MPC_IM (a), 0, GMP_RNDN);

  return MPC_INEX(inex_re, inex_im);
}

int
mpc_set_q (mpc_ptr a, mpq_srcptr b, mpc_rnd_t rnd)
{
  int inex_re, inex_im;

  inex_re = mpfr_set_q (MPC_RE (a), b, MPC_RND_RE (rnd));
  inex_im = mpfr_set_ui (MPC_IM (a), 0, GMP_RNDN);

  return MPC_INEX(inex_re, inex_im);
}

int
mpc_set_f (mpc_ptr a, mpf_srcptr b, mpc_rnd_t rnd)
{
  int inex_re, inex_im;

  inex_re = mpfr_set_f (MPC_RE (a), b, MPC_RND_RE (rnd));
  inex_im = mpfr_set_ui (MPC_IM (a), 0, GMP_RNDN);

  return MPC_INEX(inex_re, inex_im);
}
