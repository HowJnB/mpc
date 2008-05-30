/* test file for mpc_exp.

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
#include <stdlib.h>
#include "gmp.h"
#include "mpfr.h"
#include "mpc.h"

#include "random.c"

#define TEST_FUNCTION mpc_exp
#include "tgeneric.c"

int
main()
{
  mpc_t  x, z;
  mpfr_t f, g;
  mp_prec_t prec;

  test_start ();

  mpc_init (x);
  mpc_init (z);
  mpfr_init (f);
  mpfr_init (g);

  for (prec = 2; prec <= 1000; prec++)
    {
      mpc_set_prec (x, prec);
      mpc_set_prec (z, prec);
      mpfr_set_prec (f, prec);
      mpfr_set_prec (g, prec);

      /* check that exp(I*b) = cos(b) + I*sin(b) */
      mpfr_set_ui (MPC_RE (x), 0, GMP_RNDN);
      mpfr_random (MPC_IM (x));

      mpc_exp (z, x, MPC_RNDNN);
      mpfr_sin_cos (f, g, MPC_IM(x), GMP_RNDN);
      if (mpfr_cmp (g, MPC_RE(z)) || mpfr_cmp (f, MPC_IM(z)))
        {
          fprintf (stderr, "Error in mpc_exp: exp(I*x) <> cos(x)+I*sin(x)\n"
                   "got      ");
          mpc_out_str (stderr, 10, 0, z, MPC_RNDNN);
          fprintf (stderr, "\nexpected ");
          mpfr_set (MPC_RE(z), g, GMP_RNDN);
          mpfr_set (MPC_IM(z), f, GMP_RNDN);
          mpc_out_str (stderr, 10, 0, z, MPC_RNDNN);
          fprintf (stderr, "\n");
          exit (1);
        }
    }

  tgeneric ();

  mpc_clear (x);
  mpc_clear (z);
  mpfr_clear (f);
  mpfr_clear (g);

  test_end ();

  return 0;
}
