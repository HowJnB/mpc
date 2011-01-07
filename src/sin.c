/* mpc_sin -- sine of a complex number.

Copyright (C) 2007, 2009, 2010, 2011 Paul Zimmermann, Philippe Th\'eveny, Andreas Enge

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
mpc_sin (mpc_ptr rop, mpc_srcptr op, mpc_rnd_t rnd)
{
  mpfr_t s, c, sh, ch;
  mpfr_prec_t prec;
  int ok = 0;
  int inex_re, inex_im;

  /* special values */
  if (!mpc_fin_p (op) || mpfr_zero_p (MPC_IM (op)) || mpfr_zero_p (MPC_RE (op)))
     return MPC_INEX1 (mpc_sin_cos (rop, NULL, op, rnd, 0));

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

  prec = MPC_MAX_PREC(rop);

  mpfr_init2 (s, 2);
  mpfr_init2 (c, 2);
  mpfr_init2 (sh, 2);
  mpfr_init2 (ch, 2);

  do
    {
      prec += mpc_ceil_log2 (prec) + 5;

      mpfr_set_prec (s, prec);
      mpfr_set_prec (c, prec);
      mpfr_set_prec (sh, prec);
      mpfr_set_prec (ch, prec);

      mpfr_sin_cos (s, c, MPC_RE(op), GMP_RNDN);
      mpfr_sinh_cosh (sh, ch, MPC_IM(op), GMP_RNDN);
      mpfr_mul (s, s, ch, GMP_RNDN);
      ok = (!mpfr_number_p (s))
           || mpfr_can_round (s, prec - 2, GMP_RNDN, GMP_RNDZ,
                      MPC_PREC_RE(rop) + (MPC_RND_RE(rnd) == GMP_RNDN));
      if (ok) /* compute imaginary part */
        {
          mpfr_mul (c, c, sh, GMP_RNDN);
          ok = (!mpfr_number_p (c))
               || mpfr_can_round (c, prec - 2, GMP_RNDN, GMP_RNDZ,
                      MPC_PREC_IM(rop) + (MPC_RND_IM(rnd) == GMP_RNDN));
        }
    }
  while (ok == 0);

  inex_re = mpfr_set (MPC_RE(rop), s, MPC_RND_RE(rnd));
  if (mpfr_inf_p (s))
     inex_re = mpfr_sgn (s);
  inex_im = mpfr_set (MPC_IM(rop), c, MPC_RND_IM(rnd));
  if (mpfr_inf_p (c))
     inex_im = mpfr_sgn (c);

  mpfr_clear (s);
  mpfr_clear (c);
  mpfr_clear (sh);
  mpfr_clear (ch);

  return MPC_INEX (inex_re, inex_im);
}
