/* mpc_exp -- exponential of a complex number.

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

#include <stdio.h>

#include "gmp.h"
#include "mpfr.h"
#include "mpc.h"
#include "mpc-impl.h"

void
mpc_exp (mpc_ptr rop, mpc_srcptr op, mpc_rnd_t rnd)
{
  mpfr_t x, y, z;
  mp_prec_t prec;
  int ok = 0;

  /* let op = a + i*b, then exp(op) = exp(a)*[cos(b) + i*sin(b)]
                                    = exp(a)*cos(b) + i*exp(a)*sin(b).

     We use the following algorithm (same for the imaginary part):

     (1) x = o(exp(a)) rounded towards +infinity:
     (2) y = o(cos(b)) rounded to nearest
     (3) r = o(x*y)
     then the error on r for the real part is at most 4 ulps:
     |r - exp(a)*cos(b)| <= ulp(r) + |x*y - exp(a)*cos(b)|
                         <= ulp(r) + |x*y - exp(a)*y| + exp(a) * |y - cos(b)|
                         <= ulp(r) + |y| ulp(x) + 1/2 * x * ulp(y)
                         <= ulp(r) + 2 * ulp(x*y) + ulp(x*y) [Rule 4]
                         <= 4 * ulp(r) [Rule 8]
  */
  
  /* special case when the input is real */
  if (mpfr_cmp_ui (MPC_IM(op), 0) == 0)
    {
      mpfr_exp (MPC_RE(rop), MPC_RE(op), MPC_RND_RE(rnd));
      mpfr_set_ui (MPC_IM(rop), 0, MPC_RND_IM(rnd));
      return;
    }

  /* special case when the input is imaginary */
  if (mpfr_zero_p (MPC_RE (op)))
    {
      mpfr_cos (MPC_RE (rop), MPC_IM (op), MPC_RND_RE(rnd));
      mpfr_sin (MPC_IM (rop), MPC_IM (op), MPC_RND_IM(rnd));
      return;
    }

  prec = MPC_MAX_PREC(rop);

  mpfr_init2 (x, 2);
  mpfr_init2 (y, 2);
  mpfr_init2 (z, 2);

  do
    {
      prec += mpc_ceil_log2 (prec) + 5;

      mpfr_set_prec (x, prec);
      mpfr_set_prec (y, prec);
      mpfr_set_prec (z, prec);

      /* FIXME: x may overflow so x.y does overflow too, while Re(exp(op))
         could be represented in the precision of rop. */
      mpfr_exp (x, MPC_RE(op), GMP_RNDN);
      mpfr_sin_cos (z, y, MPC_IM(op), GMP_RNDN);
      mpfr_mul (y, y, x, GMP_RNDN);

      ok = mpfr_inf_p (y) || mpfr_zero_p (x)
        || mpfr_can_round (y, prec - 2, GMP_RNDN, MPC_RND_RE(rnd),
                           MPFR_PREC(MPC_RE(rop)));
      if (ok) /* compute imaginary part */
        {
          mpfr_mul (z, z, x, GMP_RNDN);
          ok = mpfr_inf_p (z) || mpfr_zero_p (x)
            || mpfr_can_round (z, prec - 2, GMP_RNDN, MPC_RND_IM(rnd),
                               MPFR_PREC(MPC_IM(rop)));
        }
    }
  while (ok == 0);

  mpfr_set (MPC_RE(rop), y, MPC_RND_RE(rnd));
  mpfr_set (MPC_IM(rop), z, MPC_RND_IM(rnd));

  mpfr_clear (x);
  mpfr_clear (y);
  mpfr_clear (z);
  

}
