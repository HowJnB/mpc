/* mpc_sqr -- Square a complex number.

Copyright (C) 2002, 2005 Andreas Enge, Paul Zimmermann

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

#define INV_RND(r) \
   (((r) == GMP_RNDU) ? GMP_RNDD : (((r) == GMP_RNDD) ? GMP_RNDU : (r)))

int
mpc_sqr (mpc_ptr a, mpc_srcptr b, mpc_rnd_t rnd)
{
   int ok;
   mpfr_t u, v;
   mpfr_t x;
      /* a temporary variable to hold the real part of b,
         needed in the case a==b */
   mp_prec_t prec;
   int inex_re, inex_im, inexact;

   prec = MPC_MAX_PREC(a);

   /* first check for real resp. purely imaginary number */
   if (MPFR_IS_ZERO (MPC_IM(b)))
   {
      inex_re = mpfr_mul (MPC_RE(a), MPC_RE(b), MPC_RE(b), MPC_RND_RE(rnd));
      inex_im = mpfr_set_ui (MPC_IM(a), 0, GMP_RNDN);
      return MPC_INEX(inex_re, inex_im);
   }
   if (MPFR_IS_ZERO (MPC_RE(b)))
   {
      inex_re = -mpfr_mul (MPC_RE(a), MPC_IM(b), MPC_IM(b),
                           INV_RND (MPC_RND_RE(rnd)));
      mpfr_neg (MPC_RE(a), MPC_RE(a), GMP_RNDN);
      inex_im = mpfr_set_ui (MPC_IM(a), 0, GMP_RNDN);
      return MPC_INEX(inex_re, inex_im);
   }
   /* If the real and imaginary part of the argument have a very different */
   /* exponent, it is not reasonable to use Karatsuba squaring; compute    */
   /* exactly with the standard formulae instead, even if this means an    */
   /* additional multiplication.                                           */
   if (SAFE_ABS (mp_exp_t, MPFR_EXP (MPC_RE (b)) - MPFR_EXP (MPC_IM (b)))
       > MPC_MAX_PREC (b) / 2)
   {
      mpfr_init2 (u, 2*MPFR_PREC (MPC_RE (b)));
      mpfr_init2 (v, 2*MPFR_PREC (MPC_IM (b)));
      
      mpfr_sqr (u, MPC_RE (b), GMP_RNDN);
      mpfr_sqr (v, MPC_IM (b), GMP_RNDN); /* both are exact */
      inex_im = mpfr_mul (a->im, b->re, b->im, MPC_RND_IM (rnd));
      mpfr_mul_2exp (a->im, a->im, 1, GMP_RNDN);
      inex_re = mpfr_sub (a->re, u, v, MPC_RND_RE (rnd));
      
      mpfr_clear (u);
      mpfr_clear (v);
      return MPC_INEX (inex_re, inex_im);
   }
   

   mpfr_init (u);
   mpfr_init (v);

   if (a == b)
   {
      mpfr_init2 (x, MPFR_PREC (b->re));
      mpfr_set (x, b->re, GMP_RNDN);
   }
   else
      x [0] = b->re [0];

   do
   {
      prec += mpc_ceil_log2 (prec) + 5;

      mpfr_set_prec (u, prec);
      mpfr_set_prec (v, prec);

      /* Let b = x + iy. We need u = x+y and v = x-y, rounded away.       */
      /* As this is not implemented in mpfr, we round towards zero and    */
      /* add one ulp if the result is not exact. The error is bounded     */
      /* above by 1 ulp.                                                  */
      /* We first let inexact be 1 if the real part is not computed       */
      /* exactly and determine the sign later.                            */
      inexact = 0;
      if (mpfr_add (u, x, MPC_IM (b), GMP_RNDZ))
         /* we have to use x here instead of MPC_RE (b), as MPC_RE (a)
            may be modified later in the attempt to assign u to it */
      {
         mpfr_add_one_ulp (u, GMP_RNDN);
         inexact = 1;
      }
      if (mpfr_sub (v, x, MPC_IM (b), GMP_RNDZ))
      {
         mpfr_add_one_ulp (v, GMP_RNDN);
         inexact = 1;
      }

      /* compute the real part as u*v, rounded away                    */
      /* determine also the sign of inex_re                            */
      if (mpfr_sgn (u) == 0 || mpfr_sgn (v) == 0)
      {
         /* as we have rounded away, the result is exact */
         mpfr_set_ui (u, 0, GMP_RNDN);
         mpfr_set_ui (MPC_RE (a), 0, GMP_RNDN);
         inex_re = 0;
         ok = 1;
      }
      else if (mpfr_sgn (u) * mpfr_sgn (v) > 0)
      {
         inexact |= mpfr_mul (u, u, v, GMP_RNDU); /* error 5 */
         ok = !inexact | mpfr_can_round (u, prec - 3, GMP_RNDU,
                            MPC_RND_RE (rnd), MPFR_PREC (MPC_RE (a)));
         if (ok)
         {
            inex_re = mpfr_set (MPC_RE (a), u, MPC_RND_RE (rnd));
            if (inex_re == 0)
               /* remember that u was already rounded */
               inex_re = inexact;
            /* even if rounding did work, we might not know whether the
               result is too large, too small or exact */
            else if (inexact && MPC_RND_RE (rnd) == GMP_RNDN
                && inex_re < 0
                && !mpfr_can_round (u, prec - 3, GMP_RNDU, GMP_RNDU,
                                    MPFR_PREC (MPC_RE (a))))
               ok = 0;
         }
      }
      else
      {
         inexact |= mpfr_mul (u, u, v, GMP_RNDD); /* error 5 */
         ok = !inexact | mpfr_can_round (u, prec - 3, GMP_RNDD,
                              MPC_RND_RE (rnd), MPFR_PREC (MPC_RE (a)));
         if (ok)
         {
            inex_re = mpfr_set (MPC_RE (a), u, MPC_RND_RE (rnd));
            if (inex_re == 0)
               inex_re = inexact;
            /* even if rounding did work, we might not know whether the
               result is too large, too small or exact */
            else if (inexact && MPC_RND_RE (rnd) == GMP_RNDN
                && inex_re > 0
                && !mpfr_can_round (u, prec - 3, GMP_RNDD, GMP_RNDD,
                                    MPFR_PREC (MPC_RE (a))))
               ok = 0;
         }
      }
   }
   while (!ok);
      
   /* compute the imaginary part as 2*x*y, which is always possible */
   inex_im = mpfr_mul (MPC_IM (a), x, MPC_IM (b),
                          MPC_RND_IM (rnd));
   mpfr_set_exp (MPC_IM (a), mpfr_get_exp (MPC_IM (a)) + 1);

   mpfr_clear (u);
   mpfr_clear (v);

   if (a == b)
      mpfr_clear (x);

   return MPC_INEX (inex_re, inex_im);
}
