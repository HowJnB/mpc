/* test file for mpc_log.

Copyright (C) 2008, 2009 Andreas Enge, Philippe Th\'eveny, Paul Zimmermann

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

#include <stdlib.h>
#include "mpc-tests.h"

/* PZ: disabled check_exp_log since it gives false positives, for example
   with GMP_CHECK_RANDOMIZE=1254693391 with GMP 4.3.0 on 64-bit computers */
#if 0
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

   for (prec = 4; prec <= 1000; prec = prec*1.1 + 1)
   {
      mpc_set_prec (z, prec);
      mpc_set_prec (z2, prec);
      mpc_set_prec (tmp, 4*prec);

      /* Test whether exp (log (z)) = z, where z is no pure real
         and no pure imaginary. */
      test_default_random (z, 0, 10, 128, 0);

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
#endif

int
main (void)
{
  DECL_FUNC (CC, f, mpc_log);

  test_start ();

  data_check (f, "log.dat");
  tgeneric (f, 2, 512, 7, 128);

  test_end ();

  return 0;
}
