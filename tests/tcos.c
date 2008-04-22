/* test file for mpc_cos.

Copyright (C) 2007 INRIA.

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

#define TEST_FUNCTION mpc_cos
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
  mpc_t c;
  mpc_t c99;

  mpc_init (z);
  mpc_init (c);
  mpc_init (c99);

  mpfr_set_nan (MPC_RE (z));
  /* cos(NaN +i*NaN) = NaN +i*NaN */
  mpfr_set_nan (MPC_IM (z));
  mpc_cos (c, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (c)) && !mpfr_nan_p (MPC_IM (c)))
    test_failed (z, c, z);

  /* cos(NaN +i*Inf) = -Inf +i*NaN */
  mpfr_set_inf (MPC_IM (z), +1);
  mpfr_set_inf (MPC_RE (c99), -1);
  mpfr_set_nan (MPC_IM (c99));
  mpc_cos (c, z, MPC_RNDNN);
  if (!mpfr_inf_p (MPC_RE (c)) || mpfr_sgn (MPC_RE (c)) >=0
      || !mpfr_nan_p (MPC_IM (c)))
    test_failed (z, c, c99);

  /* cos(NaN -i*Inf) = +Inf +i*NaN */
  mpc_conj (z, z, MPC_RNDNN);
  mpc_neg (c99, c99, MPC_RNDNN);
  mpc_cos (c, z, MPC_RNDNN);
  if (!mpfr_inf_p (MPC_RE (c)) || mpfr_sgn (MPC_RE (c)) <=0
      || !mpfr_nan_p (MPC_IM (c)))
    test_failed (z, c, z);

  /* cos(NaN +i*y) = NaN +i*NaN where 0<|y|<infinity */
  mpfr_set_ui (MPC_IM (z), 1, GMP_RNDN);
  mpfr_set_nan (MPC_RE (c99));
  mpc_cos (c, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (c)) && !mpfr_nan_p (MPC_IM (c)))
    test_failed (z, c, c99);
  mpc_conj (z, z, MPC_RNDNN);
  mpc_cos (c, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (c)) && !mpfr_nan_p (MPC_IM (c)))
    test_failed (z, c, c99);

  /* cos(NaN +i*0) = NaN +/-i*0 */
  mpfr_set_ui (MPC_IM (z), 0, GMP_RNDN);
  mpc_cos (c, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (c)) && !mpfr_zero_p (MPC_IM (c)))
    test_failed (z, c, z);

  /* cos(NaN -i*0) = NaN +/-i*0 */
  mpc_conj (z, z, MPC_RNDNN);
  mpc_cos (c, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (c)) && !mpfr_zero_p (MPC_IM (c)))
    test_failed (z, c, z);

  /* cos(+0 +i*NaN) = NaN +/-i*0 */
  mpc_mul_i (z, z, +1, MPC_RNDNN);
  mpc_cos (c, z, MPC_RNDNN);
  if (!mpfr_zero_p (MPC_IM (c)) || !mpfr_nan_p (MPC_RE (c)))
    test_failed (z, c, z);

  /* cos(-0 +i*NaN) = NaN -/+i*0 */
  mpc_neg (z, z, MPC_RNDNN);
  mpc_cos (c, z, MPC_RNDNN);
  if (!mpfr_zero_p (MPC_IM (c)) || !mpfr_nan_p (MPC_RE (c)))
    test_failed (z, c, z);

  /* cos(x +i*NaN) = NaN +i*NaN where x!=0 */
  mpfr_set_inf (MPC_RE (z), +1);
  mpc_cos (c, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (c)) && !mpfr_nan_p (MPC_IM (c)))
    test_failed (z, c, c99);
  mpc_neg (z, z, MPC_RNDNN);
  mpc_cos (c, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (c)) && !mpfr_nan_p (MPC_IM (c)))
    test_failed (z, c, c99);
  mpfr_set_ui (MPC_RE (z), +1, MPC_RNDNN);
  mpc_cos (c, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (c)) && !mpfr_nan_p (MPC_IM (c)))
    test_failed (z, c, c99);
  mpc_neg (z, z, MPC_RNDNN);
  mpc_cos (c, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (c)) && !mpfr_nan_p (MPC_IM (c)))
    test_failed (z, c, c99);

  mpfr_set_inf (MPC_RE(z), -1);
  /* cos(-Inf -i*Inf) = -/+Inf +i*NaN */
  mpfr_set_inf (MPC_IM(z), -1);
  mpfr_set_inf (MPC_RE(c99), -1);
  mpfr_set_nan (MPC_IM(c99));
  mpc_cos (c, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_IM (c)) || !mpfr_inf_p (MPC_RE (c)))
    test_failed (z, c, c99);

  /* cos(-Inf +i*Inf) = +/-Inf +i*NaN */
  mpc_conj (z, z, MPC_RNDNN);
  mpc_neg (c99, c99, MPC_RNDNN);
  mpc_cos (c, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_IM (c)) || !mpfr_inf_p (MPC_RE (c)))
    test_failed (z, c, c99);

  /* cos(+Inf -i*Inf) = -/+Inf +i*NaN */
  mpc_neg (z, z, MPC_RNDNN);
  mpc_neg (c99, c99, MPC_RNDNN);
  mpc_cos (c, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_IM (c)) || !mpfr_inf_p (MPC_RE (c)))
    test_failed (z, c, c99);

  /* cos(+Inf +i*Inf) = +/-Inf +i*NaN */
  mpc_conj (z, z, MPC_RNDNN);
  mpc_neg (c99, c99, MPC_RNDNN);
  mpc_cos (c, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_IM (c)) || !mpfr_inf_p (MPC_RE (c)))
    test_failed (z, c, c99);

  /* cos(x -i*Inf) = +Inf*(cos(x) +i*sin(x)) where 0<|x|<infinity */
  /* cos(x +i*Inf) = +Inf*(cos(x) -i*sin(x)) where 0<|x|<infinity */
  mpfr_set_ui (MPC_RE (z), 1, GMP_RNDN);
  mpfr_set_inf (MPC_RE (c99), +1);
  mpfr_set_inf (MPC_IM (c99), -1);
  mpc_cos (c, z, MPC_RNDUU);
  if (mpc_cmp (c, c99) != 0)
    test_failed (z, c, c99);
  mpc_conj (z, z, MPC_RNDNN);
  mpc_conj (c99, c99, MPC_RNDNN);
  mpc_cos (c, z, MPC_RNDUU);
  if (mpc_cmp (c, c99) != 0)
    test_failed (z, c, c99);
  mpfr_neg (MPC_RE (z), MPC_RE (z), GMP_RNDN);
  mpc_conj (c99, c99, MPC_RNDNN);
  mpc_cos (c, z, MPC_RNDUU);
  if (mpc_cmp (c, c99) != 0)
    test_failed (z, c, c99);
  mpc_conj (z, z, MPC_RNDNN);
  mpc_conj (c99, c99, MPC_RNDNN);
  mpc_cos (c, z, MPC_RNDUU);
  if (mpc_cmp (c, c99) != 0)
    test_failed (z, c, c99);

  /* cos(+0 -i*Inf) = +Inf +i*0 */
  mpfr_set_ui (MPC_RE (z), 0, GMP_RNDN);
  mpfr_set_inf (MPC_RE (c99), +1);
  mpfr_set_ui (MPC_IM (c99), 0, GMP_RNDN);
  mpc_cos (c, z, MPC_RNDNN);
  if (mpc_cmp (c, c99) != 0)
    test_failed (z, c, c99);

  /* cos(+0 +i*Inf) = +Inf -i*0 */
  mpc_conj (z, z, MPC_RNDNN);
  mpc_conj (c99, c99, MPC_RNDNN);
  mpc_cos (c, z, MPC_RNDNN);
  if (mpc_cmp (c, c99) != 0)
    test_failed (z, c, z);

  /* cos(-0 -i*Inf) = +Inf -i0 */
  mpc_neg (z, z, MPC_RNDNN);
  mpc_cos (c, z, MPC_RNDNN);
  if (mpc_cmp (c, c99) != 0)
    test_failed (z, c, c99);

  /* cos(-0 +i*Inf) = +Inf +i*0 */
  mpc_conj (z, z, MPC_RNDNN);
  mpc_conj (c99, c99, MPC_RNDNN);
  mpc_cos (c, z, MPC_RNDNN);
  if (mpc_cmp (c, c99) != 0)
    test_failed (z, c, c99);

  /* cos(-Inf +i*0) = NaN +/-i*0 */
  mpfr_set_inf (MPC_RE (z), -1);
  mpfr_set_ui (MPC_IM (z), 0, GMP_RNDN);
  mpfr_set_nan (MPC_RE (c99));
  mpfr_set_ui (MPC_IM (c99), 0, GMP_RNDN);
  mpc_cos (c, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (c)) || !mpfr_zero_p (MPC_IM (c)))
    test_failed (z, c, c99);

  /* cos(-Inf -i*0) = NaN -/+i*0 */
  mpc_conj (z, z, MPC_RNDNN);
  mpc_conj (c99, c99, MPC_RNDNN);
  mpc_cos (c, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (c)) || !mpfr_zero_p (MPC_IM (c)))
    test_failed (z, c, c99);

  /* cos(+Inf +i*0) = NaN -/+i*0 */
  mpc_neg (z, z, MPC_RNDNN);
  mpc_cos (c, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (c)) || !mpfr_zero_p (MPC_IM (c)))
    test_failed (z, c, c99);

  /* cos(+Inf -i*0) = NaN +/-i*0 */
  mpc_conj (z, z, MPC_RNDNN);
  mpc_conj (c99, c99, MPC_RNDNN);
  mpc_cos (c, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (c)) || !mpfr_zero_p (MPC_IM (c)))
    test_failed (z, c, c99);

  /* cos(+/-Inf +i*y) = NaN +i*NaN where 0<|y|<infinity*/
  mpfr_set_ui (MPC_IM (z), 1, GMP_RNDN);
  mpfr_set_nan (MPC_IM (c99));
  mpc_cos (c, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (c)) || !mpfr_nan_p (MPC_IM (c)))
    test_failed (z, c, c99);
  mpc_conj (z, z, MPC_RNDNN);
  mpc_cos (c, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (c)) || !mpfr_nan_p (MPC_IM (c)))
    test_failed (z, c, c99);
  mpc_neg (z, z, MPC_RNDNN);
  mpc_cos (c, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (c)) || !mpfr_nan_p (MPC_IM (c)))
    test_failed (z, c, c99);
  mpc_conj (z, z, MPC_RNDNN);
  mpc_cos (c, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (c)) || !mpfr_nan_p (MPC_IM (c)))
    test_failed (z, c, c99);

  mpc_clear (c99);
  mpc_clear (c);
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

      /* check that cos(I*b) = cosh(b) */
      mpfr_set_ui (MPC_RE (x), 0, GMP_RNDN);
      mpfr_random (MPC_IM (x));

      mpc_cos (z, x, MPC_RNDNN);
      mpfr_cosh (g, MPC_IM(x), GMP_RNDN);
      if (mpfr_cmp_ui (MPC_IM(z), 0) || mpfr_cmp (g, MPC_RE(z)))
        {
          fprintf (stderr, "Error in mpc_cos: cos(I*x) <> cosh(x)\n"
                   "got      ");
          mpc_out_str (stderr, 10, 0, z, MPC_RNDNN);
          fprintf (stderr, "\nexpected ");
          mpfr_set (MPC_RE (z), g, GMP_RNDN);
          mpfr_set_ui (MPC_IM (z), 0, GMP_RNDN);
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
