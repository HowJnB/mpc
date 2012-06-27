/* ttan -- test file for mpc_rootofunity.

Copyright (C) 2012 INRIA

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

#include "mpc-tests.h"

static void
check (unsigned long int n)
   /* checks whether zeta_n^n = 1, which is somewhat dangerous in floating
      point                                                                */
{
   mpc_t z, zero;
   mpfr_prec_t prec;

   mpc_init2 (z, 2);
   mpc_init2 (zero, 2);

   for (prec = 2*n; prec < 1000; prec = (mpfr_prec_t) (prec * 1.1 + 1)) {
      mpc_set_prec (z, prec);
      mpc_set_prec (zero, prec);

      mpc_rootofunity (z, n, MPC_RNDNN);
      mpc_pow_ui (zero, z, n, MPC_RNDNN);
      mpc_sub_ui (zero, zero, 1u, MPC_RNDNN);
      if (MPC_MAX (mpfr_get_exp (mpc_realref (zero)), mpfr_get_exp (mpc_imagref (zero)))
          > - (prec - n)) {
         fprintf (stderr, "rootofunity too imprecise for n=%lu\n", n);
         MPC_OUT (z);
         MPC_OUT (zero);
         exit (1);
      }
   }

   mpc_clear (z);
   mpc_clear (zero);
}


int
main (void)
{
   int n;

   for (n = 1; n < 10000; n += 10)
      check (n);
   
   return 0;
}
