/* mpc_sin -- sine of a complex number.

Copyright (C) 2007 Andreas Enge, Paul Zimmermann

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
#include "mpc-impl.h"

void
mpc_sin (mpc_ptr rop, mpc_srcptr op, mpc_rnd_t rnd)
{
  mpfr_t x, y, z;
  mp_prec_t prec;
  int ok = 0;

  /* let op = a + i*b, then sin(op) = sin(a)*cosh(b) + i*cos(a)*sinh(b).

     We use the following algorithm (same for the imaginary part),
     with rounding to nearest for all operations, and working precision w:

     (1) x = o(sin(a))
     (2) y = o(cosh(b))
     (3) r = o(x*y)     
     then the error on r is at most 4 ulps, since we can write
     r = sin(a)*cosh(b)*(1+t)^3 with |t| <= 2^(-w),
     thus for w >= 2, r = sin(a)*cosh(b)*(1+4*t) with |t| <= 2^(-w),
     thus the relative error is bounded by 4*2^(-w) <= 4*ulp(r).
  */
  
  /* special case when the input is real: sin(x) = sin(x) */
  if (mpfr_cmp_ui (MPC_IM(op), 0) == 0)
    {
      mpfr_sin (MPC_RE(rop), MPC_RE(op), MPC_RND_RE(rnd));
      mpfr_set_ui (MPC_IM(rop), 0, MPC_RND_IM(rnd));
      return;
    }

  /* special case when the input is imaginary: sin(I*y) = sinh(y)*I */
  if (mpfr_cmp_ui (MPC_RE(op), 0) == 0)
    {
      mpfr_set_ui (MPC_RE(rop), 0, MPC_RND_RE(rnd));
      mpfr_sinh (MPC_IM(rop), MPC_IM(op), MPC_RND_IM(rnd));
      return;
    }

  prec = MPC_MAX_PREC(rop);

  mpfr_init2 (x, 2);
  mpfr_init2 (y, 2);
  mpfr_init2 (z, 2);

  do
    {
      prec += mpc_ceil_log2 (prec) + 5;

      mpfr_set_prec (x, prec);
      mpfr_set_prec (y, prec);
      mpfr_set_prec (z, prec);

      mpfr_sin_cos (x, y, MPC_RE(op), GMP_RNDN);
      mpfr_cosh (z, MPC_IM(op), GMP_RNDN);
      mpfr_mul (x, x, z, GMP_RNDN);
      ok = mpfr_can_round (x, prec - 2, GMP_RNDN, MPC_RND_RE(rnd),
                           MPFR_PREC(MPC_RE(rop)));
      if (ok) /* compute imaginary part */
        {
	  mpfr_sinh (z, MPC_IM(op), GMP_RNDN);
	  mpfr_mul (y, y, z, GMP_RNDN);
          ok = mpfr_can_round (y, prec - 2, GMP_RNDN, MPC_RND_IM(rnd),
                               MPFR_PREC(MPC_IM(rop)));
        }
    }
  while (ok == 0);

  mpfr_set (MPC_RE(rop), x, MPC_RND_RE(rnd));
  mpfr_set (MPC_IM(rop), y, MPC_RND_IM(rnd));

  mpfr_clear (x);
  mpfr_clear (y);
  mpfr_clear (z);
}
