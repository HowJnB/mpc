/* mpc_abs -- Absolute value of a complex number.

Copyright (C) 2002, 2004, 2005 Andreas Enge, Paul Zimmermann

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

int
mpc_abs_basic (mpfr_ptr a, mpc_srcptr b, mp_rnd_t rnd)
{
  mpfr_t u, v;
  mp_prec_t prec;
  int inexact;

  prec = MPFR_PREC(a);

  mpfr_init2 (u, 2*prec);
  mpfr_init2 (v, 2*prec);

  do
    {
      prec += mpc_ceil_log2 (prec) + 3;

      mpfr_set_prec (u, prec);
      mpfr_set_prec (v, prec);

      /* first compute norm(b)^2 */
      inexact = mpfr_sqr (u, MPC_RE(b), GMP_RNDU); /* err<=1ulp */
      inexact |= mpfr_sqr (v, MPC_IM(b), GMP_RNDU); /* err<=1ulp*/

      inexact |= mpfr_add (u, u, v, GMP_RNDU);            /* err <= 3 ulps */
      inexact |= mpfr_sqrt (u, u, GMP_RNDU); /* err <= 4 ulps */
    }
  while (inexact != 0 &&
         mpfr_can_round (u, prec - 2, GMP_RNDU, rnd, MPFR_PREC(a)) == 0);

  inexact |= mpfr_set (a, u, rnd);

  mpfr_clear (u);
  mpfr_clear (v);

  return inexact;
}


int
mpc_abs (mpfr_ptr a, mpc_srcptr b, mp_rnd_t rnd)
   /* a more sophisticated version treating special cases separately */
{
  mpfr_t u, v;
  mp_prec_t prec;
  int inexact;

  if (MPFR_IS_ZERO (MPC_RE (b)))
     return mpfr_abs (a, MPC_IM (b), rnd);
  else if (MPFR_IS_ZERO (MPC_IM (b)))
     return mpfr_abs (a, MPC_RE (b), rnd);
  
  prec = MPFR_PREC(a);
  if (prec >= MPFR_PREC (MPC_RE (b))
        && 2 * (MPFR_EXP (MPC_RE (b)) - MPFR_EXP (MPC_IM (b)) - 1) >
           (signed long) prec)
     /* try to detect the case that the imaginary part is negligible;
        this is the case, independently of the rounding mode, whenever
        setting the result to the real part is exact, and the imaginary
        part contributes less than 1/2 ulp                              */
  {
     mpfr_abs (a, MPC_RE (b), GMP_RNDN); /* exact */
     if (rnd == GMP_RNDU)
     {
        mpfr_add_one_ulp (a, GMP_RNDN);
        return 1;
     }
     else
        return -1;
  }
  else if (prec >= MPFR_PREC (MPC_IM (b))
        && 2 * (MPFR_EXP (MPC_IM (b)) - MPFR_EXP (MPC_RE (b)) - 1) >
           (signed long) prec)
  {
     mpfr_abs (a, MPC_IM (b), GMP_RNDN); /* exact */
     if (rnd == GMP_RNDU)
     {
        mpfr_add_one_ulp (a, GMP_RNDN);
        return 1;
     }
     else
        return -1;
  }
  
  mpfr_init2 (u, 2*prec);
  mpfr_init2 (v, 2*prec);
  
  do
    {
      prec += mpc_ceil_log2 (prec) + 3;

      mpfr_set_prec (u, prec);
      mpfr_set_prec (v, prec);

      if (4*(MPFR_EXP(MPC_RE(b)) - MPFR_EXP(MPC_IM(b))) > (signed long) prec)
         /* |Re (b)| >> |Im (b)|, use the Taylor series sqrt (a^2 + b^2)
            = |a| + b^2 / (2 |a|) - e with 0 <= e <= 1 ulp of the target;
            so we round towards infinity                                 */
      {
         inexact = 1;
         mpfr_sqr (u, MPC_IM (b), GMP_RNDU); /* err <= 1 ulp */
         if (MPFR_SIGN (MPC_RE (b)) > 0)
         {
            mpfr_div (u, u, MPC_RE (b), GMP_RNDU); /* err <= 3 ulp */
            mpfr_div_2ui (u, u, 1, GMP_RNDU);
            mpfr_add (u, u, MPC_RE (b), GMP_RNDU); /* err <= 4 ulp */
         }
         else
         {
            mpfr_div (u, u, MPC_RE (b), GMP_RNDD);
            mpfr_div_2ui (u, u, 1, GMP_RNDD);
            mpfr_add (u, u, MPC_RE (b), GMP_RNDD);
            mpfr_neg (u, u, GMP_RNDU);
         }
         /* err <= 5 ulps (one for Taylor approximation) */
      }
      else if (4*(MPFR_EXP(MPC_IM(b)) - MPFR_EXP(MPC_RE(b))) >
               (signed long) prec)
      {
         inexact = 1;
         mpfr_sqr (u, MPC_RE (b), GMP_RNDU);
         if (MPFR_SIGN (MPC_IM (b)) > 0)
         {
            mpfr_div (u, u, MPC_IM (b), GMP_RNDU);
            mpfr_div_2ui (u, u, 1, GMP_RNDU);
            mpfr_add (u, u, MPC_IM (b), GMP_RNDU);
         }
         else
         {
            mpfr_div (u, u, MPC_IM (b), GMP_RNDD);
            mpfr_div_2ui (u, u, 1, GMP_RNDD);
            mpfr_add (u, u, MPC_IM (b), GMP_RNDD);
            mpfr_neg (u, u, GMP_RNDU);
         }
      }
      else
      {
         /* first compute norm(b)^2 */
         inexact = mpfr_sqr (u, MPC_RE(b), GMP_RNDU); /* err<=1ulp */
         inexact |= mpfr_sqr (v, MPC_IM(b), GMP_RNDU); /* err<=1ulp*/
         inexact |= mpfr_add (u, u, v, GMP_RNDU);            /* err <= 3 ulps */
         inexact |= mpfr_sqrt (u, u, GMP_RNDU); /* err <= 4 ulps */
      }
   }
  while (inexact != 0 &&
         mpfr_can_round (u, prec - 3, GMP_RNDU, rnd, MPFR_PREC(a)) == 0);

  inexact |= mpfr_set (a, u, rnd);

  mpfr_clear (u);
  mpfr_clear (v);

  return inexact;
}
