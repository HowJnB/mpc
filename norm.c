/* mpc_norm -- Square of the norm of a complex number.

Copyright (C) 2002 Andreas Enge, Paul Zimmermann

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

  do
    {
      prec += _mpfr_ceil_log2 ((double) prec) + 3;

      mpfr_set_prec (u, prec);
      mpfr_set_prec (v, prec);

      /* first compute norm(b)^2 */
      inexact = mpfr_mul (u, MPC_RE(b), MPC_RE(b), GMP_RNDN); /* err<=1/2ulp */
      inexact |= mpfr_mul (v, MPC_IM(b), MPC_IM(b), GMP_RNDN); /* err<=1/2ulp*/

      inexact |= mpfr_add (u, u, v, GMP_RNDN);            /* err <= 3/2 ulps */
    }
  while (inexact != 0 &&
         mpfr_can_round (u, prec - 2, GMP_RNDN, rnd, MPFR_PREC(a)) == 0);

  inexact |= mpfr_set (a, u, rnd);

  mpfr_clear (u);
  mpfr_clear (v);

  return inexact;
}
