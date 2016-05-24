/* mpc_rootofunity -- primitive root of unity.

Copyright (C) 2012, 2016 INRIA

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

/* put in rop the value of exp(2*i*pi/n) rounded according to rnd */
int
mpc_rootofunity (mpc_ptr rop, unsigned long int n, mpc_rnd_t rnd)
{
   mpfr_t t, s, c;
   mpfr_prec_t prec;
   int inex_re, inex_im;

   if (n == 0) {
      /* Compute exp (0 + i*inf). */
      mpfr_set_nan (mpc_realref (rop));
      mpfr_set_nan (mpc_imagref (rop));
      return MPC_INEX (0, 0);
   }

   /* We assume that only n=1, 2, 3, 4, 6, 8, 12 yield exact results and
      treat them separately.                                          */
   if (n == 1)
      return mpc_set_ui_ui (rop, 1, 0, rnd);
   else if (n == 2)
      return mpc_set_si_si (rop, -1, 0, rnd);
   else if (n == 4)
      return mpc_set_ui_ui (rop, 0, 1, rnd);
   else if (n == 3 || n == 6) {
      inex_re = mpfr_set_si (mpc_realref (rop), (n == 3 ? -1 : 1),
                             MPC_RND_RE (rnd));
      inex_im = mpfr_sqrt_ui (mpc_imagref (rop), 3, MPC_RND_IM (rnd));
      mpc_div_2ui (rop, rop, 1, MPC_RNDNN);
      return MPC_INEX (inex_re, inex_im);
   }
   else if (n == 8) {
      inex_re = mpfr_sqrt_ui (mpc_realref (rop), 2, MPC_RND_RE (rnd));
      inex_im = mpfr_sqrt_ui (mpc_imagref (rop), 2, MPC_RND_IM (rnd));
      mpc_div_2ui (rop, rop, 1u, MPC_RNDNN);
      return MPC_INEX (inex_re, inex_im);
   }
   else if (n == 12) {
      inex_re = mpfr_sqrt_ui (mpc_realref (rop), 3, MPC_RND_RE (rnd));
      inex_im = mpfr_set_ui (mpc_imagref (rop), 1, MPC_RND_IM (rnd));
      mpc_div_2ui (rop, rop, 1u, MPC_RNDNN);
      return MPC_INEX (inex_re, inex_im);
   }

   prec = MPC_MAX_PREC(rop);

   mpfr_init2 (t, 2);
   mpfr_init2 (s, 2);
   mpfr_init2 (c, 2);

   do {
      prec += mpc_ceil_log2 (prec) + 5;

      mpfr_set_prec (t, prec);
      mpfr_set_prec (s, prec);
      mpfr_set_prec (c, prec);

      mpfr_const_pi (t, MPFR_RNDN); /* error 0.5 ulp */
      mpfr_mul_2ui (t, t, 1u, MPFR_RNDN);
      mpfr_div_ui (t, t, n, MPFR_RNDN); /* error 2*0.5+0.5=1.5 ulp */
      mpfr_sin_cos (s, c, t, MPFR_RNDN);
         /* error (1.5*2^{Exp (t) - Exp (s resp.c)} + 0.5) ulp
            <= 6.5 ulp for n>=3                             */
   }
   while (   !mpfr_can_round (c, prec - 3, MPFR_RNDN, MPFR_RNDZ,
                 MPC_PREC_RE(rop) + (MPC_RND_RE(rnd) == MPFR_RNDN))
          || !mpfr_can_round (s, prec - 3, MPFR_RNDN, GMP_RNDZ,
                 MPC_PREC_IM(rop) + (MPC_RND_IM(rnd) == MPFR_RNDN)));

   inex_re = mpfr_set (mpc_realref(rop), c, MPC_RND_RE(rnd));
   inex_im = mpfr_set (mpc_imagref(rop), s, MPC_RND_IM(rnd));

   mpfr_clear (t);
   mpfr_clear (s);
   mpfr_clear (c);

   return MPC_INEX(inex_re, inex_im);
}
