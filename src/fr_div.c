/* mpc_fr_div -- Divide a floating-point number by a complex number.

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
#include <stdio.h>
#include "gmp.h"
#include "mpfr.h"
#include "mpc.h"
#include "mpc-impl.h"

int
mpc_fr_div (mpc_ptr a, mpfr_srcptr b, mpc_srcptr c, mpc_rnd_t rnd)
{
  mpc_t cc;
  mpc_t z;
  mpfr_t d;
  mp_prec_t prec;
  mp_rnd_t zre_rnd, zim_rnd;
  int inexact_prod, inexact_norm, inexact_re, inexact_im, loops = 0;
  int ok_re = 0, ok_im = 0;

  mpfr_init (d);
  mpc_init (z);

  /* cc=conj(c), exact operation, allocating no new memory */
  cc[0] = c[0];
  MPFR_CHANGE_SIGN (MPC_IM (cc));

  /* choose rounding direction so that |d| <= |b|/norm(c) */
  /*   d_rnd = MPFR_SIGN (b) < 0 ? GMP_RNDU : GMP_RNDD; */
  /* choose rounding direction so that |RE(z)| >= |RE(b/c)| and
     |IM(z)| >= |IM(b/c)| */
  zre_rnd = MPFR_SIGN (MPC_RE (cc)) == MPFR_SIGN (b) ? GMP_RNDU : GMP_RNDD;
  zim_rnd = MPFR_SIGN (MPC_IM (cc)) == MPFR_SIGN (b) ? GMP_RNDU : GMP_RNDD;

  prec = MPC_MAX_PREC (a);

  do
    {
      loops ++;
      prec += (loops <= 2) ? mpc_ceil_log2 (prec) + 5 : prec / 2;
      
      mpc_set_prec (z, prec);
      mpfr_set_prec (d, prec);
      
      /* first compute norm(c)^2 */
      inexact_norm = mpc_norm (d, c, GMP_RNDD);

      /* now compute b*conjugate(c) */
      /* We need rounding away from zero for both the real and the imagin-  */
      /* ary part; then the final result is also rounded away from zero.    */
      /* The error is less than 1 ulp. Since this is not implemented in     */
      /* mpfr, we round towards zero and add 1 ulp to the absolute values   */
      /* if they are not exact. */
      inexact_prod = mpc_mul_fr (z, cc, b, MPC_RNDZZ);
      inexact_re = MPC_INEX_RE (inexact_prod);
      inexact_im = MPC_INEX_IM (inexact_prod);
      if (inexact_re != 0)
        mpfr_add_one_ulp (MPC_RE (z), GMP_RNDN);
      if (inexact_im != 0)
        mpfr_add_one_ulp (MPC_IM (z), GMP_RNDN);

      /* divide the product by the norm */
      if (inexact_norm == 0 && (inexact_re == 0 || inexact_im == 0))
        {
          /* The division has good chances to be exact in at least one part.*/
          /* Since this can cause problems when not rounding to the nearest,*/
          /* we use the division code of mpfr, which handles the situation. */
          inexact_re |= mpfr_div (MPC_RE (z), MPC_RE (z), d, zre_rnd);
          ok_re = mpfr_can_round (MPC_RE (z), prec - 4, zre_rnd, 
                                  MPC_RND_RE (rnd), MPFR_PREC (MPC_RE (a)));

          if (ok_re || !inexact_re) /* compute imaginary part */
            {
              inexact_im |= mpfr_div (MPC_IM (z), MPC_IM (z), d, zim_rnd);
              ok_im = mpfr_can_round (MPC_IM (z), prec - 4, zim_rnd,
                                      MPC_RND_IM (rnd), MPFR_PREC(MPC_IM(a)));
            }
        }
      else
        {
          /* The division is inexact, so for efficiency reasons we invert q */
          /* only once and multiply by the inverse.                         */
          /* We do not decide about the sign of the difference. */
          if (mpfr_ui_div (d, 1, d, GMP_RNDU))
            {
              /* if 1/d is inexact, the approximations of the real and
                 imaginary part below will be inexact, unless RE(z)
                 or IM(z) is zero */
              inexact_re = inexact_re || (MPFR_IS_ZERO(MPC_RE(z)) == 0);
              inexact_im = inexact_im || (MPFR_IS_ZERO(MPC_IM(z)) == 0);
            }
          inexact_re = mpfr_mul (MPC_RE (z), MPC_RE (z), d, zre_rnd)
            || inexact_re;
          ok_re = mpfr_can_round (MPC_RE (z), prec - 4, zre_rnd,
                                  MPC_RND_RE(rnd), MPFR_PREC(MPC_RE(a)));

          if (ok_re) /* compute imaginary part */
            {
              inexact_im = mpfr_mul (MPC_IM (z), MPC_IM (z), d, zim_rnd)
                || inexact_im;
              ok_im = mpfr_can_round (MPC_IM (z), prec - 4, zim_rnd,
                                      MPC_RND_IM(rnd), MPFR_PREC(MPC_IM(a)));
            }
        }
    }
  while ((!ok_re && inexact_re) || (!ok_im && inexact_im));

  mpc_set (a, z, rnd);

  mpc_clear (z);
  mpfr_clear (d);

  return (MPC_INEX (inexact_re, inexact_im));
  /* Only exactness vs. inexactness is tested, not the sign. */
}
