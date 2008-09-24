/* mpc_set_fr -- Set the real part of a complex number from a floating-point
   number (imaginary part equals +0 regardless of rounding mode).

Copyright (C) 2008 Philippe Th\'eveny

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

#include "gmp.h"
#include "mpfr.h"
#include "mpc.h"

int
mpc_set_fr (mpc_ptr a, mpfr_srcptr b, mpc_rnd_t rnd)
{
  int inex_re, inex_im;

  inex_re = mpfr_set (MPC_RE (a), b, MPC_RND_RE (rnd));
  inex_im = mpfr_set_ui (MPC_IM (a), 0, GMP_RNDN);

  return MPC_INEX(inex_re, inex_im);
}
