/* mpc_log10 -- Take the base-10 logarithm of a complex number.

Copyright (C) 2012 INRIA

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

/* Auxiliary functions which implement Ziv's strategy for special cases.
   if flag = 0: compute only real part
   if flag = 1: compute only imaginary
   Exact cases should be dealt with separately. */
static int
mpc_log10_aux (mpc_ptr rop, mpc_srcptr op, mpc_rnd_t rnd, int flag, int nb)
{
  mp_prec_t prec = (MPFR_PREC_MIN > 4) ? MPFR_PREC_MIN : 4;
  mpc_t tmp;
  mpfr_t log10;
  int ok = 0, ret;

  prec = mpfr_get_prec ((flag == 0) ? mpc_realref (rop) : mpc_imagref (rop));
  prec += 10;
  mpc_init2 (tmp, prec);
  mpfr_init2 (log10, prec);
  while (ok == 0)
    {
      mpfr_set_ui (log10, 10, GMP_RNDN); /* exact since prec >= 4 */
      mpfr_log (log10, log10, GMP_RNDN);
      /* In each case we have two roundings, thus the final value is
         x * (1+u)^2 where x is the exact value, and |u| <= 2^(-prec-1).
         Thus the error is always less than 3 ulps. */
      switch (nb)
        {
        case 0: /* imag <- atan2(y/x) */
          mpfr_atan2 (mpc_imagref (tmp), mpc_imagref (op), mpc_realref (op),
                      MPC_RND_IM (rnd));
          mpfr_div (mpc_imagref (tmp), mpc_imagref (tmp), log10, GMP_RNDN);
          ok = mpfr_can_round (mpc_imagref (tmp), prec - 2, GMP_RNDZ,
                               GMP_RNDZ, MPC_PREC_IM(rop) +
                               MPC_RND_IM (rnd) == GMP_RNDN);
          if (ok)
            ret = mpfr_set (mpc_imagref (rop), mpc_imagref (tmp),
                            MPC_RND_IM (rnd));
          break;
        case 1: /* real <- log(x) */
          mpfr_log (mpc_realref (tmp), mpc_realref (op), MPC_RND_RE (rnd));
          mpfr_div (mpc_realref (tmp), mpc_realref (tmp), log10, GMP_RNDN);
          ok = mpfr_can_round (mpc_realref (tmp), prec - 2, GMP_RNDZ,
                               GMP_RNDZ, MPC_PREC_RE(rop) +
                               MPC_RND_RE (rnd) == GMP_RNDN);
          if (ok)
            ret = mpfr_set (mpc_realref (rop), mpc_realref (tmp),
                            MPC_RND_RE (rnd));
          break;
        case 2: /* imag <- pi */
          mpfr_const_pi (mpc_imagref (tmp), MPC_RND_IM (rnd));
          mpfr_div (mpc_imagref (tmp), mpc_imagref (tmp), log10, GMP_RNDN);
          ok = mpfr_can_round (mpc_imagref (tmp), prec - 2, GMP_RNDZ,
                               GMP_RNDZ, MPC_PREC_IM(rop) +
                               MPC_RND_IM (rnd) == GMP_RNDN);
          if (ok)
            ret = mpfr_set (mpc_imagref (rop), mpc_imagref (tmp),
                            MPC_RND_IM (rnd));
          break;
        case 3: /* real <- log(y) */
          mpfr_log (mpc_realref (tmp), mpc_imagref (op), MPC_RND_RE (rnd));
          mpfr_div (mpc_realref (tmp), mpc_realref (tmp), log10, GMP_RNDN);
          ok = mpfr_can_round (mpc_realref (tmp), prec - 2, GMP_RNDZ,
                               GMP_RNDZ, MPC_PREC_RE(rop) +
                               MPC_RND_RE (rnd) == GMP_RNDN);
          if (ok)
            ret = mpfr_set (mpc_realref (rop), mpc_realref (tmp),
                            MPC_RND_RE (rnd));
          break;
        }
      prec += prec / 2;
      mpc_set_prec (tmp, prec);
      mpfr_set_prec (log10, prec);
    }
  mpc_clear (tmp);
  mpfr_clear (log10);
  return ret;
}

