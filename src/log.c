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
mpc_log (mpc_ptr rop, mpc_srcptr op, mpc_rnd_t rnd){
   int ok=0;
   mpfr_t w;
   mp_prec_t prec;
   int loops = 0;
   int re_cmp, im_cmp;
   int overlap = (rop == op);

   /* special values: NaN and infinities */
   if (!mpfr_number_p (MPC_RE (op)) || !mpfr_number_p (MPC_IM (op))) {
      if (mpfr_nan_p (MPC_RE (op))) {
         if (mpfr_inf_p (MPC_IM (op)))
            mpfr_set_inf (MPC_RE (rop), +1);
         else
            mpfr_set_nan (MPC_RE (rop));
         mpfr_set_nan (MPC_IM (rop));
      }
      else if (mpfr_nan_p (MPC_IM (op))) {
         if (mpfr_inf_p (MPC_RE (op)))
            mpfr_set_inf (MPC_RE (rop), +1);
         else
            mpfr_set_nan (MPC_RE (rop));
         mpfr_set_nan (MPC_IM (rop));
      }
      else /* We have an infinity in at least one part. */ {
         mpfr_atan2 (MPC_IM (rop), MPC_IM (op), MPC_RE (op), MPC_RND_IM (rnd));
         mpfr_set_inf (MPC_RE (rop), +1);
      }
      return;
   }

   /* special cases: real and purely imaginary numbers */
   re_cmp = mpfr_cmp_ui (MPC_RE (op), 0);
   im_cmp = mpfr_cmp_ui (MPC_IM (op), 0);
   if (im_cmp == 0) {
      if (re_cmp == 0) {
         mpfr_atan2 (MPC_IM (rop), MPC_IM (op), MPC_RE (op), MPC_RND_IM (rnd));
         mpfr_set_inf (MPC_RE (rop), -1);
      }
      else if (re_cmp > 0) {
         mpfr_log (MPC_RE (rop), MPC_RE (op), MPC_RND_RE (rnd));
         mpfr_set_ui (MPC_IM (rop), 0, GMP_RNDN);
      }
      else {
         /* op = x + 0*y; let w = -x = |x| */
         if (overlap) {
            mpfr_init2 (w, MPFR_PREC (MPC_RE (op)));
            mpfr_neg (w, MPC_RE (op), GMP_RNDN);
         }
         else {
            w [0] = *MPC_RE (op);
            MPFR_CHANGE_SIGN (w);
         }

         mpfr_log (MPC_RE (rop), w, MPC_RND_RE (rnd));
         mpfr_const_pi (MPC_IM (rop), MPC_RND_IM (rnd));

         if (overlap)
            mpfr_clear (w);
      }
      return;
   }
   else if (re_cmp == 0) {
      if (im_cmp > 0) {
         mpfr_log (MPC_RE (rop), MPC_IM (op), MPC_RND_RE (rnd));
         mpfr_const_pi (MPC_IM (rop), MPC_RND_IM (rnd));
         mpfr_div_2ui (MPC_IM (rop), MPC_IM (rop), 1, GMP_RNDN);
      }
      else {
         w [0] = *MPC_IM (op);
         MPFR_CHANGE_SIGN (w);
         mpfr_log (MPC_RE (rop), w, MPC_RND_RE (rnd));
         mpfr_const_pi (MPC_IM (rop), INV_RND (MPC_RND_IM (rnd)));
         mpfr_div_2ui (MPC_IM (rop), MPC_IM (rop), 1, GMP_RNDN);
         mpfr_neg (MPC_IM (rop), MPC_IM (rop), GMP_RNDN);
      }
      return;
   }

   prec = MPC_PREC_RE(rop);
   mpfr_init2 (w, prec);
   /* let op = x + iy; log = 1/2 log (x^2 + y^2) + i atan2 (y, x) */
   /* loop for the real part: log (x^2 + y^2)                    */
   do {
      loops ++;
      prec += (loops <= 2) ? mpc_ceil_log2 (prec) + 4 : prec / 2;
      mpfr_set_prec (w, prec);

      /* w is rounded down */
      mpc_norm (w, op, GMP_RNDD);
      /* error 1 ulp */
      mpfr_log (w, w, GMP_RNDD);
      /* generic error of log: (2^(2 - exp(w)) + 1) ulp */

      if (MPFR_EXP (w) >= 2)
         ok = mpfr_can_round (w, prec - 2, GMP_RNDD, MPC_RND_RE(rnd), MPC_PREC_RE(rop));
      else
         ok = mpfr_can_round (w, prec - 3 + MPFR_EXP (w), GMP_RNDD, MPC_RND_RE(rnd), MPC_PREC_RE(rop));
   } while (ok == 0);

   /* imaginary part */
   mpfr_atan2 (MPC_IM (rop), MPC_IM (op), MPC_RE (op), MPC_RND_IM (rnd));

   /* set the real part; cannot be done before when rop==op */
   mpfr_div_2ui (MPC_RE(rop), w, 1ul, MPC_RND_RE (rnd));
   mpfr_clear (w);
}
