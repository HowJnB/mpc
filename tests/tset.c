/* tset.c -- Test file for mpc_set_x functions.

Copyright (C) 2009 Philippe Th\'eveny

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
#include "mpc.h"
#include "mpc-impl.h"

int
main (void)
{
  unsigned long int n;
  mpfr_t f;
  mpc_t x, z;
  mp_prec_t prec;

  mpc_init2 (x, 1000);
  mpc_init3 (z, 2, 2);
  mpfr_init2 (f, 1000);

  for (prec = 2; prec <= 1000; prec++)
    {
      mpc_set_prec (z, prec);
      mpfr_set_prec (f, prec);
      mpfr_set_d (f, 1.23456789, GMP_RNDN);

      mpc_set_d (z, 1.23456789, MPC_RNDNN);
      mpfr_clear_flags (); /* mpc_cmp set erange flag when an operand is a
                              NaN */
      if (mpfr_cmp (MPC_RE(z), f) != 0
          || mpfr_cmp_si (MPC_IM(z), 0) != 0
          || mpfr_erangeflag_p())
        {
          printf ("Error in mpc_set_d for prec = %lu\n",
                  (unsigned long int) prec);
          OUT(z);
          exit (1);
        }

      mpc_set_d_d (z, 1.23456789, 1.23456789, MPC_RNDNN);
      mpfr_clear_flags ();
      if (mpfr_cmp (MPC_RE(z), f) != 0
          || mpfr_cmp (MPC_IM(z), f) != 0
          || mpfr_erangeflag_p())
        {
          printf ("Error in mpc_set_d_d for prec = %lu\n",
                  (unsigned long int) prec);
          OUT(z);
          exit (1);
        }

      n = (1<<prec)-1;
      mpc_set_ui (z, n, MPC_RNDNN);
      mpfr_clear_flags ();
      if (mpfr_cmp_ui (MPC_RE(z), n) != 0
          || mpfr_cmp_ui (MPC_IM(z), 0) != 0
          || mpfr_erangeflag_p())
        {
          printf ("Error in mpc_set_ui for prec = %lu\n",
                  (unsigned long int) prec);
          OUT(z);
          exit (1);
        }

      mpc_set_prec (x, prec);
      mpfr_set_ui(f, 1, GMP_RNDN);
      mpfr_div_ui(f, f, 3, GMP_RNDN);
      mpfr_set(MPC_RE(x), f, GMP_RNDN);
      mpfr_set(MPC_IM(x), f, GMP_RNDN);

      mpc_set (z, x, MPC_RNDNN);
      mpfr_clear_flags ();
      if (mpc_cmp (z, x) != 0 || mpfr_erangeflag_p())
        {
          printf ("Error in mpc_set for prec = %lu\n",
                  (unsigned long int) prec);
          OUT(z);
          OUT(x);
          exit (1);
        }

      mpc_set_fr_fr (z, f, f, MPC_RNDNN);
      mpfr_clear_flags ();
      if (mpfr_cmp (MPC_RE(z), f) != 0
          || mpfr_cmp (MPC_IM(z), f) != 0
          || mpfr_erangeflag_p())
        {
          printf ("Error in mpc_set_fr_fr for prec = %lu\n",
                  (unsigned long int) prec);
          OUT(z);
          exit (1);
        }

      mpc_set_ui_fr (z, n, f, MPC_RNDNN);
      mpfr_clear_flags ();
      if (mpfr_cmp_ui (MPC_RE(z), n) != 0
          || mpfr_cmp (MPC_IM(z), f) != 0
          || mpfr_erangeflag_p())
        {
          printf ("Error in mpc_set_ui_fr for prec = %lu\n",
                  (unsigned long int) prec);
          OUT(z);
          exit (1);
        }

      mpc_set_ui_ui (z, n, n, MPC_RNDNN);
      mpfr_clear_flags ();
      if (mpfr_cmp_ui (MPC_RE(z), n) != 0
          || mpfr_cmp_ui (MPC_IM(z), n) != 0
          || mpfr_erangeflag_p())
        {
          printf ("Error in mpc_set_ui_ui for prec = %lu\n",
                  (unsigned long int) prec);
          OUT(z);
          exit (1);
        }
    }

  mpc_clear (x);
  mpc_clear (z);
  mpfr_clear (f);

  return 0;
}
