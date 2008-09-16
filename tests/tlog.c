/* test file for mpc_log.

Copyright (C) 2008 Andreas Enge

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
#include "gmp.h"
#include "mpfr.h"
#include "mpc.h"

#include "random.c"

#define TEST_FUNCTION mpc_log
#include "tgeneric.c"

int
main (void)
{
   mpc_t z, z2, tmp;
   mpfr_t twopi;
   mp_prec_t prec;

   test_start ();

   tgeneric (2, 512, 7, 128);

   mpc_init (z);
   mpc_init (z2);
   mpc_init (tmp);
   mpfr_init2 (twopi, 1000);
   mpfr_const_pi (twopi, GMP_RNDD);
   mpfr_div_2ui (twopi, twopi, 1, GMP_RNDN);

   /* Test whether exp (log (z)) = z */
   for (prec = 2; prec <= 1000; prec+=7)
   {
      mpc_set_prec (z, prec);
      mpc_set_prec (z2, prec);
      mpc_set_prec (tmp, 4*prec);

      /* The case of a purely imaginary number is treated below. */
      do {
         mpc_random (z);
      } while (mpfr_zero_p (MPC_RE (z)));
      mpc_log (tmp, z, MPC_RNDNN);
      mpc_exp (z2, tmp, MPC_RNDNN);

      if (mpc_cmp (z, z2) != 0)
      {
         printf ("Possible error in log; difference between z and z2=exp(log(z)):\n");
         OUT (z);
         OUT (tmp);
         OUT (z2);
         exit (1);
      }
   }


   /* Test whether log (exp (z)) = z for purely imaginary z; then exp (x) */
   /* lies on the unit cercle, a critical case for the logarithm.         */
   for (prec = 2; prec <= 1000; prec+=7)
   {
      mpc_set_prec (z, prec);
      mpc_set_prec (z2, prec);
      mpc_set_prec (tmp, 4*prec);

      mpfr_set_ui (MPC_RE (z), 0, GMP_RNDN);
      mpfr_random (MPC_IM (z));
      mpfr_remainder (MPC_IM (z), MPC_IM (z), twopi, GMP_RNDZ);
      mpc_exp (tmp, z, MPC_RNDNN);
      mpc_log (z2, tmp, MPC_RNDNN);

      /* There is a tiny real part, do not care if it si sufficiently small. */
      if (   mpfr_cmp (MPC_IM (z), MPC_IM (z2)) != 0
          || MPFR_EXP (MPC_RE (z)) > -4 * (mp_exp_t) prec)
      {
         printf ("Possible error in purely imaginary log; difference between z and z2=log(exp(z)):\n");
         OUT (z);
         OUT (tmp);
         OUT (z2);
         exit (1);
      }
   }

   mpc_clear (z);
   mpc_clear (z2);
   mpc_clear (tmp);
   mpfr_clear (twopi);

   test_end ();

   return 0;
}
