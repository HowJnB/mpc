/* mpc_log -- Take the logarithm of a complex number.

Copyright (C) 2008, 2009, 2010, 2011, 2012 INRIA

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

#include <stdio.h> /* for MPC_ASSERT */
#include "mpc-impl.h"

int
mpc_log (mpc_ptr rop, mpc_srcptr op, mpc_rnd_t rnd){
   int ok=0;
   mpfr_t w;
   mpfr_prec_t prec;
   int loops = 0;
   int re_cmp, im_cmp;
   int inex_re, inex_im, inex;

   /* special values: NaN and infinities */
   if (!mpc_fin_p (op)) {
      if (mpfr_nan_p (mpc_realref (op))) {
         if (mpfr_inf_p (mpc_imagref (op)))
            mpfr_set_inf (mpc_realref (rop), +1);
         else
            mpfr_set_nan (mpc_realref (rop));
         mpfr_set_nan (mpc_imagref (rop));
         inex_im = 0; /* Inf/NaN is exact */
      }
      else if (mpfr_nan_p (mpc_imagref (op))) {
         if (mpfr_inf_p (mpc_realref (op)))
            mpfr_set_inf (mpc_realref (rop), +1);
         else
            mpfr_set_nan (mpc_realref (rop));
         mpfr_set_nan (mpc_imagref (rop));
         inex_im = 0; /* Inf/NaN is exact */
      }
      else /* We have an infinity in at least one part. */ {
         inex_im = mpfr_atan2 (mpc_imagref (rop), mpc_imagref (op), mpc_realref (op),
                               MPC_RND_IM (rnd));
         mpfr_set_inf (mpc_realref (rop), +1);
      }
      return MPC_INEX(0, inex_im);
   }

   /* special cases: real and purely imaginary numbers */
   re_cmp = mpfr_cmp_ui (mpc_realref (op), 0);
   im_cmp = mpfr_cmp_ui (mpc_imagref (op), 0);
   if (im_cmp == 0) {
      if (re_cmp == 0) {
         inex_im = mpfr_atan2 (mpc_imagref (rop), mpc_imagref (op), mpc_realref (op),
                               MPC_RND_IM (rnd));
         mpfr_set_inf (mpc_realref (rop), -1);
         inex_re = 0; /* -Inf is exact */
      }
      else if (re_cmp > 0) {
         inex_re = mpfr_log (mpc_realref (rop), mpc_realref (op), MPC_RND_RE (rnd));
         inex_im = mpfr_set (mpc_imagref (rop), mpc_imagref (op), MPC_RND_IM (rnd));
      }
      else {
         /* op = x + 0*y; let w = -x = |x| */
         int negative_zero;
         mpfr_rnd_t rnd_im;

         negative_zero = mpfr_signbit (mpc_imagref (op));
         if (negative_zero)
            rnd_im = INV_RND (MPC_RND_IM (rnd));
         else
            rnd_im = MPC_RND_IM (rnd);
         w [0] = *mpc_realref (op);
         MPFR_CHANGE_SIGN (w);
         inex_re = mpfr_log (mpc_realref (rop), w, MPC_RND_RE (rnd));
         inex_im = mpfr_const_pi (mpc_imagref (rop), rnd_im);
         if (negative_zero) {
            mpc_conj (rop, rop, MPC_RNDNN);
            inex_im = -inex_im;
         }
      }
      return MPC_INEX(inex_re, inex_im);
   }
   else if (re_cmp == 0) {
      if (im_cmp > 0) {
         inex_re = mpfr_log (mpc_realref (rop), mpc_imagref (op), MPC_RND_RE (rnd));
         inex_im = mpfr_const_pi (mpc_imagref (rop), MPC_RND_IM (rnd));
         /* division by 2 does not change the ternary flag */
         mpfr_div_2ui (mpc_imagref (rop), mpc_imagref (rop), 1, GMP_RNDN);
      }
      else {
         w [0] = *mpc_imagref (op);
         MPFR_CHANGE_SIGN (w);
         inex_re = mpfr_log (mpc_realref (rop), w, MPC_RND_RE (rnd));
         inex_im = mpfr_const_pi (mpc_imagref (rop), INV_RND (MPC_RND_IM (rnd)));
         /* division by 2 does not change the ternary flag */
         mpfr_div_2ui (mpc_imagref (rop), mpc_imagref (rop), 1, GMP_RNDN);
         mpfr_neg (mpc_imagref (rop), mpc_imagref (rop), GMP_RNDN);
         inex_im = -inex_im; /* negate the ternary flag */
      }
      return MPC_INEX(inex_re, inex_im);
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
      inex = mpc_norm (w, op, GMP_RNDD);
         /* error 1 ulp */
      MPC_ASSERT (!mpfr_inf_p (w));
         /* FIXME: intermediate overflow; the logarithm may be representable */
      MPC_ASSERT (!(inex && mpfr_cmp_ui (w, 1) == 0));
         /* FIXME: this is like an underflow; the following call to log will
            compute 0 instead of a positive result                           */
      
      mpfr_log (w, w, GMP_RNDD);
      /* generic error of log: (2^(2 - exp(w)) + 1) ulp */

      if (mpfr_get_exp (w) >= 2)
         ok = mpfr_can_round (w, prec - 2, GMP_RNDD,
            MPC_RND_RE(rnd), MPC_PREC_RE(rop));
      else
         ok = mpfr_can_round (w, prec - 3 + mpfr_get_exp (w), GMP_RNDD,
            MPC_RND_RE(rnd), MPC_PREC_RE(rop));
   } while (ok == 0);

   /* imaginary part */
   inex_im = mpfr_atan2 (mpc_imagref (rop), mpc_imagref (op), mpc_realref (op),
                         MPC_RND_IM (rnd));

   /* set the real part; cannot be done before when rop==op */
   inex_re = mpfr_div_2ui (mpc_realref(rop), w, 1ul, MPC_RND_RE (rnd));
   mpfr_clear (w);
   return MPC_INEX(inex_re, inex_im);
}
