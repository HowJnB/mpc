/* mpc_norm -- Square of the norm of a complex number.

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

int
mpc_norm (mpfr_ptr a, mpc_srcptr b, mp_rnd_t rnd)
{
  mpfr_t u, v;
  mp_prec_t prec;
  int inexact;

  prec = MPFR_PREC(a);

  mpfr_init (u);
  mpfr_init (v);

  if (!MPFR_IS_ZERO(MPC_RE(b)) && !MPFR_IS_ZERO(MPC_IM(b)) &&
      2 * SAFE_ABS (mp_exp_t, MPFR_EXP (MPC_RE (b)) - MPFR_EXP (MPC_IM (b)))
       > (unsigned int) prec)
    /* If real and imaginary part have very different magnitudes, then the */
    /* generic code increases the precision too much. Instead, compute the */
    /* squarings _exactly_.                                                */
  {
     mpfr_set_prec (u, 2 * MPFR_PREC (MPC_RE (b)));
     mpfr_set_prec (v, 2 * MPFR_PREC (MPC_IM (b)));
     mpfr_sqr (u, MPC_RE (b), GMP_RNDN);
     mpfr_sqr (v, MPC_IM (b), GMP_RNDN);
     inexact = mpfr_add (a, u, v, rnd);
  }
  else
  {
    do
      {
        prec += mpc_ceil_log2 (prec) + 3;

        mpfr_set_prec (u, prec);
        mpfr_set_prec (v, prec);

        /* first compute norm(b)^2 */
        inexact = mpfr_sqr (u, MPC_RE(b), GMP_RNDN); /* err<=1/2ulp */
        inexact |= mpfr_sqr (v, MPC_IM(b), GMP_RNDN); /* err<=1/2ulp*/

        inexact |= mpfr_add (u, u, v, GMP_RNDN);            /* err <= 3/2 ulps */
      }
    while (inexact != 0 &&
           mpfr_can_round (u, prec - 2, GMP_RNDN, rnd, MPFR_PREC(a)) == 0);

    inexact |= mpfr_set (a, u, rnd);
  }
  mpfr_clear (u);
  mpfr_clear (v);

  return inexact;
}
