/* test file for mpc_sin.

Copyright (C) 2007 Andreas Enge, Paul Zimmermann

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

#define TEST_FUNCTION mpc_sin
#include "tgeneric.c"

static void
test_failed (mpc_t op, mpc_t get, mpc_t expected)
{
  printf ("mpc_sin(op) failed with ");
  OUT (op);
  printf ("     ");
  OUT (get);
  OUT (expected);
  exit (1);
}

/* check special values as defined in C99 standard */
static void
special ()
{
  mpc_t z;
  mpc_t s;
  mpc_t c99;

  mpc_init (z);
  mpc_init (s);
  mpc_init (c99);

  mpfr_set_nan (MPC_RE (z));
  /* sin(NaN +i*NaN) = NaN +i*NaN */
  mpfr_set_nan (MPC_IM (z));
  mpc_sin (s, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (s)) && !mpfr_nan_p (MPC_IM (s)))
    test_failed (z, s, z);

  /* sin(NaN +i*infinity) = NaN +/-i*infinity */
  mpfr_set_inf (MPC_IM (z), +1);
  mpc_sin (s, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (s)) && !mpfr_inf_p (MPC_IM (s)))
    test_failed (z, s, z);

  /* sin(NaN -i*infinity) = NaN -/+i*infinity */
  mpc_conj (z, z, MPC_RNDNN);
  mpc_sin (s, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (s)) && !mpfr_inf_p (MPC_IM (s)))
    test_failed (z, s, z);

  /* sin(NaN +i*y) = NaN +i*NaN where 0<|y|<infinity */
  mpfr_set_ui (MPC_IM (z), 1, GMP_RNDN);
  mpfr_set_nan (MPC_RE (c99));
  mpfr_set_nan (MPC_IM (c99));
  mpc_sin (s, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (s)) && !mpfr_nan_p (MPC_IM (s)))
    test_failed (z, s, c99);
  mpc_conj (z, z, MPC_RNDNN);
  mpc_sin (s, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (s)) && !mpfr_nan_p (MPC_IM (s)))
    test_failed (z, s, c99);

  /* sin(NaN +i*0) = NaN +/-i*0 */
  mpfr_set_ui (MPC_IM (z), 0, GMP_RNDN);
  mpc_sin (s, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (s)) && !mpfr_zero_p (MPC_IM (s)))
    test_failed (z, s, z);
  mpc_conj (z, z, MPC_RNDNN);
  mpc_sin (s, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (s)) && !mpfr_zero_p (MPC_IM (s)))
    test_failed (z, s, z);

  /* sin(+0 +i*NaN) = +0 +i*NaN */
  mpc_mul_i (z, z, +1, MPC_RNDNN);
  mpc_sin (s, z, MPC_RNDNN);
  if (!mpfr_zero_p (MPC_RE (s)) || mpfr_signbit (MPC_RE (s)) != 0
      || !mpfr_nan_p (MPC_IM (s)))
    test_failed (z, s, z);

  /* sin(-0 +i*NaN) = -0 +i*NaN */
  mpc_neg (z, z, MPC_RNDNN);
  mpc_sin (s, z, MPC_RNDNN);
  if (!mpfr_zero_p (MPC_RE (s)) || mpfr_signbit (MPC_RE (s)) == 0
      || !mpfr_nan_p (MPC_IM (s)))
    test_failed (z, s, z);

  /* sin(x +i*NaN) = NaN +i*NaN where x!=0 */
  mpfr_set_inf (MPC_RE (z), +1);
  mpc_sin (s, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (s)) && !mpfr_nan_p (MPC_IM (s)))
    test_failed (z, s, c99);
  mpc_neg (z, z, MPC_RNDNN);
  mpc_sin (s, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (s)) && !mpfr_nan_p (MPC_IM (s)))
    test_failed (z, s, c99);
  mpfr_set_ui (MPC_RE (z), +1, MPC_RNDNN);
  mpc_sin (s, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (s)) && !mpfr_nan_p (MPC_IM (s)))
    test_failed (z, s, c99);
  mpc_neg (z, z, MPC_RNDNN);
  mpc_sin (s, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (s)) && !mpfr_nan_p (MPC_IM (s)))
    test_failed (z, s, c99);

  mpfr_set_inf (MPC_RE(z), -1);
  /* sin(-infinity -i*infinity) = NaN -/+i*infinity */
  mpfr_set_inf (MPC_IM(z), -1);
  mpfr_set_nan (MPC_RE(c99));
  mpfr_set_inf (MPC_IM(c99), -1);
  mpc_sin (s, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (s)) || !mpfr_inf_p (MPC_IM (s)))
    test_failed (z, s, c99);

  /* sin(-infinity +i*infinity) = NaN +/-i*infinity */
  mpc_conj (z, z, MPC_RNDNN);
  mpc_conj (c99, c99, MPC_RNDNN);
  mpc_sin (s, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (s)) || !mpfr_inf_p (MPC_IM (s)))
    test_failed (z, s, c99);

  /* sin(+infinity -i*infinity) = NaN -/+i*infinity */
  mpc_neg (z, z, MPC_RNDNN);
  mpc_neg (c99, c99, MPC_RNDNN);
  mpc_sin (s, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (s)) || !mpfr_inf_p (MPC_IM (s)))
    test_failed (z, s, c99);

  /* sin(+infinity +i*infinity) = NaN +/-i*infinity */
  mpc_conj (z, z, MPC_RNDNN);
  mpc_conj (c99, c99, MPC_RNDNN);
  mpc_sin (s, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (s)) || !mpfr_inf_p (MPC_IM (s)))
    test_failed (z, s, c99);

  /* sin(x -i*infinity) = infinity*(sin x -i*cos x) where 0<|x|<infinity */
  /* sin(x +i*infinity) = infinity*(sin x +i*cos x) where 0<|x|<infinity */
  mpfr_set_ui (MPC_RE (z), 1, GMP_RNDN);
  mpfr_set_inf (MPC_RE (c99), +1);
  mpfr_set_inf (MPC_IM (c99), +1);
  mpc_sin (s, z, MPC_RNDUU);
  if (mpc_cmp (s, c99) != 0)
    test_failed (z, s, c99);
  mpc_conj (z, z, MPC_RNDNN);
  mpc_conj (c99, c99, MPC_RNDNN);
  mpc_sin (s, z, MPC_RNDUU);
  if (mpc_cmp (s, c99) != 0)
    test_failed (z, s, c99);
  mpfr_neg (MPC_RE (z), MPC_RE (z), GMP_RNDN);
  mpfr_neg (MPC_RE (c99), MPC_RE (c99), GMP_RNDN);
  mpc_sin (s, z, MPC_RNDUU);
  if (mpc_cmp (s, c99) != 0)
    test_failed (z, s, c99);
  mpc_conj (z, z, MPC_RNDNN);
  mpc_conj (c99, c99, MPC_RNDNN);
  mpc_sin (s, z, MPC_RNDUU);
  if (mpc_cmp (s, c99) != 0)
    test_failed (z, s, c99);

  /* sin(+0 -i*infinity) = +0 -i*infinity */
  mpfr_set_ui (MPC_RE (z), 0, GMP_RNDN);
  mpc_sin (s, z, MPC_RNDNN);
  if (mpc_cmp (s, z) != 0)
    test_failed (z, s, z);

  /* sin(+0 +i*infinity) = +0 +i*infinity */
  mpc_conj (z, z, MPC_RNDNN);
  mpc_sin (s, z, MPC_RNDNN);
  if (mpc_cmp (s, z) != 0)
    test_failed (z, s, z);

  /* sin(-0 -i*infinity) = -0 -i*infinity */
  mpc_neg (z, z, MPC_RNDNN);
  mpc_sin (s, z, MPC_RNDNN);
  if (mpc_cmp (s, z) != 0)
    test_failed (z, s, z);

  /* sin(-0 +i*infinity) = -0 +i*infinity */
  mpc_conj (z, z, MPC_RNDNN);
  mpc_sin (s, z, MPC_RNDNN);
  if (mpc_cmp (s, z) != 0)
    test_failed (z, s, z);

  /* sin(-infinity +i*0) = NaN +/-i*0 */
  mpfr_set_inf (MPC_RE (z), -1);
  mpfr_set_ui (MPC_IM (z), 0, GMP_RNDN);
  mpfr_set_nan (MPC_RE (c99));
  mpfr_set_ui (MPC_IM (c99), 0, GMP_RNDN);
  mpc_sin (s, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (s)) || !mpfr_zero_p (MPC_IM (s)))
    test_failed (z, s, c99);

  /* sin(-infinity -i*0) = NaN -/+i*0 */
  mpc_conj (z, z, MPC_RNDNN);
  mpc_conj (c99, c99, MPC_RNDNN);
  mpc_sin (s, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (s)) || !mpfr_zero_p (MPC_IM (s)))
    test_failed (z, s, c99);

  /* sin(+infinity +i*0) = NaN +/-i*0 */
  mpc_neg (z, z, MPC_RNDNN);
  mpc_neg (c99, c99, MPC_RNDNN);
  mpc_sin (s, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (s)) || !mpfr_zero_p (MPC_IM (s)))
    test_failed (z, s, c99);

  /* sin(+infinity -i*0) = NaN -/+i*0 */
  mpc_conj (z, z, MPC_RNDNN);
  mpc_conj (c99, c99, MPC_RNDNN);
  mpc_sin (s, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (s)) || !mpfr_zero_p (MPC_IM (s)))
    test_failed (z, s, c99);

  /* sin(+/-infinity +i*y) = NaN +i*NaN where 0<|y|<infinity*/
  mpfr_set_ui (MPC_IM (z), 1, GMP_RNDN);
  mpfr_set_nan (MPC_IM (c99));
  mpc_sin (s, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (s)) || !mpfr_nan_p (MPC_IM (s)))
    test_failed (z, s, c99);
  mpc_conj (z, z, MPC_RNDNN);
  mpc_sin (s, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (s)) || !mpfr_nan_p (MPC_IM (s)))
    test_failed (z, s, c99);
  mpc_neg (z, z, MPC_RNDNN);
  mpc_sin (s, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (s)) || !mpfr_nan_p (MPC_IM (s)))
    test_failed (z, s, c99);
  mpc_conj (z, z, MPC_RNDNN);
  mpc_sin (s, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (s)) || !mpfr_nan_p (MPC_IM (s)))
    test_failed (z, s, c99);

  mpc_clear (c99);
  mpc_clear (s);
  mpc_clear (z);
}

