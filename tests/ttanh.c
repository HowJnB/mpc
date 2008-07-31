/* test file for mpc_tanh.

Copyright (C) 2008 Philippe Th\'eveny

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

#define TEST_FUNCTION mpc_tanh
#include "tgeneric.c"

static void
test_failed (mpc_t op, mpc_t get, mpc_t expected)
{
  printf ("mpc_tanh(op) failed\n with ");
  OUT (op);
  printf ("     ");
  OUT (get);
  OUT (expected);
  exit (1);
}

/* check special values as defined in C99 standard */
static void
special (void)
{
  mpc_t z;
  mpc_t t;

  mpc_init (z);
  mpc_init (t);

  /* tanh(NaN +i*NaN) = NaN +i*NaN */
  mpfr_set_nan (MPC_RE (z));
  mpfr_set_nan (MPC_IM (z));
  mpc_tanh (t, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (t)) || !mpfr_nan_p (MPC_IM (t)))
    test_failed (z, t, z);

  /* tanh(NaN -i*Inf) = NaN +i*NaN */
  mpfr_set_inf (MPC_IM (z), -1);
  mpc_tanh (t, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (t)) || !mpfr_nan_p (MPC_IM (t)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_nan (MPC_RE (c99));
      mpfr_set_nan (MPC_IM (c99));
      test_failed (z, t, c99);
    }

  /* tanh(NaN +i*Inf) = NaN +i*NaN */
  mpc_conj (z, z, MPC_RNDNN);
  mpc_tanh (t, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (t)) || !mpfr_nan_p (MPC_IM (t)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_nan (MPC_RE (c99));
      mpfr_set_nan (MPC_IM (c99));
      test_failed (z, t, c99);
    }

  /* tanh(NaN +i*y) = NaN +i*NaN when |y|>0 */
  mpfr_set_si (MPC_IM (z), -1, GMP_RNDN);
  mpc_tanh (t, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (t)) || !mpfr_nan_p (MPC_IM (t)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_nan (MPC_RE (c99));
      mpfr_set_nan (MPC_IM (c99));
      test_failed (z, t, c99);
    }
  mpc_conj (z, z, MPC_RNDNN);
  mpc_tanh (t, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (t)) || !mpfr_nan_p (MPC_IM (t)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_nan (MPC_RE (c99));
      mpfr_set_nan (MPC_IM (c99));
      test_failed (z, t, c99);
    }

  /* tanh(x +i*NaN) = NaN +i*NaN when x is a finite number */
  mpfr_set_si (MPC_RE (z), -1, GMP_RNDN);
  mpfr_set_nan (MPC_IM (z));
  mpc_tanh (t, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (t)) || !mpfr_nan_p (MPC_IM (t)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_nan (MPC_RE (c99));
      mpfr_set_nan (MPC_IM (c99));
      test_failed (z, t, c99);
    }
  mpc_neg (z, z, MPC_RNDNN);
  mpc_tanh (t, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (t)) || !mpfr_nan_p (MPC_IM (t)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_nan (MPC_RE (c99));
      mpfr_set_nan (MPC_IM (c99));
      test_failed (z, t, c99);
    }
  mpfr_set_ui (MPC_RE (z), 0, GMP_RNDN);
  mpfr_set_nan (MPC_IM (z));
  mpc_tanh (t, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (t)) || !mpfr_nan_p (MPC_IM (t)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_nan (MPC_RE (c99));
      mpfr_set_nan (MPC_IM (c99));
      test_failed (z, t, c99);
    }
  mpc_neg (z, z, MPC_RNDNN);
  mpc_tanh (t, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (t)) || !mpfr_nan_p (MPC_IM (t)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_nan (MPC_RE (c99));
      mpfr_set_nan (MPC_IM (c99));
      test_failed (z, t, c99);
    }

  /* tanh(x +i*+/-Inf) = NaN +i*NaN when x is a finite number */
  mpfr_set_si (MPC_RE (z), -1, GMP_RNDN);
  mpfr_set_inf (MPC_IM (z), +1);
  mpc_tanh (t, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (t)) || !mpfr_nan_p (MPC_IM (t)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_nan (MPC_RE (c99));
      mpfr_set_nan (MPC_IM (c99));
      test_failed (z, t, c99);
    }
  mpc_conj (z, z, MPC_RNDNN);
  mpc_tanh (t, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (t)) || !mpfr_nan_p (MPC_IM (t)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_nan (MPC_RE (c99));
      mpfr_set_nan (MPC_IM (c99));
      test_failed (z, t, c99);
    }
  mpc_neg (z, z, MPC_RNDNN);
  mpc_tanh (t, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (t)) || !mpfr_nan_p (MPC_IM (t)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_nan (MPC_RE (c99));
      mpfr_set_nan (MPC_IM (c99));
      test_failed (z, t, c99);
    }
  mpc_conj (z, z, MPC_RNDNN);
  mpc_tanh (t, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (t)) || !mpfr_nan_p (MPC_IM (t)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_nan (MPC_RE (c99));
      mpfr_set_nan (MPC_IM (c99));
      test_failed (z, t, c99);
    }
  mpfr_set_ui (MPC_RE (z), 0, GMP_RNDN);
  mpfr_set_inf (MPC_IM (z), +1);
  mpc_tanh (t, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (t)) || !mpfr_nan_p (MPC_IM (t)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_nan (MPC_RE (c99));
      mpfr_set_nan (MPC_IM (c99));
      test_failed (z, t, c99);
    }
  mpc_conj (z, z, MPC_RNDNN);
  mpc_tanh (t, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (t)) || !mpfr_nan_p (MPC_IM (t)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_nan (MPC_RE (c99));
      mpfr_set_nan (MPC_IM (c99));
      test_failed (z, t, c99);
    }
  mpc_neg (z, z, MPC_RNDNN);
  mpc_tanh (t, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (t)) || !mpfr_nan_p (MPC_IM (t)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_nan (MPC_RE (c99));
      mpfr_set_nan (MPC_IM (c99));
      test_failed (z, t, c99);
    }
  mpc_conj (z, z, MPC_RNDNN);
  mpc_tanh (t, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (t)) || !mpfr_nan_p (MPC_IM (t)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_nan (MPC_RE (c99));
      mpfr_set_nan (MPC_IM (c99));
      test_failed (z, t, c99);
    }

  /* tanh(+Inf +i*NaN) = 1 +i*+/-0 */
  mpfr_set_inf (MPC_RE (z), +1);
  mpfr_set_nan (MPC_IM (z));
  mpc_tanh (t, z, MPC_RNDNN);
  if (mpfr_cmp_si (MPC_RE (t), +1) != 0 || !mpfr_zero_p (MPC_IM (t)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpc_set_ui_ui (c99, 1, 0, MPC_RNDNN);
      test_failed (z, t, c99);
    }

  /* tanh(-Inf +i*NaN) = -1 +i*-/+0 */
  mpc_neg (z, z, MPC_RNDNN);
  mpc_tanh (t, z, MPC_RNDNN);
  if (mpfr_cmp_si (MPC_RE (t), -1) != 0 || !mpfr_zero_p (MPC_IM (t)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpc_set_si_si (c99, -1, 0, MPC_RNDNN);
      test_failed (z, t, c99);
    }

  /* tanh(-Inf +i*Inf) = -1 +i*+/-0 */
  mpfr_set_inf (MPC_IM (z), +1);
  mpc_tanh (t, z, MPC_RNDNN);
  if (mpfr_cmp_si (MPC_RE (t), -1) != 0 || !mpfr_zero_p (MPC_IM (t)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpc_set_si_si (c99, -1, 0, MPC_RNDNN);
      test_failed (z, t, c99);
    }

  /* tanh(-Inf -i*Inf) = -1 +i*-/+0 */
  mpc_conj (z, z, MPC_RNDNN);
  mpc_tanh (t, z, MPC_RNDNN);
  if (mpfr_cmp_si (MPC_RE (t), -1) != 0 || !mpfr_zero_p (MPC_IM (t)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpc_set_si_si (c99, -1, 0, MPC_RNDNN);
      test_failed (z, t, c99);
    }

  /* tanh(+Inf +i*Inf) = +1 +i*+/-0 */
  mpc_neg (z, z, MPC_RNDNN);
  mpc_tanh (t, z, MPC_RNDNN);
  if (mpfr_cmp_si (MPC_RE (t), +1) != 0 || !mpfr_zero_p (MPC_IM (t)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpc_set_si_si (c99, 1, 0, MPC_RNDNN);
      test_failed (z, t, c99);
    }

  /* tanh(+Inf -i*Inf) = +1 +i*-/+0 */
  mpc_conj (z, z, MPC_RNDNN);
  mpc_tanh (t, z, MPC_RNDNN);
  if (mpfr_cmp_si (MPC_RE (t), +1) != 0 || !mpfr_zero_p (MPC_IM (t)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpc_set_si_si (c99, 1, 0, MPC_RNDNN);
      test_failed (z, t, c99);
    }

  /* tanh(NaN +i*0) = NaN +i*0 */
  mpfr_set_nan (MPC_RE (z));
  mpfr_set_ui (MPC_IM (z), 0, GMP_RNDN);
  mpc_tanh (t, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (t))
      || !mpfr_zero_p (MPC_IM (t)) || mpfr_signbit (MPC_IM (t)))
    test_failed (z, t, z);

  /* tanh(NaN -i*0) = NaN -i*0 */
  mpc_neg (z, z, MPC_RNDNN);
  mpc_tanh (t, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (t))
      || !mpfr_zero_p (MPC_IM (t)) || !mpfr_signbit (MPC_IM (t)))
    test_failed (z, t, z);

  /* tanh(+Inf +i*0) = +1 +i*0 */
  mpfr_set_inf (MPC_RE (z), +1);
  mpfr_set_ui (MPC_IM (z), 0, MPC_RNDNN);
  mpc_tanh (t, z, MPC_RNDNN);
  if (mpfr_cmp_si (MPC_RE (t), +1) != 0
      || !mpfr_zero_p (MPC_IM (t)) || mpfr_signbit (MPC_IM (t)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpc_set_si_si (c99, +1, 0, MPC_RNDNN);
      test_failed (z, t, c99);
    }

  /* tanh(-Inf -i*0) = -1 -i*0 */
  mpc_neg (z, z, MPC_RNDNN);
  mpc_tanh (t, z, MPC_RNDNN);
  if (mpfr_cmp_si (MPC_RE (t), -1) != 0
      || !mpfr_zero_p (MPC_IM (t)) || !mpfr_signbit (MPC_IM (t)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpc_set_ui_ui (c99, 1, 0, MPC_RNDNN);
      mpc_neg (c99, c99, MPC_RNDNN);
      test_failed (z, t, c99);
    }

  /* tanh(-Inf +i*0) = -1 +i*0 */
  mpc_conj (z, z, MPC_RNDNN);
  mpc_tanh (t, z, MPC_RNDNN);
  if (mpfr_cmp_si (MPC_RE (t), -1) != 0
      || !mpfr_zero_p (MPC_IM (t)) || mpfr_signbit (MPC_IM (t)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpc_set_si_si (c99, -1, 0, MPC_RNDNN);
      test_failed (z, t, c99);
    }

  /* tanh(+Inf -i*0) = +1 -i*0 */
  mpc_neg (z, z, MPC_RNDNN);
  mpc_tanh (t, z, MPC_RNDNN);
  if (mpfr_cmp_si (MPC_RE (t), +1) != 0
      || !mpfr_zero_p (MPC_IM (t)) || !mpfr_signbit (MPC_IM (t)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpc_set_ui_ui (c99, 1, 0, MPC_RNDNN);
      mpc_conj (c99, c99, MPC_RNDNN);
      test_failed (z, t, c99);
    }

  /* tanh(+Inf +i*y) = 1 +i*0*sin 2y */
  mpfr_set_inf (MPC_RE (z), +1);
  mpfr_set_ui (MPC_IM (z), +1, MPC_RNDNN);
  mpc_tanh (t, z, MPC_RNDNN);
  if (mpfr_cmp_si (MPC_RE (t), 1) != 0
      || !mpfr_zero_p (MPC_IM (t)) || mpfr_signbit (MPC_IM (t)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpc_set_ui_ui (c99, 1, 0, MPC_RNDNN);
      test_failed (z, t, c99);
    }
  mpc_conj (z, z, MPC_RNDNN);
  mpc_tanh (t, z, MPC_RNDNN);
  if (mpfr_cmp_si (MPC_RE (t), 1) != 0
      || !mpfr_zero_p (MPC_IM (t)) || !mpfr_signbit (MPC_IM (t)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpc_set_ui_ui (c99, 1, 0, MPC_RNDNN);
      mpc_conj (c99, c99, MPC_RNDNN);
      test_failed (z, t, c99);
    }
  mpfr_set_ui (MPC_IM (z), +2, MPC_RNDNN);
  mpc_tanh (t, z, MPC_RNDNN);
  if (mpfr_cmp_si (MPC_RE (t), 1) != 0
      || !mpfr_zero_p (MPC_IM (t)) || !mpfr_signbit (MPC_IM (t)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpc_set_ui_ui (c99, 1, 0, MPC_RNDNN);
      mpc_conj (c99, c99, MPC_RNDNN);
      test_failed (z, t, c99);
    }
  mpc_conj (z, z, MPC_RNDNN);
  mpc_tanh (t, z, MPC_RNDNN);
  if (mpfr_cmp_si (MPC_RE (t), 1) != 0
      || !mpfr_zero_p (MPC_IM (t)) || mpfr_signbit (MPC_IM (t)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpc_set_ui_ui (c99, 1, 0, MPC_RNDNN);
      test_failed (z, t, c99);
    }

  /* tanh(-Inf +i*y) = -1 +i*0*sin 2y */
  mpfr_set_inf (MPC_RE (z), -1);
  mpfr_set_ui (MPC_IM (z), +1, MPC_RNDNN);
  mpc_tanh (t, z, MPC_RNDNN);
  if (mpfr_cmp_si (MPC_RE (t), -1) != 0
      || !mpfr_zero_p (MPC_IM (t)) || mpfr_signbit (MPC_IM (t)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpc_set_ui_ui (c99, 1, 0, MPC_RNDNN);
      test_failed (z, t, c99);
    }
  mpc_conj (z, z, MPC_RNDNN);
  mpc_tanh (t, z, MPC_RNDNN);
  if (mpfr_cmp_si (MPC_RE (t), -1) != 0
      || !mpfr_zero_p (MPC_IM (t)) || !mpfr_signbit (MPC_IM (t)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpc_set_ui_ui (c99, 1, 0, MPC_RNDNN);
      mpc_conj (c99, c99, MPC_RNDNN);
      test_failed (z, t, c99);
    }
  mpfr_set_ui (MPC_IM (z), +2, MPC_RNDNN);
  mpc_tanh (t, z, MPC_RNDNN);
  if (mpfr_cmp_si (MPC_RE (t), -1) != 0
      || !mpfr_zero_p (MPC_IM (t)) || !mpfr_signbit (MPC_IM (t)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpc_set_ui_ui (c99, 1, 0, MPC_RNDNN);
      mpc_conj (c99, c99, MPC_RNDNN);
      test_failed (z, t, c99);
    }
  mpc_conj (z, z, MPC_RNDNN);
  mpc_tanh (t, z, MPC_RNDNN);
  if (mpfr_cmp_si (MPC_RE (t), -1) != 0
      || !mpfr_zero_p (MPC_IM (t)) || mpfr_signbit (MPC_IM (t)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpc_set_ui_ui (c99, 1, 0, MPC_RNDNN);
      test_failed (z, t, c99);
    }

  /* tanh(+0 +i*0) = +0 +i*0 */
  mpc_set_ui_ui (z, 0, 0, MPC_RNDNN);
  mpc_tanh (t, z, MPC_RNDNN);
  if (mpc_cmp (t, z) != 0 )
    test_failed (z, t, z);

  /* tanh(+0 -i*0) = +0 -i*0 */
  mpc_conj (z, z, MPC_RNDNN);
  mpc_tanh (t, z, MPC_RNDNN);
  if (mpc_cmp (t, z) != 0 )
    test_failed (z, t, z);

  /* tanh(-0 +i*0) = -0 +i*0 */
  mpc_neg (z, z, MPC_RNDNN);
  mpc_tanh (t, z, MPC_RNDNN);
  if (mpc_cmp (t, z) != 0 )
    test_failed (z, t, z);

  /* tanh(-0 -i*0) = -0 -i*0 */
  mpc_conj (z, z, MPC_RNDNN);
  mpc_tanh (t, z, MPC_RNDNN);
  if (mpc_cmp (t, z) != 0 )
    test_failed (z, t, z);

  mpc_clear (t);
  mpc_clear (z);
}

static void
pure_real_argument (void)
{
  /* tanh(x -i*0) = tanh(x) -i*0 */
  /* tanh(x +i*0) = tanh(x) +i*0 */
  mpc_t u;
  mpc_t z;
  mpc_t tanh_z;

  mpc_init (u);
  mpc_init (z);
  mpc_init (tanh_z);

  /* tanh(1 +i*0) = tanh(1) +i*0 */
  mpc_set_ui_ui (z, 1, 0, MPC_RNDNN);
  mpfr_tanh (MPC_RE (u), MPC_RE (z), GMP_RNDN);
  mpfr_set_ui (MPC_IM (u), 0, GMP_RNDN);
  mpc_tanh (tanh_z, z, MPC_RNDNN);
  if (mpc_cmp (tanh_z, u) != 0 || mpfr_signbit (MPC_IM (tanh_z)))
    test_failed (z, tanh_z, u);

  /* tanh(1 -i*0) = tanh(1) -i*0 */
  mpc_conj (z, z, MPC_RNDNN);
  mpc_conj (u, u, MPC_RNDNN);
  mpc_tanh (tanh_z, z, MPC_RNDNN);
  if (mpc_cmp (tanh_z, u) != 0 || !mpfr_signbit (MPC_IM (tanh_z)))
    test_failed (z, tanh_z, u);

  /* tanh(-1 +i*0) = -tanh(1) +i*0 */
  mpc_neg (z, z, MPC_RNDNN);
  mpc_neg (u, u, MPC_RNDNN);
  mpc_tanh (tanh_z, z, MPC_RNDNN);
  if (mpc_cmp (tanh_z, u) != 0 || mpfr_signbit (MPC_IM (tanh_z)))
    test_failed (z, tanh_z, u);

  /* tanh(-1 -i*0) = -tanh(1) -i*0 */
  mpc_conj (z, z, MPC_RNDNN);
  mpc_conj (u, u, MPC_RNDNN);
  mpc_tanh (tanh_z, z, MPC_RNDNN);
  if (mpc_cmp (tanh_z, u) != 0 || !mpfr_signbit (MPC_IM (tanh_z)))
    test_failed (z, tanh_z, u);

  mpc_clear (tanh_z);
  mpc_clear (z);
  mpc_clear (u);
}

static void
pure_imaginary_argument (void)
{
  /* tanh(+0 +i*y) = +0 +i*tan y */
  /* tanh(-0 +i*y) = -0 +i*tan y */
  mpc_t u;
  mpc_t z;
  mpc_t tanh_z;

  mpc_init (u);
  mpc_init (z);
  mpc_init (tanh_z);

  /* tanh(+0 +i) = +0 +i*tan(1) */
  mpc_set_ui_ui (z, 0, 1, MPC_RNDNN);
  mpfr_tan (MPC_IM (u), MPC_IM (z), GMP_RNDN);
  mpfr_set_ui (MPC_RE (u), 0, GMP_RNDN);
  mpc_tanh (tanh_z, z, MPC_RNDNN);
  if (mpc_cmp (tanh_z, u) != 0 || mpfr_signbit (MPC_RE (tanh_z)))
    test_failed (z, tanh_z, u);

  /* tanh(+0 -i) = +0 -i*tan(1) */
  mpc_conj (z, z, MPC_RNDNN);
  mpc_conj (u, u, MPC_RNDNN);
  mpc_tanh (tanh_z, z, MPC_RNDNN);
  if (mpc_cmp (tanh_z, u) != 0 || mpfr_signbit (MPC_RE (tanh_z)))
    test_failed (z, tanh_z, u);

  /* tanh(-0 +i) = -0 +i*tan(1) */
  mpc_neg (z, z, MPC_RNDNN);
  mpc_neg (u, u, MPC_RNDNN);
  mpc_tanh (tanh_z, z, MPC_RNDNN);
  if (mpc_cmp (tanh_z, u) != 0 || !mpfr_signbit (MPC_RE (tanh_z)))
    test_failed (z, tanh_z, u);

  /* tanh(-0 -i) = -0 -i*tan(1) */
  mpc_conj (z, z, MPC_RNDNN);
  mpc_conj (u, u, MPC_RNDNN);
  mpc_tanh (tanh_z, z, MPC_RNDNN);
  if (mpc_cmp (tanh_z, u) != 0 || !mpfr_signbit (MPC_RE (tanh_z)))
    test_failed (z, tanh_z, u);

  mpc_clear (tanh_z);
  mpc_clear (z);
  mpc_clear (u);
}

static void
check_53bits (void)
{
  mpc_t z;
  mpc_t tanh_z;
  mpc_t t;

  mpc_init2 (z, 53);
  mpc_init2 (tanh_z, 53);
  mpc_init2 (t, 53);

  /* tanh(z) is near 0.5+2^(-53)+i */
  mpfr_set_str (MPC_RE (z), "1E938CBCEB16DFp-55", 16, GMP_RNDN);
  mpfr_set_str (MPC_IM (z), "1B1F56FDEEF00Fp-53", 16, GMP_RNDN);
  mpfr_set_str (MPC_RE (t), "10000000000001p-53", 16, GMP_RNDN);
  mpfr_set_str (MPC_IM (t), "1FFFFFFFFFFFFFp-53", 16, GMP_RNDN);
  mpc_tanh (tanh_z, z, MPC_RNDNN);
  if (mpc_cmp (tanh_z, t) != 0)
    test_failed (z, tanh_z, t);

  mpc_clear (t);
  mpc_clear (tanh_z);
  mpc_clear (z);
}

int
main (void)
{
  test_start ();

  special ();
  pure_real_argument ();
  pure_imaginary_argument ();

  tgeneric (2, 512, 10);

  check_53bits ();

  test_end ();

  return 0;
}
