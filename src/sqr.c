/* mpc_sqr -- Square a complex number.

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

#include <stdio.h>    /* for MPC_ASSERT */
#include "mpc-impl.h"

int
mpc_sqr (mpc_ptr rop, mpc_srcptr op, mpc_rnd_t rnd)
{
   int ok;
   mpfr_t u, v;
   mpfr_t x;
      /* rop temporary variable to hold the real part of op,
         needed in the case rop==op */
   mpfr_prec_t prec;
   int inex_re, inex_im, inexact;
   mpfr_exp_t emin, emax;

   /* special values: NaN and infinities */
   if (!mpc_fin_p (op)) {
      if (mpfr_nan_p (mpc_realref (op)) || mpfr_nan_p (mpc_imagref (op))) {
         mpfr_set_nan (mpc_realref (rop));
         mpfr_set_nan (mpc_imagref (rop));
      }
      else if (mpfr_inf_p (mpc_realref (op))) {
         if (mpfr_inf_p (mpc_imagref (op))) {
            mpfr_set_inf (mpc_imagref (rop),
                          MPFR_SIGN (mpc_realref (op)) * MPFR_SIGN (mpc_imagref (op)));
            mpfr_set_nan (mpc_realref (rop));
         }
         else {
            if (mpfr_zero_p (mpc_imagref (op)))
               mpfr_set_nan (mpc_imagref (rop));
            else
               mpfr_set_inf (mpc_imagref (rop),
                             MPFR_SIGN (mpc_realref (op)) * MPFR_SIGN (mpc_imagref (op)));
            mpfr_set_inf (mpc_realref (rop), +1);
         }
      }
      else /* IM(op) is infinity, RE(op) is not */ {
         if (mpfr_zero_p (mpc_realref (op)))
            mpfr_set_nan (mpc_imagref (rop));
         else
            mpfr_set_inf (mpc_imagref (rop),
                          MPFR_SIGN (mpc_realref (op)) * MPFR_SIGN (mpc_imagref (op)));
         mpfr_set_inf (mpc_realref (rop), -1);
      }
      return MPC_INEX (0, 0); /* exact */
   }

   prec = MPC_MAX_PREC(rop);

   /* first check for real resp. purely imaginary number */
   if (mpfr_zero_p (mpc_imagref(op)))
   {
      int same_sign = mpfr_signbit (mpc_realref (op)) == mpfr_signbit (mpc_imagref (op));
      inex_re = mpfr_sqr (mpc_realref(rop), mpc_realref(op), MPC_RND_RE(rnd));
      inex_im = mpfr_set_ui (mpc_imagref(rop), 0ul, GMP_RNDN);
      if (!same_sign)
        mpc_conj (rop, rop, MPC_RNDNN);
      return MPC_INEX(inex_re, inex_im);
   }
   if (mpfr_zero_p (mpc_realref(op)))
   {
      int same_sign = mpfr_signbit (mpc_realref (op)) == mpfr_signbit (mpc_imagref (op));
      inex_re = -mpfr_sqr (mpc_realref(rop), mpc_imagref(op), INV_RND (MPC_RND_RE(rnd)));
      mpfr_neg (mpc_realref(rop), mpc_realref(rop), GMP_RNDN);
      inex_im = mpfr_set_ui (mpc_imagref(rop), 0ul, GMP_RNDN);
      if (!same_sign)
        mpc_conj (rop, rop, MPC_RNDNN);
      return MPC_INEX(inex_re, inex_im);
   }
   /* If the real and imaginary parts of the argument have very different  */
   /* exponents, it is not reasonable to use Karatsuba squaring; compute   */
   /* exactly with the standard formulae instead, even if this means an    */
   /* additional multiplication.                                           */
   if (SAFE_ABS (mpfr_exp_t,
                 mpfr_get_exp (mpc_realref (op)) - mpfr_get_exp (mpc_imagref (op)))
       > (mpfr_exp_t) MPC_MAX_PREC (op) / 2)
   {
      mpfr_init2 (u, 2*MPC_PREC_RE (op));
      mpfr_init2 (v, 2*MPC_PREC_IM (op));

      mpfr_sqr (u, mpc_realref (op), GMP_RNDN);
      mpfr_sqr (v, mpc_imagref (op), GMP_RNDN); /* both are exact */
      inex_im = mpfr_mul (rop->im, op->re, op->im, MPC_RND_IM (rnd));
      mpfr_mul_2exp (rop->im, rop->im, 1, GMP_RNDN);
      inex_re = mpfr_sub (rop->re, u, v, MPC_RND_RE (rnd));

      mpfr_clear (u);
      mpfr_clear (v);
      return MPC_INEX (inex_re, inex_im);
   }


   mpfr_init (u);
   mpfr_init (v);

   if (rop == op)
   {
      mpfr_init2 (x, MPC_PREC_RE (op));
      mpfr_set (x, op->re, GMP_RNDN);
   }
   else
      x [0] = op->re [0];

   emax = mpfr_get_emax ();
   emin = mpfr_get_emin ();

   do
   {
      prec += mpc_ceil_log2 (prec) + 5;

      mpfr_set_prec (u, prec);
      mpfr_set_prec (v, prec);

      /* Let op = x + iy. We need u = x+y and v = x-y, rounded away.      */
      /* The error is bounded above by 1 ulp.                             */
      /* We first let inexact be 1 if the real part is not computed       */
      /* exactly and determine the sign later.                            */
      inexact =    ROUND_AWAY (mpfr_add (u, x, mpc_imagref (op), MPFR_RNDA), u)
                 | ROUND_AWAY (mpfr_sub (v, x, mpc_imagref (op), MPFR_RNDA), v);

      /* compute the real part as u*v, rounded away                    */
      /* determine also the sign of inex_re                            */
      if (mpfr_sgn (u) == 0 || mpfr_sgn (v) == 0)
        {
          /* as we have rounded away, the result is exact */
          mpfr_set_ui (mpc_realref (rop), 0, GMP_RNDN);
          inex_re = 0;
          ok = 1;
        }
      else if (mpfr_sgn (u) * mpfr_sgn (v) > 0)
        {
          inexact |= mpfr_mul (u, u, v, GMP_RNDU); /* error 5 */
          /* checks that no overflow nor underflow occurs: since u*v > 0
             and we round up, an overflow will give +Inf, but an underflow
             will give 0.5*2^emin */
          MPC_ASSERT (mpfr_get_exp (u) != emin);
          if (mpfr_inf_p (u))
            {
              /* let mpc_realref(rop) be a "correctly rounded overflow" */
              inex_re = mpfr_set_ui_2exp (mpc_realref (rop), 1, emax, MPC_RND_RE (rnd));
              break;
            }
          ok = (!inexact) | mpfr_can_round (u, prec - 3, GMP_RNDU, GMP_RNDZ,
               MPC_PREC_RE (rop) + (MPC_RND_RE (rnd) == GMP_RNDN));
          if (ok)
            {
              inex_re = mpfr_set (mpc_realref (rop), u, MPC_RND_RE (rnd));
              if (inex_re == 0)
                /* remember that u was already rounded */
                inex_re = inexact;
            }
        }
      else
        {
          mpfr_rnd_t rnd_re = MPC_RND_RE (rnd);
          inexact |= mpfr_mul (u, u, v, GMP_RNDD); /* error 5 */
          /* if an overflow occurs: since u*v < 0 and we round down,
             the result is -Inf or -MAXDBL */
          if (mpfr_inf_p (u))
            {
              /* replace by "correctly rounded overflow" */
              mpfr_set_si (u, -1, GMP_RNDN);
              mpfr_mul_2ui (u, u, mpfr_get_emax (), rnd_re);
              ok = 1;
            }
          else
            {
              /* if an underflow happens (i.e., u = -0.5*2^emin since we round
                 away from zero), the result will be an underflow */
              if (mpfr_get_exp (u) == emin)
                {
                  if (rnd_re == GMP_RNDZ || rnd_re == GMP_RNDN ||
                      rnd_re == GMP_RNDU)
                    {
                      mpfr_set_ui (mpc_realref (rop), 0, rnd_re);
                      inex_re = 1;
                    }
                  else /* round down or away from zero */ {
                    mpfr_set (mpc_realref (rop), u, rnd_re);
                    inex_re = -1;
                  }
                  break;
                }
              ok = (!inexact) | mpfr_can_round (u, prec - 3, GMP_RNDD,
                 GMP_RNDZ, MPC_PREC_RE (rop) + (MPC_RND_RE (rnd) == GMP_RNDN));
            }
          if (ok)
            {
              inex_re = mpfr_set (mpc_realref (rop), u, MPC_RND_RE (rnd));
              if (inex_re == 0)
                inex_re = inexact;
            }
        }
   }
   while (!ok);

   /* compute the imaginary part as 2*x*y, which is always possible */
   if (mpfr_get_exp (x) + mpfr_get_exp(mpc_imagref (op)) <= emin + 1)
     {
       mpfr_mul_2ui (x, x, 1, GMP_RNDN);
       inex_im = mpfr_mul (mpc_imagref (rop), x, mpc_imagref (op), MPC_RND_IM (rnd));
     }
   else
     {
       inex_im = mpfr_mul (mpc_imagref (rop), x, mpc_imagref (op), MPC_RND_IM (rnd));
       /* checks that no underflow occurs (an overflow can occur here) */
       MPC_ASSERT (mpfr_zero_p (mpc_imagref (rop)) == 0);
       mpfr_mul_2ui (mpc_imagref (rop), mpc_imagref (rop), 1, GMP_RNDN);
     }

   mpfr_clear (u);
   mpfr_clear (v);

   if (rop == op)
      mpfr_clear (x);

   return MPC_INEX (inex_re, inex_im);
}
