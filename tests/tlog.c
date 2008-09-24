/* test file for mpc_log.

Copyright (C) 2008 Andreas Enge, Philippe Th\'eveny

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
#include "tgeneric_cc.c"

static void
check_exp_log (void)
{
   mpc_t z, z2, tmp;
   mpfr_t twopi;
   mp_prec_t prec;

   mpc_init2 (z, 1000);
   mpc_init2 (z2, 1000);
   mpc_init2 (tmp, 4000);
   mpfr_init2 (twopi, 1000);
   mpfr_const_pi (twopi, GMP_RNDD);
   mpfr_div_2ui (twopi, twopi, 1, GMP_RNDN);

   for (prec = 2; prec <= 1000; prec = prec*1.1 + 1)
   {
      mpc_set_prec (z, prec);
      mpc_set_prec (z2, prec);
      mpc_set_prec (tmp, 4*prec);

      /* Test whether exp (log (z)) = z, where z is no pure real
         and no pure imaginary. */
      test_default_random (z, 0, 10, 1);
      if (mpfr_zero_p (MPC_RE (z)))
        mpfr_set (MPC_RE (z), MPC_IM (z), GMP_RNDN);
      else if  (mpfr_zero_p (MPC_IM (z)))
        mpfr_set (MPC_IM (z), MPC_RE (z), GMP_RNDN);

      mpc_log (tmp, z, MPC_RNDNN);
      mpc_exp (z2, tmp, MPC_RNDNN);

      if (mpc_cmp (z, z2) != 0)
      {
         printf ("Possible error in log; difference between z and "\
                 "z2=exp(log(z)):\n");
         OUT (z);
         OUT (tmp);
         OUT (z2);
         exit (1);
      }


      /* Test whether log (exp (z)) = z for purely imaginary z; then exp (x) */
      /* lies on the unit cercle, a critical case for the logarithm.         */
      mpfr_set_ui (MPC_RE (z), 0, GMP_RNDN);

      mpfr_remainder (MPC_IM (z), MPC_IM (z), twopi, GMP_RNDZ);
      mpc_exp (tmp, z, MPC_RNDNN);
      mpc_log (z2, tmp, MPC_RNDNN);

      /* There is a tiny real part, do not care if it si sufficiently small. */
      if (mpfr_cmp (MPC_IM (z), MPC_IM (z2)) != 0
          || MPFR_EXP (MPC_RE (z)) > -4 * (mp_exp_t) prec)
      {
         printf ("Possible error in purely imaginary log; difference "\
                 "between z and z2=log(exp(z)):\n");
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
}

int
main (void)
{
   test_start ();

   check_exp_log ();
   tgeneric (2, 512, 7, 128);

   test_end ();

   return 0;
}
