/* mpc_tan -- tangent of a complex number.

Copyright (C) 2008 Philippe Th\'eveny

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

#include "mpfr.h"
#include "mpc.h"
#include "mpc-impl.h"

void
mpc_tan (mpc_ptr rop, mpc_srcptr op, mpc_rnd_t rnd)
{
  mpc_t x, y, z;
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

     We use the following algorithm
     with rounding towards +infinity for all operations, and working precision w:

     (1) x = N(sin(op))
     (2) y = N(cos(op))
     (3) z = N(x/y)

     the error on Im(z) is at most 61 ulp,
     the error on Re(z) is at most
     7 ulp if k < 3,
     else 5+k ulp, where
     k = Exp(Re(x))+Exp(Re(y))-2min{Exp(Re(y)), Exp(Im(y))}-Exp(Re(x/y))
     see proof in algorithms.tex.
  */

  MPC_LOG_VAR (op);
  MPC_LOG_RND (rnd);

  prec = MPC_MAX_PREC(rop);

  mpc_init2 (x, 2);
  mpc_init2 (y, 2);
  mpc_init2 (z, 2);

  err = 7;

  do
    {
      prec += mpc_ceil_log2 (prec) + err;

      MPC_LOG_MSG ("loop prec=%ld", prec);

      mpc_set_prec (x, prec);
      mpc_set_prec (y, prec);
      mpc_set_prec (z, prec);

      mpc_sin (x, op, MPC_RNDUU);
      mpc_cos (y, op, MPC_RNDUU);
      mpc_div (z, x, y, MPC_RNDUU);

      err = 5 + MAX(2, mpfr_get_exp (MPC_RE (x)) + mpfr_get_exp (MPC_RE (y))
                    - mpfr_get_exp (MPC_IM (y)) - mpfr_get_exp (MPC_RE (z)));

      /* TODO: prove that overflow occurs only when the exact result has no
         floating point representation. For the time being, do as if it were
         true. */
      ok = mpfr_inf_p (MPC_RE (z))
        ||mpfr_can_round (MPC_RE(z), prec - err, GMP_RNDU, MPC_RND_RE(rnd),
                          MPFR_PREC(MPC_RE(rop)));

      if (ok) /* compute imaginary part */
        {
          ok = mpfr_inf_p (MPC_RE (z))
            || mpfr_can_round (MPC_IM(z), prec - 6, GMP_RNDU, MPC_RND_IM(rnd),
                               MPFR_PREC(MPC_IM(rop)));
        }
      MPC_LOG_MSG ("err: %ld", err);
      MPC_LOG_VAR (z);
    }
  while (ok == 0);

  mpc_set (rop, z, rnd);

  MPC_LOG_VAR (rop);

  mpc_clear (x);
  mpc_clear (y);
  mpc_clear (z);
}
