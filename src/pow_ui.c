/* mpc_pow_ui -- Raise a complex number to an integer power.

Copyright (C) 2009, 2010 Paul Zimmermann

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

#include <limits.h> /* for CHAR_BIT */
#include "mpc-impl.h"

int
mpc_pow_ui (mpc_ptr z, mpc_srcptr x, unsigned long y, mpc_rnd_t rnd)
{
  int inex;

  if (y == 0) /* let mpc_pow deal with that special case */
    {
      mpc_t t;
      mpc_init3 (t, sizeof (unsigned long) * CHAR_BIT, MPFR_PREC_MIN);
      mpc_set_ui (t, y, MPC_RNDNN);   /* exact */
      inex = mpc_pow (z, x, t, rnd);
      mpc_clear (t);
    }
  else if (y == 1)
    inex = mpc_set (z, x, rnd);
  else if (y == 2)
    inex = mpc_sqr (z, x, rnd);
  else /* case y >= 3 */
    {
      mpc_t t;
      mp_prec_t p, er, ei;
      unsigned long l, l0, u;
      mp_exp_t diff;

      for (l = 0, u = y; u > 3; l ++, u >>= 1);
      /* l>0 is the number of bits of y, minus 2, thus y has bits:
         y_{l+1} y_l y_{l-1} ... y_1 y_0 */
      l0 = l + 2;
      p = MPC_MAX_PREC(z) + l0 + 32; /* ensures that 2^{-p} (y-1) <= 1 below */
      mpc_init2 (t, p);

      mpc_sqr (t, x, MPC_RNDNN);
      if ((y >> l) & 1)
        mpc_mul (t, t, x, MPC_RNDNN);
      while (l-- > 0)
        {
          mpc_sqr (t, t, MPC_RNDNN);
          if ((y >> l) & 1)
            mpc_mul (t, t, x, MPC_RNDNN);
        }

      /* the absolute error on the real and imaginary parts is bounded
         by |x|^y (|1+2^{-p}|^{y-1}-1) [see algorithms.tex].
         For em <= 1, (1+e)^m - 1 <= 2em since
         (1+e)^m - 1 = exp(m*log(1+e))-1 <= exp(em)-1 <= 2em for em <= 1.
         We apply this for e=2^{-p} and m=y-1, thus the absolute error is
         bounded by |x|^y 2^{1-p} (y-1) < 2^{l0+1-p} */

      if (mpfr_zero_p (MPC_RE(t)) || mpfr_zero_p (MPC_IM(t)))
        goto failure; /* since mpfr_get_exp() is not defined for zero */

      diff = mpfr_get_exp (MPC_RE(t)) - mpfr_get_exp (MPC_IM(t));
      /* the factor on the real part is 2+2^(-diff+2) <= 4 for diff >= 1
         and <= 2^(-diff+3) for diff <= 0 */
      er = (diff >= 1) ? l0 + 3 : l0 + (-diff) + 4;
      /* the factor on the imaginary part is 2+2^(diff+2) <= 4 for diff <= -1
         and <= 2^(diff+3) for diff >= 0 */
      ei = (diff <= -1) ? l0 + 3 : l0 + diff + 4;
      if (mpfr_can_round (MPC_RE(t), p - er, GMP_RNDZ, GMP_RNDZ,
                          MPFR_PREC(MPC_RE(z)) + (MPC_RND_RE(rnd) == GMP_RNDN))
          && mpfr_can_round (MPC_IM(t), p - ei, GMP_RNDZ, GMP_RNDZ,
                         MPFR_PREC(MPC_IM(z)) + (MPC_RND_IM(rnd) == GMP_RNDN)))
        inex = mpc_set (z, t, rnd);
      else /* fall back to mpc_pow, in particular to avoid to detect exact
              cases */
        {
        failure:
          mpfr_set_prec (MPC_RE(t), sizeof (unsigned long) * CHAR_BIT);
          mpfr_set_prec (MPC_IM(t), MPFR_PREC_MIN);
          mpc_set_ui (t, y, MPC_RNDNN);   /* exact */
          inex = mpc_pow (z, x, t, rnd);
        }
      mpc_clear (t);
    }

  return inex;
}

