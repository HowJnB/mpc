/* mpc_mul_si -- Multiply a complex number by plus or minus i.

Copyright (C) 2005 Andreas Enge

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
mpc_mul_i (mpc_ptr a, mpc_srcptr b, int sign, mpc_rnd_t rnd)
/* if sign is >= 0, multiply by i, otherwise by -i */
{
  int   inex_re, inex_im;
  mpc_t res;
  mpfr_t tmp;

  /* Treat the most probable case of compatible precisions first */
  if (     MPFR_PREC (MPC_RE (b)) == MPFR_PREC (MPC_IM (a))
        && MPFR_PREC (MPC_IM (b)) == MPFR_PREC (MPC_RE (a)))
  {
     if (a == b)
     {
        tmp [0] = MPC_RE (a) [0];
        MPC_RE (a) [0] = MPC_IM (a) [0];
        MPC_IM (a) [0] = tmp [0];
     }
     else
     {
        mpfr_set (MPC_RE (a), MPC_IM (b), GMP_RNDN);
        mpfr_set (MPC_IM (a), MPC_RE (b), GMP_RNDN);
     }
     if (sign >= 0)
        MPFR_CHANGE_SIGN (MPC_RE (a));
     else
        MPFR_CHANGE_SIGN (MPC_IM (a));
     inex_re = 0;
     inex_im = 0;
  }
  else
  {
     if (a == b)
        mpc_init3 (res, MPFR_PREC (MPC_RE (a)), MPFR_PREC (MPC_IM (a)));
     else
        res [0] = a [0];
     if (sign >= 0)
     {
        inex_re = mpfr_neg (MPC_RE (res), MPC_IM (b), MPC_RND_RE (rnd));
        inex_im = mpfr_set (MPC_IM (res), MPC_RE (b), MPC_RND_IM (rnd));
     }
     else
     {
        inex_re = mpfr_set (MPC_RE (res), MPC_IM (b), MPC_RND_RE (rnd));
        inex_im = mpfr_neg (MPC_IM (res), MPC_RE (b), MPC_RND_IM (rnd));
     }
     mpc_set (a, res, GMP_RNDN);
     if (a == b)
        mpc_clear (res);
  }
  
  return MPC_INEX(inex_re, inex_im);  
}