int
mpc_log10 (mpc_ptr rop, mpc_srcptr op, mpc_rnd_t rnd)
{
  int ok = 0, loops = 0, re_cmp, im_cmp, inex_re, inex_im, negative_zero;
  mpfr_t w;
  mpfr_prec_t prec;
  mpfr_rnd_t rnd_im;
  mpc_t ww;
  mpc_rnd_t invrnd;

  /* special values: NaN and infinities: same as mpc_log */
  if (!mpc_fin_p (op)) /* real or imaginary parts are NaN or Inf */
    {
      if (mpfr_nan_p (mpc_realref (op)))
        {
          if (mpfr_inf_p (mpc_imagref (op)))
            /* (NaN, Inf) -> (+Inf, NaN) */
            mpfr_set_inf (mpc_realref (rop), +1);
          else
            /* (NaN, xxx) -> (NaN, NaN) */
            mpfr_set_nan (mpc_realref (rop));
         mpfr_set_nan (mpc_imagref (rop));
         inex_im = 0; /* Inf/NaN is exact */
        }
      else if (mpfr_nan_p (mpc_imagref (op)))
        {
          if (mpfr_inf_p (mpc_realref (op)))
            /* (Inf, NaN) -> (+Inf, NaN) */
            mpfr_set_inf (mpc_realref (rop), +1);
          else
            /* (xxx, NaN) -> (NaN, NaN) */
            mpfr_set_nan (mpc_realref (rop));
          mpfr_set_nan (mpc_imagref (rop));
          inex_im = 0; /* Inf/NaN is exact */
        }
      else /* We have an infinity in at least one part. */
        {
          /* (+Inf, y) -> (+Inf, 0) for finite positive-signed y */
          if (mpfr_inf_p (mpc_realref (op)) && mpfr_signbit (mpc_realref (op))
              == 0 && mpfr_number_p (mpc_imagref (op)))
            inex_im = mpfr_atan2 (mpc_imagref (rop), mpc_imagref (op),
                                  mpc_realref (op), MPC_RND_IM (rnd));
          else
            /* (xxx, Inf) -> (+Inf, atan2(Inf/xxx))
               (Inf, yyy) -> (+Inf, atan2(yyy/Inf)) */
            inex_im = mpc_log10_aux (rop, op, rnd, 1, 0);
          mpfr_set_inf (mpc_realref (rop), +1);
        }
      return MPC_INEX(0, inex_im);
    }

   /* special cases: real and purely imaginary numbers */
  re_cmp = mpfr_cmp_ui (mpc_realref (op), 0);
  im_cmp = mpfr_cmp_ui (mpc_imagref (op), 0);
  if (im_cmp == 0) /* Im(op) = 0 */
    {
      if (re_cmp == 0) /* Re(op) = 0 */
        {
          if (mpfr_signbit (mpc_realref (op)) == 0)
            inex_im = mpfr_atan2 (mpc_imagref (rop), mpc_imagref (op),
                                  mpc_realref (op), MPC_RND_IM (rnd));
          else
            inex_im = mpc_log10_aux (rop, op, rnd, 1, 0);
          mpfr_set_inf (mpc_realref (rop), -1);
          inex_re = 0; /* -Inf is exact */
        }
      else if (re_cmp > 0)
        {
          inex_re = mpfr_log10 (mpc_realref (rop), mpc_realref (op),
                                MPC_RND_RE (rnd));
          inex_im = mpfr_set (mpc_imagref (rop), mpc_imagref (op),
                              MPC_RND_IM (rnd));
        }
      else /* log10(x + 0*i) for negative x */
        { /* op = x + 0*i; let w = -x = |x| */
          negative_zero = mpfr_signbit (mpc_imagref (op));
          if (negative_zero)
            rnd_im = INV_RND (MPC_RND_IM (rnd));
          else
            rnd_im = MPC_RND_IM (rnd);
          ww->re[0] = *mpc_realref (op);
          MPFR_CHANGE_SIGN (ww->re);
          ww->im[0] = *mpc_imagref (op);
          if (mpfr_cmp_ui (ww->re, 1) == 0)
            inex_re = mpfr_set_ui (mpc_realref (rop), 0, MPC_RND_RE (rnd));
          else
            inex_re = mpc_log10_aux (rop, ww, rnd, 0, 1);
          inex_im = mpc_log10_aux (rop, op, RNDC(0,rnd_im), 1, 2);
          if (negative_zero)
            {
              mpc_conj (rop, rop, MPC_RNDNN);
              inex_im = -inex_im;
            }
        }
      return MPC_INEX(inex_re, inex_im);
    }
   else if (re_cmp == 0)
     {
       if (im_cmp > 0)
         {
           inex_re = mpc_log10_aux (rop, op, rnd, 0, 3);
           inex_im = mpc_log10_aux (rop, op, rnd, 1, 2);
           /* division by 2 does not change the ternary flag */
           mpfr_div_2ui (mpc_imagref (rop), mpc_imagref (rop), 1, GMP_RNDN);
         }
       else
         {
           ww->re[0] = *mpc_realref (op);
           ww->im[0] = *mpc_imagref (op);
           MPFR_CHANGE_SIGN (ww->im);
           inex_re = mpc_log10_aux (rop, ww, rnd, 0, 3);
           invrnd = RNDC(0, INV_RND (MPC_RND_IM (rnd)));
           inex_im = mpc_log10_aux (rop, op, invrnd, 1, 2);
           /* division by 2 does not change the ternary flag */
           mpfr_div_2ui (mpc_imagref (rop), mpc_imagref (rop), 1, GMP_RNDN);
           mpfr_neg (mpc_imagref (rop), mpc_imagref (rop), GMP_RNDN);
           inex_im = -inex_im; /* negate the ternary flag */
         }
       return MPC_INEX(inex_re, inex_im);
     }

  /* generic case: neither Re(op) nor Im(op) is NaN, Inf or zero */
  prec = MPC_PREC_RE(rop);
  mpfr_init2 (w, prec);
  mpc_init2 (ww, prec);
  /* let op = x + iy; compute log(op)/log(10) */
  while (ok == 0)
    {
      loops ++;
      prec += (loops <= 2) ? mpc_ceil_log2 (prec) + 4 : prec / 2;
      mpfr_set_prec (w, prec);
      mpc_set_prec (ww, prec);

      mpc_log (ww, op, MPC_RNDNN);
      mpfr_set_ui (w, 10, GMP_RNDN); /* exact since prec >= 4 */
      mpfr_log (w, w, GMP_RNDN);
      mpc_div_fr (ww, ww, w, MPC_RNDNN);

      ok = mpfr_can_round (mpc_realref (ww), prec - 2, GMP_RNDZ, GMP_RNDZ,
                           MPC_PREC_RE(rop) + MPC_RND_RE (rnd) == GMP_RNDN);
      ok = ok && mpfr_can_round (mpc_imagref (ww), prec-2, GMP_RNDZ, GMP_RNDZ,
                             MPC_PREC_IM(rop) + MPC_RND_IM (rnd) == GMP_RNDN);
    }

  inex_re = mpfr_set (mpc_realref(rop), mpc_realref (ww), MPC_RND_RE (rnd));
  inex_im = mpfr_set (mpc_imagref(rop), mpc_imagref (ww), MPC_RND_IM (rnd));
  mpfr_clear (w);
  mpc_clear (ww);
  return MPC_INEX(inex_re, inex_im);
}
