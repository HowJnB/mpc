/* mpc_log -- Take the logarithm of a complex number.

Copyright (C) 2008 Andreas Enge

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

void
mpc_log (mpc_ptr a, mpc_srcptr b, mpc_rnd_t rnd)
{
   int ok=0;
   mpfr_t w;
   mp_prec_t prec;
   int loops = 0;
   int re_cmp, im_cmp;
   int overlap = (a == b);

   /* special cases: real and purely imaginary numbers */
   re_cmp = mpfr_cmp_ui (MPC_RE (b), 0);
   im_cmp = mpfr_cmp_ui (MPC_IM (b), 0);
   if (im_cmp == 0)
   {
      if (re_cmp >= 0)
      {
         mpfr_log (MPC_RE (a), MPC_RE (b), MPC_RND_RE (rnd));
         mpfr_set_ui (MPC_IM (a), 0, GMP_RNDN);
         return;
      }
      else
      {
         /* b = x + 0*y; let w = -x = |x| */
         if (overlap) {
            mpfr_init2 (w, MPFR_PREC (MPC_RE (b)));
            mpfr_neg (w, MPC_RE (b), GMP_RNDN);
         }
         else {
            w [0] = *MPC_RE (b);
            MPFR_CHANGE_SIGN (w);
         }

         mpfr_log (MPC_RE (a), w, MPC_RND_RE (rnd));
         mpfr_const_pi (MPC_IM (a), MPC_RND_IM (rnd));

         if (overlap)
            mpfr_clear (w);

         return;
      }
   }
   else if (re_cmp == 0)
   {
      if (im_cmp > 0)
      {
         mpfr_log (MPC_RE (a), MPC_IM (b), MPC_RND_RE (rnd));
         mpfr_const_pi (MPC_IM (a), MPC_RND_IM (rnd));
         mpfr_div_2ui (MPC_IM (a), MPC_IM (a), 1, GMP_RNDN);
         return;
      }
      else
      {
         w [0] = *MPC_IM (b);
         MPFR_CHANGE_SIGN (w);
         mpfr_log (MPC_RE (a), w, MPC_RND_RE (rnd));
         mpfr_const_pi (MPC_IM (a), INV_RND (MPC_RND_IM (rnd)));
         mpfr_div_2ui (MPC_IM (a), MPC_IM (a), 1, GMP_RNDN);
         mpfr_neg (MPC_IM (a), MPC_IM (a), GMP_RNDN);
         return;
      }
   }

   prec = MPC_PREC_RE(a);
   mpfr_init2 (w, prec);
   /* let b = x + iy; log = 1/2 log (x^2 + y^2) + i atan2 (y, x) */
   /* loop for the real part: log (x^2 + y^2)                    */
   do
   {
      loops ++;
      prec += (loops <= 2) ? mpc_ceil_log2 (prec) + 4 : prec / 2;
      mpfr_set_prec (w, prec);

      /* w is rounded down */
      mpc_norm (w, b, GMP_RNDD);
         /* error 1 ulp */
      mpfr_log (w, w, GMP_RNDD);
      /* generic error of log: (2^(2 - exp(w)) + 1) ulp */

      if (MPFR_EXP (w) >= 2)
         ok = mpfr_can_round (w, prec - 2, GMP_RNDD, MPC_RND_RE(rnd), MPC_PREC_RE(a));
      else
         ok = mpfr_can_round (w, prec - 3 + MPFR_EXP (w), GMP_RNDD, MPC_RND_RE(rnd), MPC_PREC_RE(a));
   }
   while (ok == 0);

   /* imaginary part */
   mpfr_atan2 (MPC_IM (a), MPC_IM (b), MPC_RE (b), MPC_RND_IM (rnd));

   /* set the real part; cannot be done before when a==b */
   mpfr_div_2ui (MPC_RE(a), w, 1, MPC_RND_RE (rnd));
   mpfr_clear (w);
}
