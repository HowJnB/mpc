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

#include <stdio.h> /* for MPC_ASSERT */
#include "mpc-impl.h"

static unsigned long
gcd (unsigned long a, unsigned long b)
{
   if (b == 0)
      return a;
   else return gcd (b, a % b);
}

/* put in rop the value of exp(2*i*pi*k/n) rounded according to rnd */
int
mpc_rootofunity (mpc_ptr rop, unsigned long n, unsigned long k, mpc_rnd_t rnd)
{
   unsigned long g;
   mpq_t kn;
   mpfr_t t, s, c;
   mpfr_prec_t prec;
   int inex_re, inex_im;
   mpfr_rnd_t rnd_re, rnd_im;

   if (n == 0) {
      /* Compute exp (0 + i*inf). */
      mpfr_set_nan (mpc_realref (rop));
      mpfr_set_nan (mpc_imagref (rop));
      return MPC_INEX (0, 0);
   }

   /* Eliminate common denominator. */
   k %= n;
   g = gcd (k, n);
   k /= g;
   n /= g;

   /* Now 0 <= k < n and gcd(k,n)=1. */

   /* We assume that only n=1, 2, 3, 4, 6 and 12 may yield exact results
      and treat them separately; n=8 is also treated here for efficiency
      reasons. */
   if (n == 1)
     {
       /* necessarily k=0 thus we want exp(0)=1 */
       MPC_ASSERT (k == 0);
       return mpc_set_ui_ui (rop, 1, 0, rnd);
     }
   else if (n == 2)
     {
       /* since gcd(k,n)=1, necessarily k=1, thus we want exp(i*pi)=-1 */
       MPC_ASSERT (k == 1);
       return mpc_set_si_si (rop, -1, 0, rnd);
     }
   else if (n == 4)
     {
       /* since gcd(k,n)=1, necessarily k=1 or k=3, thus we want
          exp(2*i*pi/4)=i or exp(2*i*pi*3/4)=-i */
       MPC_ASSERT (k == 1 || k == 3);
       if (k == 1)
         return mpc_set_ui_ui (rop, 0, 1, rnd);
       else
         return mpc_set_si_si (rop, 0, -1, rnd);
     }
   else if (n == 3 || n == 6)
     {
       MPC_ASSERT ((n == 3 && (k == 1 || k == 2)) ||
                   (n == 6 && (k == 1 || k == 5)));
       /* for n=3, necessarily k=1 or k=2: -1/2+/-1/2*sqrt(3)*i;
          for n=6, necessarily k=1 or k=5: 1/2+/-1/2*sqrt(3)*i */
       inex_re = mpfr_set_si (mpc_realref (rop), (n == 3 ? -1 : 1),
                              MPC_RND_RE (rnd));
       /* inverse the rounding mode for -sqrt(3)/2 for zeta_3^2 and zeta_6^5 */
       rnd_im = MPC_RND_IM (rnd);
       if (k != 1)
         rnd_im = INV_RND (rnd_im);
       inex_im = mpfr_sqrt_ui (mpc_imagref (rop), 3, rnd_im);
       mpc_div_2ui (rop, rop, 1, MPC_RNDNN);
       if (k != 1)
         {
           mpfr_neg (mpc_imagref (rop), mpc_imagref (rop), MPFR_RNDN);
           inex_im = -inex_im;
         }
       return MPC_INEX (inex_re, inex_im);
     }
   else if (n == 12)
     {
       /* necessarily k=1, 5, 7, 11:
          k=1: 1/2*sqrt(3) + 1/2*I
          k=5: -1/2*sqrt(3) + 1/2*I
          k=7: -1/2*sqrt(3) - 1/2*I
          k=11: 1/2*sqrt(3) - 1/2*I */
       MPC_ASSERT (k == 1 || k == 5 || k == 7 || k == 11);
       /* inverse the rounding mode for -sqrt(3)/2 for zeta_12^5 and zeta_12^7 */
       rnd_re = MPC_RND_RE (rnd);
       if (k == 5 || k == 7)
         rnd_re = INV_RND (rnd_re);
       inex_re = mpfr_sqrt_ui (mpc_realref (rop), 3, rnd_re);
       inex_im = mpfr_set_si (mpc_imagref (rop), k < 6 ? 1 : -1,
                              MPC_RND_IM (rnd));
       mpc_div_2ui (rop, rop, 1, MPC_RNDNN);
       if (k == 5 || k == 7)
         {
           mpfr_neg (mpc_realref (rop), mpc_realref (rop), MPFR_RNDN);
           inex_re = -inex_re;
         }
       return MPC_INEX (inex_re, inex_im);
     }
   else if (n == 8)
     {
       /* k=1, 3, 5 or 7:
          k=1: (1/2*I + 1/2)*sqrt(2)
          k=3: (1/2*I - 1/2)*sqrt(2)
          k=5: -(1/2*I + 1/2)*sqrt(2)
          k=7: -(1/2*I - 1/2)*sqrt(2) */
       MPC_ASSERT (k == 1 || k == 3 || k == 5 || k == 7);
       rnd_re = MPC_RND_RE (rnd);
       if (k == 3 || k == 5)
         rnd_re = INV_RND (rnd_re);
       rnd_im = MPC_RND_IM (rnd);
       if (k > 4)
         rnd_im = INV_RND (rnd_im);
       inex_re = mpfr_sqrt_ui (mpc_realref (rop), 2, rnd_re);
       inex_im = mpfr_sqrt_ui (mpc_imagref (rop), 2, rnd_im);
       mpc_div_2ui (rop, rop, 1, MPC_RNDNN);
       if (k == 3 || k == 5)
         {
           mpfr_neg (mpc_realref (rop), mpc_realref (rop), MPFR_RNDN);
           inex_re = -inex_re;
         }
       if (k > 4)
         {
           mpfr_neg (mpc_imagref (rop), mpc_imagref (rop), MPFR_RNDN);
           inex_im = -inex_im;
         }
       return MPC_INEX (inex_re, inex_im);
     }

   prec = MPC_MAX_PREC(rop);

   mpfr_init2 (t, 67); /* see the argument at the end of the following loop */
   mpfr_init2 (s, 67);
   mpfr_init2 (c, 67);
   mpq_init (kn);
   mpq_set_ui (kn, k, n);
   mpq_mul_2exp (kn, kn, 1); /* kn=2*k/n < 2 */

   do {
      prec += mpc_ceil_log2 (prec) + 5;

      mpfr_set_prec (t, prec);
      mpfr_set_prec (s, prec);
      mpfr_set_prec (c, prec);

      mpfr_const_pi (t, MPFR_RNDN); /* error <= 0.5 ulp but since
                                       ulp(t)=2^(2-prec), the absolute error
                                       is bounded by 2^(1-prec), and the
                                       relative error is bounded by
                                       2^(1-prec)/pi <= 0.64*2^(-prec) */
      mpfr_mul_q (t, t, kn, MPFR_RNDN); /* error <= 1.15 ulp(t) */
      /* Indeed, the error is bounded by 0.64*2^(-prec)*pi*kn + 0.5 ulp [1].
         Applying 2^(-prec)*|x| <= ulp(x) to x=pi*kn, we get a bound of:
         0.64*ulp(pi*kn)+0.5 ulp.
         Now since ulp(pi*kn) <= 2*ulp(t), we get: |t-pi*kn| <= 1.78*ulp(t).
         If we plug this into [1] we get:
         0.64*2^(-prec)*t + 0.64*2^(-prec)*1.78*ulp(t) + 0.5 ulp(t)
         <= 0.64*ulp(t) + 0.64*2^(-prec)*1.78*ulp(t) + 0.5 ulp(t)
         <= 1.15*ulp(t) for prec >= 7. */

      mpfr_sin_cos (s, c, t, MPFR_RNDN);
         /* error (1.5*2^{Exp (t) - Exp (s resp. c)} + 0.5) ulp
            We have 0<t<2*pi, so Exp (t) <= 3.
            Unfortunately s or c can be close to 0, but with n<2^64,
            we lose at most about 64 bits:
            Where the minimum of s and c over all primitive n-th roots of
            unity is reached depends on n mod 4.
            To simplify the argument, we will consider the 4*n-th roots of
            unity, which contain the n-th roots of unity and which are
            symmetrically distributed with respect to the real and imaginary
            axes, so that it becomes enough to consider only s for k=1.
            With n<2^64, the absolute values of all s or c are at least
            sin (2 * pi * 2^(-64) / 4) > 2^(-64) of exponent at least -63.
            So the error is bounded above by
            (1.5*2^66+0.5) ulp < 2^67 ulp.
            To obtain a more precise bound for smaller n, which is useful
            especially at small precision, we may use the error bound of
            (1.5*2^(3 - Exp (s or c)) + 0.5) ulp
            < 2^(4 - Exp (s or c)) ulp, since Exp (s or c) is at most 0. */

   }
   while (   !mpfr_can_round (c, prec - (4 - mpfr_get_exp (c)),
                 MPFR_RNDN, MPFR_RNDZ,
                 MPC_PREC_RE(rop) + (MPC_RND_RE(rnd) == MPFR_RNDN))
          || !mpfr_can_round (s, prec - (4 - mpfr_get_exp (s)),
                 MPFR_RNDN, MPFR_RNDZ,
                 MPC_PREC_IM(rop) + (MPC_RND_IM(rnd) == MPFR_RNDN)));

   inex_re = mpfr_set (mpc_realref(rop), c, MPC_RND_RE(rnd));
   inex_im = mpfr_set (mpc_imagref(rop), s, MPC_RND_IM(rnd));

   mpfr_clear (t);
   mpfr_clear (s);
   mpfr_clear (c);
   mpq_clear (kn);

   return MPC_INEX(inex_re, inex_im);
}
