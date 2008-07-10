/* file for generic tests.

Copyright (C) 2008 Philippe Th\'eveny.

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
#include <stdlib.h>
#include "mpfr.h"
#include "mpc.h"
#include "mpc-impl.h"

#define FUNCTION_NAME(F) NAME_STR(F)
#define NAME_STR(F) #F

/* tgeneric usage:                                                          */
/*  define TEST_FUNCTION as the function under test                         */
/*  define TWOARGS when the function under test combine two mpc_t arguments */

static void
#ifdef TWOARGS
message_failed (mpc_srcptr op, mpc_srcptr op2, mpc_srcptr rop,
		mpc_srcptr rop4, mpc_srcptr rop4rnd, mpc_rnd_t rnd)
#else
message_failed (mpc_srcptr op, mpc_srcptr rop,
		mpc_srcptr rop4, mpc_srcptr rop4rnd, mpc_rnd_t rnd)
#endif
{
  printf ("Rounding in " FUNCTION_NAME(TEST_FUNCTION) " might be "
	  "incorrect for\n");
  OUT (op);

#ifdef TWOARGS
  OUT (op2);
#endif

  printf ("with rounding mode (%s, %s)",
	  mpfr_print_rnd_mode (MPC_RND_RE (rnd)),
	  mpfr_print_rnd_mode (MPC_RND_IM (rnd)));

  printf ("\n" FUNCTION_NAME(TEST_FUNCTION) "                     gives ");
  OUT (rop);
  printf (FUNCTION_NAME(TEST_FUNCTION) " quadruple precision gives ");
  OUT (rop4);
  printf ("and is rounded to                  ");
  OUT (rop4rnd);

  exit (1);
}

static void
tgeneric(mpfr_prec_t prec_min, mpfr_prec_t prec_max, mp_exp_t exp_max)
{
  mpc_t  x, z, t, u;
  mpc_rnd_t rnd_re;
  mpc_rnd_t rnd_im;
  mpfr_prec_t prec;
  mp_exp_t exp_min;

#ifdef TWOARGS
  mpc_t y;

  mpc_init (y);
#endif

  mpc_init (x);
  mpc_init (z);
  mpc_init (t);
  mpc_init (u);

  exp_min = mpfr_get_emin ();
  if (exp_max <= 0)
    exp_max = mpfr_get_emax ();
  else if (exp_max > mpfr_get_emax ())
    exp_max = mpfr_get_emax();
  if (-exp_max > exp_min)
    exp_min = - exp_max;

  for (prec = prec_min; prec <= prec_max; prec++)
    {
      mpc_set_prec (x, prec);
      mpc_set_prec (z, prec);
      mpc_set_prec (t, prec);
      mpc_set_prec (u, 4*prec);

      test_default_random (x, exp_min, exp_max, 1);

#ifdef TWOARGS
      mpc_set_prec (y, prec);
      test_default_random (y, exp_min, exp_max, 1);
#endif

      for (rnd_re = 0; rnd_re < 4; rnd_re ++)
        for (rnd_im = 0; rnd_im < 4; rnd_im ++)
          {

            /* We compute the result with four times the precision and      */
            /* check whether the rounding is correct. Error reports in this */
            /* part of the algorithm might still be wrong, though, since    */
            /* there are two consecutive roundings (but we try to avoid     */
            /* them).                                                       */
            const mpc_rnd_t rnd = RNDC (rnd_re, rnd_im);

#ifdef TWOARGS
	    TEST_FUNCTION (u, x, y, rnd);
	    TEST_FUNCTION (z, x, y, rnd);
#else
            TEST_FUNCTION (u, x, rnd);
            TEST_FUNCTION (z, x, rnd);
#endif

            /* can't use mpfr_can_round when argument is singular */
            if ((mpfr_zero_p (MPC_RE (u)) || mpfr_inf_p (MPC_RE (u))
                 || mpfr_can_round (MPC_RE (u), 4 * prec - 1,
                                    MPC_RND_RE (rnd), MPC_RND_RE (rnd), prec))
                && (mpfr_zero_p (MPC_IM (u)) || mpfr_inf_p (MPC_IM (u))
                    || mpfr_can_round (MPC_IM (u), 4 * prec - 1,
                                       MPC_RND_IM (rnd), MPC_RND_IM (rnd),
                                       prec)))
              mpc_set (t, u, rnd);
            else
              /* avoid double rounding error */
                continue;

            if (mpc_cmp (z, t) == 0)
              continue;

#ifdef TWOARGS
            message_failed (x, y, z, u, t, rnd);
#else
            message_failed (x, z, u, t, rnd);
#endif
          }
    }


#ifdef TWOARGS
  mpc_clear (y);
#endif

  mpc_clear (x);
  mpc_clear (z);
  mpc_clear (t);
  mpc_clear (u);
}
