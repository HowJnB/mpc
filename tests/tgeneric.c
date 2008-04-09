/* file for generic tests.

Copyright (C) 2008 INRIA.

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

#define FUNCTION_NAME(F) NAME_STR(F)
#define NAME_STR(F) #F

static void
message_failed (mpc_srcptr op, mpc_srcptr rop, mpc_srcptr rop4,
		mpc_srcptr rop4rnd, mpc_rnd_t rnd)
{
  fprintf (stderr, "rounding in " FUNCTION_NAME(TEST_FUNCTION) " might be "
	   "incorrect for\nx=");
  mpc_out_str (stderr, 2, 0, op, rnd);

  fprintf (stderr, "\nwith rounding mode (%s, %s)",
	   mpfr_print_rnd_mode (MPC_RND_RE (rnd)),
	   mpfr_print_rnd_mode (MPC_RND_IM (rnd)));

  fprintf (stderr, "\n" FUNCTION_NAME(TEST_FUNCTION) "                     "
	   "gives ");
  mpc_out_str (stderr, 2, 0, rop, rnd);
  fprintf (stderr, "\n" FUNCTION_NAME(TEST_FUNCTION) " quadruple precision "
	   "gives ");
  mpc_out_str (stderr, 2, 0, rop4, rnd);
  fprintf (stderr, "\nand is rounded to                  ");
  mpc_out_str (stderr, 2, 0, rop4rnd, rnd);
  fprintf (stderr, "\n");

  exit (1);
}

static void
tgeneric()
{
  mpc_t  x, z, t, u;
  mp_prec_t prec;
  mpc_rnd_t rnd_re;
  mpc_rnd_t rnd_im;

  mpc_init (x);
  mpc_init (z);
  mpc_init (t);
  mpc_init (u);

  for (prec = 2; prec <= 1000; prec++)
    {
      mpc_set_prec (x, prec);
      mpc_set_prec (z, prec);
      mpc_set_prec (t, prec);
      mpc_set_prec (u, 4*prec);

      mpc_random (x);

      for (rnd_re = 0; rnd_re < 4; rnd_re ++)
         for (rnd_im = 0; rnd_im < 4; rnd_im ++)
       {
        /* We compute the result with four times the precision and check        */
        /* whether the rounding is correct. Error reports in this part of the   */
        /* algorithm might still be wrong, though, since there are two          */
        /* consecutive roundings.                                               */
        const mpc_rnd_t rnd = RNDC (rnd_re, rnd_im);
        TEST_FUNCTION (u, x, rnd);
	    TEST_FUNCTION (z, x, rnd);
	    mpc_set (t, u, rnd);

	    if (mpc_cmp (z, t))
	      message_failed (x, z, u, t, rnd);
	  }
    }

  mpc_clear (x);
  mpc_clear (z);
  mpc_clear (t);
  mpc_clear (u);
}
