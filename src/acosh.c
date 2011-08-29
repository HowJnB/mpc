/* mpc_acosh -- inverse hyperbolic cosine of a complex number.

Copyright (C) 2009 INRIA

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

int
mpc_acosh (mpc_ptr rop, mpc_srcptr op, mpc_rnd_t rnd)
{
  /* acosh(z) =
      NaN + i*NaN, if z=0+i*NaN
     -i*acos(z), if sign(Im(z)) = -
      i*acos(z), if sign(Im(z)) = +
      http://functions.wolfram.com/ElementaryFunctions/ArcCosh/27/02/03/01/01/
  */
  mpc_t a;
  mpfr_t tmp;
  int inex;

  if (mpfr_zero_p (MPC_RE (op)) && mpfr_nan_p (MPC_IM (op)))
    {
      mpfr_set_nan (MPC_RE (rop));
      mpfr_set_nan (MPC_IM (rop));
      return 0;
    }

  /* Note reversal of precisions due to later multiplication by i or -i */
  mpc_init3 (a, MPC_PREC_IM(rop), MPC_PREC_RE(rop));

  if (mpfr_signbit (MPC_IM (op)))
    {
      inex = mpc_acos (a, op,
                       RNDC (INV_RND (MPC_RND_IM (rnd)), MPC_RND_RE (rnd)));

      /* change a to -i*a, i.e., -y+i*x to x+i*y */
      tmp[0] = MPC_RE (a)[0];
      MPC_RE (a)[0] = MPC_IM (a)[0];
      MPC_IM (a)[0] = tmp[0];
      MPFR_CHANGE_SIGN (MPC_IM (a));
      inex = MPC_INEX (MPC_INEX_IM (inex), -MPC_INEX_RE (inex));
    }
  else
    {
      inex = mpc_acos (a, op,
                       RNDC (MPC_RND_IM (rnd), INV_RND(MPC_RND_RE (rnd))));

      /* change a to i*a, i.e., y-i*x to x+i*y */
      tmp[0] = MPC_RE (a)[0];
      MPC_RE (a)[0] = MPC_IM (a)[0];
      MPC_IM (a)[0] = tmp[0];
      MPFR_CHANGE_SIGN (MPC_RE (a));
      inex = MPC_INEX (-MPC_INEX_IM (inex), MPC_INEX_RE (inex));
    }

  mpc_set (rop, a, rnd);

  mpc_clear (a);

  return inex;
}
