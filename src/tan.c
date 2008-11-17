/* mpc_tan -- tangent of a complex number.

Copyright (C) 2008 Philippe Th\'eveny, Andreas Enge

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
#include "mpfr.h"
#include "mpc.h"
#include "mpc-impl.h"

void
mpc_tan (mpc_ptr rop, mpc_srcptr op, mpc_rnd_t rnd)
{
  mpc_t x, y;
  mp_prec_t prec;
  mp_exp_t err;
  int ok = 0;

  /* special values */
  if (!mpfr_number_p (MPC_RE (op)) || !mpfr_number_p (MPC_IM (op)))
    {
      if (mpfr_nan_p (MPC_RE (op)))
        {
          if (mpfr_inf_p (MPC_IM (op)))
            /* tan(NaN -i*Inf) = +/-0 -i */
            /* tan(NaN +i*Inf) = +/-0 +i */
            {
              mpc_set_si_si (rop, 0,
                             (MPFR_SIGN (MPC_IM (op)) < 0) ? -1 : +1, rnd);
            }
          else
            /* tan(NaN +i*y) = NaN +i*NaN, when y is finite */
            /* tan(NaN +i*NaN) = NaN +i*NaN */
            {
              mpfr_set_nan (MPC_RE (rop));
              mpfr_set_nan (MPC_IM (rop));
            }

          return;
        }

      if (mpfr_nan_p (MPC_IM (op)))
        {
          if (mpfr_cmp_ui (MPC_RE (op), 0) == 0)
            /* tan(-0 +i*NaN) = -0 +i*NaN */
            /* tan(+0 +i*NaN) = +0 +i*NaN */
            {
              mpc_set (rop, op, rnd);
            }
          else
            /* tan(x +i*NaN) = NaN +i*NaN, when x != 0 */
            {
              mpfr_set_nan (MPC_RE (rop));
              mpfr_set_nan (MPC_IM (rop));
            }

          return;
        }

      if (mpfr_inf_p (MPC_RE (op)))
        {
          if (mpfr_inf_p (MPC_IM (op)))
            /* tan(-Inf -i*Inf) = -/+0 -i */
            /* tan(-Inf +i*Inf) = -/+0 +i */
            /* tan(+Inf -i*Inf) = +/-0 -i */
            /* tan(+Inf +i*Inf) = +/-0 +i */
            {
              const int sign_re = mpfr_signbit (MPC_RE (op));

              mpfr_set_ui (MPC_RE (rop), 0, MPC_RND_RE (rnd));
              mpfr_setsign (MPC_RE (rop), MPC_RE (rop), sign_re, MPC_RNDNN);
              mpfr_set_si (MPC_IM (rop), mpfr_signbit (MPC_IM (op)) ? -1 : +1,
                           MPC_RND_IM (rnd));
            }
          else
            /* tan(-Inf +i*y) = tan(+Inf +i*y) = NaN +i*NaN, when y is
               finite */
            {
              mpfr_set_nan (MPC_RE (rop));
              mpfr_set_nan (MPC_IM (rop));
            }

          return;
        }

      {
        /* tan(x -i*Inf) = +0*sin(x)*cos(x) -i, when x is finite */
        /* tan(x +i*Inf) = +0*sin(x)*cos(x) +i, when x is finite */
        mpfr_t c;
        mpfr_t s;

        mpfr_init (c);
        mpfr_init (s);

        mpfr_sin_cos (s, c, MPC_RE (op), GMP_RNDN);
        mpfr_set_ui (MPC_RE (rop), 0, MPC_RND_RE (rnd));
        mpfr_setsign (MPC_RE (rop), MPC_RE (rop),
                      mpfr_signbit (c) != mpfr_signbit (s), MPC_RNDNN);
        mpfr_set_si (MPC_IM (rop), (mpfr_signbit (MPC_IM (op)) ? -1 : +1),
                     MPC_RND_IM (rnd));

        mpfr_clear (s);
        mpfr_clear (c);
      }

      return;
    }

  if (mpfr_zero_p (MPC_RE (op)))
    {
      /* tan(-0 -i*y) = -0 +i*tanh(y), when y is finite. */
      /* tan(+0 +i*y) = +0 +i*tanh(y), when y is finite. */
      mpfr_set (MPC_RE (rop), MPC_RE (op), MPC_RND_RE (rnd));
      mpfr_tanh (MPC_IM (rop), MPC_IM (op), MPC_RND_IM (rnd));

      return;
    }

  if (mpfr_zero_p (MPC_IM (op)))
    {
      /* tan(x -i*0) = tan(x) -i*0, when x is finite. */
      /* tan(x +i*0) = tan(x) +i*0, when x is finite. */
      mpfr_tan (MPC_RE (rop), MPC_RE (op), MPC_RND_RE (rnd));
      mpfr_set (MPC_IM (rop), MPC_IM (op), MPC_RND_IM (rnd));

      return;
    }

  /* ordinary (non-zero) numbers */

  /* tan(op) = sin(op) / cos(op).

     We use the following algorithm with rounding away from 0 for all
     operations, and working precision w:

     (1) x = A(sin(op))
     (2) y = A(cos(op))
     (3) z = A(x/y)

     the error on Im(z) is at most 81 ulp,
     the error on Re(z) is at most
     7 ulp if k < 2,
     8 ulp if k = 2,
     else 5+k ulp, where
     k = Exp(Re(x))+Exp(Re(y))-2min{Exp(Re(y)), Exp(Im(y))}-Exp(Re(x/y))
     see proof in algorithms.tex.
  */

  MPC_LOG_VAR (op);
  MPC_LOG_RND (rnd);

  prec = MPC_MAX_PREC(rop);

  mpc_init2 (x, 2);
  mpc_init2 (y, 2);

  err = 7;

  do
    {
      mp_exp_t k;
      mp_exp_t exr, eyr, eyi, ezr;
      int inex;

      ok = 0;

      /* FIXME: prevent addition overflow */
      prec += mpc_ceil_log2 (prec) + err;
      mpc_set_prec (x, prec);
      mpc_set_prec (y, prec);

      MPC_LOG_MSG (("loop prec=%ld", prec));

      /* rounding away from zero: except in the cases x=0 or y=0 (processed
         above), sin x and cos y are never exact, so rounding away from 0 is
         rounding towards 0 and adding one ulp to the absolute value */
      mpc_sin (x, op, MPC_RNDZZ);
      mpfr_signbit (MPC_RE (x)) ?
        mpfr_nextbelow (MPC_RE (x)) : mpfr_nextabove (MPC_RE (x));
      mpfr_signbit (MPC_IM (x)) ?
        mpfr_nextbelow (MPC_IM (x)) : mpfr_nextabove (MPC_IM (x));
      exr = mpfr_get_exp (MPC_RE (x));
      mpc_cos (y, op, MPC_RNDZZ);
      mpfr_signbit (MPC_RE (y)) ?
        mpfr_nextbelow (MPC_RE (y)) : mpfr_nextabove (MPC_RE (y));
      mpfr_signbit (MPC_IM (y)) ?
        mpfr_nextbelow (MPC_IM (y)) : mpfr_nextabove (MPC_IM (y));
      eyr = mpfr_get_exp (MPC_RE (y));
      eyi = mpfr_get_exp (MPC_IM (y));

      /* some parts of the quotient may be exact */
      inex = mpc_div (x, x, y, MPC_RNDZZ);
      /* OP is no pure real nor pure imaginary, so in theory the real and
         imaginary parts of its tangent cannot be null. However due to
         rouding errors this might happen. Consider for example
         tan(1+14*I) = 1.26e-10 + 1.00*I. For small precision sin(op) and
         cos(op) differ only by a factor I, thus after mpc_div x = I and
         its real part is zero. */
      if (mpfr_zero_p (MPC_RE (x)) || mpfr_zero_p (MPC_IM (x)))
        {
          err = prec; /* double precision */
          continue;
        }
      if (MPC_INEX_RE (inex))
        mpfr_signbit (MPC_RE (x)) ?
          mpfr_nextbelow (MPC_RE (x)) : mpfr_nextabove (MPC_RE (x));
      if (MPC_INEX_IM (inex))
        mpfr_signbit (MPC_IM (x)) ?
          mpfr_nextbelow (MPC_IM (x)) : mpfr_nextabove (MPC_IM (x));
      ezr = mpfr_get_exp (MPC_RE (x));

      /* FIXME: compute
         k = Exp(Re(x))+Exp(Re(y))-2min{Exp(Re(y)), Exp(Im(y))}-Exp(Re(x/y))
         avoiding overflow */
      k = exr - ezr + MAX(-eyr, eyr - 2 * eyi);
      err = k < 2 ? 7 : (k == 2 ? 8 : (5 + k));

      /* Can the real part be rounded ? */
      ok = mpfr_inf_p (MPC_RE (x))
        || mpfr_can_round (MPC_RE(x), prec - err, GMP_RNDN,
                           MPC_RND_RE(rnd), MPFR_PREC(MPC_RE(rop)));

      if (ok)
        {
          /* Can the imaginary part be rounded ? */
          ok = mpfr_inf_p (MPC_IM (x))
            || mpfr_can_round (MPC_IM(x), prec - 6, GMP_RNDN, MPC_RND_IM(rnd),
                               MPFR_PREC(MPC_IM(rop)));
        }
      MPC_LOG_MSG (("err: %ld", err));
      MPC_LOG_VAR (x);
    }
  while (ok == 0);

  mpc_set (rop, x, rnd);

  MPC_LOG_VAR (rop);

  mpc_clear (x);
  mpc_clear (y);
}