int
main()
{
  mpc_t  x, z;
  mpfr_t g;
  mp_prec_t prec;

  mpc_init (x);
  mpc_init (z);
  mpfr_init (g);

  for (prec = 2; prec <= 1000; prec+=2)
    {
      mpc_set_prec (x, prec);
      mpc_set_prec (z, prec);
      mpfr_set_prec (g, prec);

      /* check that sin(I*b) = I*sinh(b) */
      mpfr_set_ui (MPC_RE (x), 0, GMP_RNDN);
      mpfr_random (MPC_IM (x));

      mpc_sin (z, x, MPC_RNDNN);
      mpfr_sinh (g, MPC_IM(x), GMP_RNDN);
      if (mpfr_cmp_ui (MPC_RE(z), 0) || mpfr_cmp (g, MPC_IM(z)))
	{
	  fprintf (stderr, "Error in mpc_sin: sin(I*x) <> I*sinh(x)\n"
		   "got      ");
	  mpc_out_str (stderr, 10, 0, z, MPC_RNDNN);
	  fprintf (stderr, "\nexpected ");
	  mpfr_set_ui (MPC_RE(z), 0, GMP_RNDN);
	  mpfr_set (MPC_IM(z), g, GMP_RNDN);
	  mpc_out_str (stderr, 10, 0, z, MPC_RNDNN);
	  fprintf (stderr, "\n");
	  exit (1);
	}
    }

  special ();
  tgeneric ();

  mpc_clear (x);
  mpc_clear (z);
  mpfr_clear (g);

  return 0;
}
