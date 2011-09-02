/* mpc_norm -- Square of the norm of a complex number.

Copyright (C) 2002, 2005, 2008, 2009, 2010, 2011 INRIA

This file is part of GNU MPC.

GNU MPC is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the
Free Software Foundation; either version 3 of the License, or (at your
option) any later version.

GNU MPC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for
more details.

You should have received a copy of the GNU Lesser General Public License
along with this program. If not, see http://www.gnu.org/licenses/ .
*/

#include "mpc-impl.h"

/* a <- norm(b) = b * conj(b)
   (the rounding mode is mpfr_rnd_t here since we return an mpfr number) */
int
mpc_norm (mpfr_ptr a, mpc_srcptr b, mpfr_rnd_t rnd)
{
   int inexact;

   /* handling of special values; consistent with abs in that
      norm = abs^2; so norm (+-inf, xxx) = norm (xxx, +-inf) = +inf */
   if (!mpc_fin_p (b))
         return mpc_abs (a, b, rnd);
   else if (mpfr_zero_p (MPC_RE (b))) {
      if (mpfr_zero_p (MPC_IM (b)))
         return mpfr_set_ui (a, 0, rnd); /* +0 */
      else
         return mpfr_sqr (a, MPC_IM (b), rnd);
   }
   else if (mpfr_zero_p (MPC_IM (b)))
     return mpfr_sqr (a, MPC_RE (b), rnd); /* Re(b) <> 0 */

   else /* everything finite and non-zero */ {
      mpfr_t u, v, res;
      mpfr_prec_t prec, prec_u, prec_v;
      int loops;
      const int max_loops = 2;
         /* switch to exact squarings when loops==max_loops */

      prec = mpfr_get_prec (a);

      mpfr_init (u);
      mpfr_init (v);
      mpfr_init (res);

      loops = 0;
      mpfr_clear_underflow ();
      mpfr_clear_overflow ();
      do {
         loops++;
         prec += mpc_ceil_log2 (prec) + 3;
         if (loops >= max_loops) {
            prec_u = 2 * MPC_PREC_RE (b);
            prec_v = 2 * MPC_PREC_IM (b);
         }
         else {
            prec_u = MPC_MIN (prec, 2 * MPC_PREC_RE (b));
            prec_v = MPC_MIN (prec, 2 * MPC_PREC_IM (b));
         }

         mpfr_set_prec (u, prec_u);
         mpfr_set_prec (v, prec_v);

         inexact  = mpfr_sqr (u, MPC_RE(b), GMP_RNDD); /* err <= 1 ulp in prec */
         inexact |= mpfr_sqr (v, MPC_IM(b), GMP_RNDD); /* err <= 1 ulp in prec */

         /* If loops = max_loops, inexact should be 0 here, except in case
               of underflow or overflow.
            If loops < max_loops and inexact is zero, we can exit the
            while-loop since it only remains to add u and v into a. */
         if (inexact != 0)
           {
             mpfr_set_prec (res, prec);
             mpfr_add (res, u, v, GMP_RNDD); /* err <= 3 ulp in prec */
           }

      } while (loops < max_loops && inexact != 0
               && !mpfr_can_round (res, prec - 2, GMP_RNDD, GMP_RNDU,
                                   mpfr_get_prec (a) + (rnd == GMP_RNDN)));

      if (inexact == 0) /* squarings were exact, nor underflow nor overflow */
         inexact = mpfr_add (a, u, v, rnd);
      /* if there was an overflow in Re(b)^2 or Im(b)^2 or their sum,
         since the norm is larger, there is an overflow for the norm */
      else if (mpfr_overflow_p ()) {
         /* replace by "correctly rounded overflow" */
         mpfr_set_ui (a, 1ul, GMP_RNDN);
         inexact = mpfr_mul_2ui (a, a, mpfr_get_emax (), rnd);
      }
      else if (mpfr_underflow_p ()) {
        /* necessarily one of the squarings did underflow (otherwise their
           sum could not underflow), thus one of u, v is zero */
         /* squarings were exact except for underflow */
         inexact = mpfr_add (a, u, v, rnd);
         if (!inexact) {
            if (rnd == GMP_RNDN || rnd == GMP_RNDD || rnd == GMP_RNDZ)
               inexact = -1;
            else { /* rounding up */
               mpfr_nextabove (a);
               inexact = 1;
            }
         }
      }
      else /* no problems, ternary value due to mpfr_can_round trick */
         inexact = mpfr_set (a, res, rnd);

      mpfr_clear (u);
      mpfr_clear (v);
      mpfr_clear (res);
   }

   return inexact;
}
