/* mpc_sqrt -- Take the square root of a complex number.

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

int
mpc_sqrt (mpc_ptr a, mpc_srcptr b, mpc_rnd_t rnd)
{
  int ok=0;
  mpfr_t    w, t;
  mp_rnd_t  rnd_w, rnd_t;
  mp_prec_t prec_w, prec_t;
    /* the rounding mode and the precision required for w and t, which can */
    /* be either the real or the imaginary part of a */
  int re_cmp, im_cmp;
    /* comparison of the real/imaginary part of b with 0 */
  mp_prec_t prec;
  int inexact;

  im_cmp = mpfr_cmp_ui (MPC_IM (b), 0);  
  re_cmp = mpfr_cmp_ui (MPC_RE (b), 0);  
  if (im_cmp == 0)
  {
    if (re_cmp == 0)
    {
      mpc_set_ui_ui (a, 0, 0, MPC_RNDNN);
      return 0;
    }
    else if (re_cmp > 0)
    {
      inexact = mpfr_sqrt (MPC_RE (a), MPC_RE (b), MPC_RND_RE (rnd));
      mpfr_set_ui (MPC_IM (a), 0, GMP_RNDN);
      return inexact;
    }
    else
    {
      mpfr_init2 (w, MPFR_PREC (MPC_RE (b)));
      inexact = mpfr_neg (w, MPC_RE (b), GMP_RNDN);
      inexact = mpfr_sqrt (MPC_IM (a), w, MPC_RND_IM (rnd));
      mpfr_set_ui (MPC_RE (a), 0, GMP_RNDN);
      mpfr_clear (w);
      return inexact;
    }
  }

  prec = MPC_MAX_PREC(a);

  mpfr_init (w);
  mpfr_init (t);
  
  if (re_cmp >= 0)
  {
    rnd_w = MPC_RND_RE (rnd);
    prec_w = MPFR_PREC (MPC_RE (a));
    rnd_t = MPC_RND_IM(rnd);
    prec_t = MPFR_PREC (MPC_IM (a));
  }
  else
  {
    rnd_w = MPC_RND_IM(rnd);
    prec_w = MPFR_PREC (MPC_IM (a));
    rnd_t = MPC_RND_RE(rnd);
    prec_t = MPFR_PREC (MPC_RE (a));
    if (im_cmp < 0)
    {
      if (rnd_w == GMP_RNDD)
        rnd_w = GMP_RNDU;
      else if (rnd_w == GMP_RNDU)
        rnd_w = GMP_RNDD;
      if (rnd_t == GMP_RNDD)
        rnd_t = GMP_RNDU;
      if (rnd_t == GMP_RNDU)
        rnd_t = GMP_RNDD;
    }
  }

  do
  {
      prec += mpc_ceil_log2 (prec) + 4;
      mpfr_set_prec (w, prec);
      mpfr_set_prec (t, prec);
      /* let b = x + iy */
      /* w = sqrt ((|x| + sqrt (x^2 + y^2)) / 2), rounded down */
      /* total error bounded by 3 ulps */
      inexact = mpc_abs (w, b, GMP_RNDD);
      if (re_cmp < 0)
        inexact |= mpfr_sub (w, w, MPC_RE (b), GMP_RNDD);
      else
        inexact |= mpfr_add (w, w, MPC_RE (b), GMP_RNDD);
      inexact |= mpfr_div_2ui (w, w, 1, GMP_RNDD);
      inexact |= mpfr_sqrt (w, w, GMP_RNDD);

      ok = mpfr_can_round (w, prec - 2, GMP_RNDD, rnd_w, prec_w);
      if (ok)
      {
        /* t = y / 2w, rounded up */
        /* total error bounded by 7 ulps */
        inexact |= mpfr_div (t, MPC_IM (b), w, GMP_RNDU);
        inexact |= mpfr_div_2ui (t, t, 1, GMP_RNDU);
        ok = mpfr_can_round (t, prec - 3, GMP_RNDU, rnd_t, prec_t);
      }
  }
  while (inexact != 0 && ok == 0);

  if (re_cmp > 0)
  {
    inexact |= mpfr_set (MPC_RE (a), w, rnd_w);
    inexact |= mpfr_set (MPC_IM (a), t, rnd_t);
  }
  else
  {
    inexact |= mpfr_set (MPC_RE(a), t, MPC_RND_RE(rnd));
    inexact |= mpfr_set (MPC_IM(a), w, MPC_RND_IM(rnd));
    if (im_cmp < 0)
    {
      inexact |= mpfr_neg (MPC_RE (a), MPC_RE (a), MPC_RND_RE (rnd));
      inexact |= mpfr_neg (MPC_IM (a), MPC_IM (a), MPC_RND_IM (rnd));
    }
  }

  mpfr_clear (w);
  mpfr_clear (t);

  return inexact;
}
