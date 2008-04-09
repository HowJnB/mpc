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

#define FUNCTION_NAME(F) #F
 
static void
tgeneric()
{
  mpc_t  x, z, t, u;
  mp_prec_t prec;

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

      /* We compute the result with four times the precision and check        */
      /* whether the rounding is correct. Error reports in this part of the   */
      /* algorithm might still be wrong, though, since there are two          */
      /* consecutive roundings.                                               */
      mpc_random (x);
      TEST_FUNCTION (z, x, MPC_RNDNN);
      TEST_FUNCTION (u, x, MPC_RNDNN);
      mpc_set (t, u, MPC_RNDNN);

      if (mpc_cmp (z, t))
	{
	  fprintf (stderr, "rounding in " FUNCTION_NAME(TEST_FUNCTION) \
		   " might be incorrect for\nx=");
	  mpc_out_str (stderr, 2, 0, x, MPC_RNDNN);
	  fprintf (stderr, "\n" FUNCTION_NAME(TEST_FUNCTION) "          "\
		   "           gives ");
	  mpc_out_str (stderr, 2, 0, z, MPC_RNDNN);
	  fprintf (stderr, "\n" FUNCTION_NAME(TEST_FUNCTION) " quadruple "\
		   "precision gives ");
	  mpc_out_str (stderr, 2, 0, u, MPC_RNDNN);
	  fprintf (stderr, "\nand is rounded to                 ");
	  mpc_out_str (stderr, 2, 0, t, MPC_RNDNN);
	  fprintf (stderr, "\n");
	  exit (1);
	}
    }
  mpc_clear (x);
  mpc_clear (z);
  mpc_clear (t);
  mpc_clear (u);
}
