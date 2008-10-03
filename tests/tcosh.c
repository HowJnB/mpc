/* test file for mpc_cosh.

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
#define TEST_FUNCTION mpc_cosh
#include "tgeneric_cc.c"

#include "read_data_cc.c"

static void
test_failed (mpc_t op, mpc_t get, mpc_t expected)
{
  printf ("mpc_cosh(op) failed\n with ");
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
  mpc_t c;

  mpc_init (z);
  mpc_init (c);

  /* cosh(NaN +i*NaN) = NaN +i*NaN */
  mpfr_set_nan (MPC_RE (z));
  mpfr_set_nan (MPC_IM (z));
  mpc_cosh (c, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (c)) || !mpfr_nan_p (MPC_IM (c)))
    test_failed (z, c, z);

  /* cosh(NaN -i*Inf) = NaN +i*NaN */
  mpfr_set_inf (MPC_IM (z), -1);
  mpc_cosh (c, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (c)) || !mpfr_nan_p (MPC_IM (c)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_nan (MPC_RE (c99));
      mpfr_set_nan (MPC_IM (c99));
      test_failed (z, c, c99);
    }

  /* cosh(NaN +i*Inf) = NaN +i*NaN */
  mpc_conj (z, z, MPC_RNDNN);
  mpc_cosh (c, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (c)) || !mpfr_nan_p (MPC_IM (c)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_nan (MPC_RE (c99));
      mpfr_set_nan (MPC_IM (c99));
      test_failed (z, c, c99);
    }

  /* cosh(NaN +i*y) = NaN +i*NaN when |y|>0 */
  mpfr_set_si (MPC_IM (z), -1, GMP_RNDN);
  mpc_cosh (c, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (c)) || !mpfr_nan_p (MPC_IM (c)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_nan (MPC_RE (c99));
      mpfr_set_nan (MPC_IM (c99));
      test_failed (z, c, c99);
    }
  mpc_conj (z, z, MPC_RNDNN);
  mpc_cosh (c, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (c)) || !mpfr_nan_p (MPC_IM (c)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_nan (MPC_RE (c99));
      mpfr_set_nan (MPC_IM (c99));
      test_failed (z, c, c99);
    }

  /* cosh(x +i*NaN) = NaN +i*NaN when Inf>|x|>0 */
  mpfr_set_si (MPC_RE (z), -1, GMP_RNDN);
  mpfr_set_nan (MPC_IM (z));
  mpc_cosh (c, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (c)) || !mpfr_nan_p (MPC_IM (c)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_nan (MPC_RE (c99));
      mpfr_set_nan (MPC_IM (c99));
      test_failed (z, c, c99);
    }
  mpc_neg (z, z, MPC_RNDNN);
  mpc_cosh (c, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (c)) || !mpfr_nan_p (MPC_IM (c)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_nan (MPC_RE (c99));
      mpfr_set_nan (MPC_IM (c99));
      test_failed (z, c, c99);
    }

  /* cosh(x +i*+/-Inf) = NaN +i*NaN when Inf>|x|>0 */
  mpfr_set_si (MPC_RE (z), -1, GMP_RNDN);
  mpfr_set_inf (MPC_IM (z), +1);
  mpc_cosh (c, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (c)) || !mpfr_nan_p (MPC_IM (c)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_nan (MPC_RE (c99));
      mpfr_set_nan (MPC_IM (c99));
      test_failed (z, c, c99);
    }
  mpc_conj (z, z, MPC_RNDNN);
  mpc_cosh (c, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (c)) || !mpfr_nan_p (MPC_IM (c)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_nan (MPC_RE (c99));
      mpfr_set_nan (MPC_IM (c99));
      test_failed (z, c, c99);
    }
  mpc_neg (z, z, MPC_RNDNN);
  mpc_cosh (c, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (c)) || !mpfr_nan_p (MPC_IM (c)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_nan (MPC_RE (c99));
      mpfr_set_nan (MPC_IM (c99));
      test_failed (z, c, c99);
    }
  mpc_conj (z, z, MPC_RNDNN);
  mpc_cosh (c, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (c)) || !mpfr_nan_p (MPC_IM (c)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_nan (MPC_RE (c99));
      mpfr_set_nan (MPC_IM (c99));
      test_failed (z, c, c99);
    }

  /* cosh(+/-Inf +i*NaN) = +Inf +i*NaN */
  mpfr_set_inf (MPC_RE (z), +1);
  mpfr_set_nan (MPC_IM (z));
  mpc_cosh (c, z, MPC_RNDNN);
  if (!mpfr_inf_p (MPC_RE (c)) || mpfr_signbit (MPC_RE (c))
      || !mpfr_nan_p (MPC_IM (c)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_inf (MPC_RE (c99), +1);
      mpfr_set_nan (MPC_IM (c99));
      test_failed (z, c, c99);
    }
  mpc_neg (z, z, MPC_RNDNN);
  mpc_cosh (c, z, MPC_RNDNN);
  if (!mpfr_inf_p (MPC_RE (c)) || mpfr_signbit (MPC_RE (c))
      || !mpfr_nan_p (MPC_IM (c)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_inf (MPC_RE (c99), +1);
      mpfr_set_nan (MPC_IM (c99));
      test_failed (z, c, c99);
    }

  /* cosh(+/-Inf +i*+/-Inf) = +/-Inf +i*NaN */
  mpfr_set_inf (MPC_IM (z), +1);
  mpc_cosh (c, z, MPC_RNDNN);
  if (!mpfr_inf_p (MPC_RE (c)) || !mpfr_nan_p (MPC_IM (c)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_inf (MPC_RE (c99), +1);
      mpfr_set_nan (MPC_IM (c99));
      test_failed (z, c, c99);
    }
  mpc_conj (z, z, MPC_RNDNN);
  mpc_cosh (c, z, MPC_RNDNN);
  if (!mpfr_inf_p (MPC_RE (c)) || !mpfr_nan_p (MPC_IM (c)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_inf (MPC_RE (c99), +1);
      mpfr_set_nan (MPC_IM (c99));
      test_failed (z, c, c99);
    }
  mpc_neg (z, z, MPC_RNDNN);
  mpc_cosh (c, z, MPC_RNDNN);
  if (!mpfr_inf_p (MPC_RE (c)) || !mpfr_nan_p (MPC_IM (c)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_inf (MPC_RE (c99), +1);
      mpfr_set_nan (MPC_IM (c99));
      test_failed (z, c, c99);
    }
  mpc_conj (z, z, MPC_RNDNN);
  mpc_cosh (c, z, MPC_RNDNN);
  if (!mpfr_inf_p (MPC_RE (c)) || !mpfr_nan_p (MPC_IM (c)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_inf (MPC_RE (c99), +1);
      mpfr_set_nan (MPC_IM (c99));
      test_failed (z, c, c99);
    }

  /* cosh(+/-0 +i*NaN) = NaN +i*+/-0 */
  /* cosh(+0 +i*Inf) = cosh(-0 -i*Inf) = NaN +i*+/-0 */
  mpfr_set_ui (MPC_RE (z), 0, GMP_RNDN);
  mpfr_set_nan (MPC_IM (z));
  mpc_cosh (c, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (c)) || !mpfr_zero_p (MPC_IM (c)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_nan (MPC_RE (c99));
      mpfr_set_ui (MPC_IM (c99), 0, GMP_RNDN);
      test_failed (z, c, c99);
    }
  mpc_neg (z, z, MPC_RNDNN);
  mpc_cosh (c, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (c)) || !mpfr_zero_p (MPC_IM (c)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_nan (MPC_RE (c99));
      mpfr_set_ui (MPC_IM (c99), 0, GMP_RNDN);
      test_failed (z, c, c99);
    }
  mpfr_set_ui (MPC_RE (z), 0, GMP_RNDN);
  mpfr_set_inf (MPC_IM (z), +1);
  mpc_cosh (c, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (c)) || !mpfr_zero_p (MPC_IM (c)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_nan (MPC_RE (c99));
      mpfr_set_ui (MPC_IM (c99), 0, GMP_RNDN);
      test_failed (z, c, c99);
    }
  mpc_neg (z, z, MPC_RNDNN);
  mpc_cosh (c, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (c)) || !mpfr_zero_p (MPC_IM (c)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_nan (MPC_RE (c99));
      mpfr_set_ui (MPC_IM (c99), 0, GMP_RNDN);
      test_failed (z, c, c99);
    }

  /* cosh(-0 +i*Inf) = cosh(+0 -i*Inf) = NaN +i*-/+0 */
  mpc_cosh (c, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (c)) || !mpfr_zero_p (MPC_IM (c)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_nan (MPC_RE (c99));
      mpfr_set_ui (MPC_IM (c99), 0, GMP_RNDN);
      test_failed (z, c, c99);
    }
  mpc_neg (z, z, MPC_RNDNN);
  mpc_cosh (c, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (c)) || !mpfr_zero_p (MPC_IM (c)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_nan (MPC_RE (c99));
      mpfr_set_ui (MPC_IM (c99), 0, GMP_RNDN);
      test_failed (z, c, c99);
    }

  /* cosh(NaN +i*0) = NaN +i*+/-0 */
  /* cosh(NaN -i*0) = NaN +i*-/+0 */
  mpfr_set_nan (MPC_RE (z));
  mpfr_set_ui (MPC_IM (z), 0, GMP_RNDN);
  mpc_cosh (c, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (c)) || !mpfr_zero_p (MPC_IM (c)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_nan (MPC_RE (c99));
      mpfr_set_ui (MPC_IM (c99), 0, GMP_RNDN);
      test_failed (z, c, c99);
    }
  mpc_neg (z, z, MPC_RNDNN);
  mpc_cosh (c, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (c)) || !mpfr_zero_p (MPC_IM (c)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_nan (MPC_RE (c99));
      mpfr_set_ui (MPC_IM (c99), 0, GMP_RNDN);
      test_failed (z, c, c99);
    }

  /* cosh(+Inf +i*0) = cosh(-Inf -i*0) = +Inf +i*0 */
  /* cosh(-Inf +i*0) = cosh(+Inf -i*0) = +Inf -i*0 */
  mpfr_set_inf (MPC_RE (z), +1);
  mpfr_set_ui (MPC_IM (z), 0, MPC_RNDNN);
  mpc_cosh (c, z, MPC_RNDNN);
  if (!mpfr_inf_p (MPC_RE (c)) || mpfr_signbit (MPC_RE (c))
      || !mpfr_zero_p (MPC_IM (c)) || mpfr_signbit (MPC_IM (c)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_inf (MPC_RE (c99), +1);
      mpfr_set_ui (MPC_IM (c99), 0, MPC_RNDNN);
      test_failed (z, c, c99);
    }
  mpc_neg (z, z, MPC_RNDNN);
  mpc_cosh (c, z, MPC_RNDNN);
  if (!mpfr_inf_p (MPC_RE (c)) || mpfr_signbit (MPC_RE (c))
      || !mpfr_zero_p (MPC_IM (c)) || mpfr_signbit (MPC_IM (c)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_inf (MPC_RE (c99), +1);
      mpfr_set_ui (MPC_IM (c99), 0, MPC_RNDNN);
      test_failed (z, c, c99);
    }
  mpc_conj (z, z, MPC_RNDNN);
  mpc_cosh (c, z, MPC_RNDNN);
  if (!mpfr_inf_p (MPC_RE (c)) || mpfr_signbit (MPC_RE (c))
      || !mpfr_zero_p (MPC_IM (c)) || !mpfr_signbit (MPC_IM (c)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_inf (MPC_RE (c99), +1);
      mpfr_set_ui (MPC_IM (c99), 0, MPC_RNDNN);
      mpc_conj (c99, c99, MPC_RNDNN);
      test_failed (z, c, c99);
    }
  mpc_neg (z, z, MPC_RNDNN);
  mpc_cosh (c, z, MPC_RNDNN);
  if (!mpfr_inf_p (MPC_RE (c)) || mpfr_signbit (MPC_RE (c))
      || !mpfr_zero_p (MPC_IM (c)) || !mpfr_signbit (MPC_IM (c)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_inf (MPC_RE (c99), +1);
      mpfr_set_ui (MPC_IM (c99), 0, MPC_RNDNN);
      mpc_conj (c99, c99, MPC_RNDNN);
      test_failed (z, c, c99);
    }

  /* cosh(+Inf +i*y) = +Inf * (cos y +i*sin y) */
  mpfr_set_inf (MPC_RE (z), +1);
  mpfr_set_ui (MPC_IM (z), +1, MPC_RNDNN);
  mpc_cosh (c, z, MPC_RNDNN);
  if (!mpfr_inf_p (MPC_RE (c)) || mpfr_signbit (MPC_RE (c))
      || !mpfr_inf_p (MPC_IM (c)) || mpfr_signbit (MPC_IM (c)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_inf (MPC_RE (c99), +1);
      mpfr_set_inf (MPC_IM (c99), +1);
      test_failed (z, c, c99);
    }
  mpc_conj (z, z, MPC_RNDNN);
  mpc_cosh (c, z, MPC_RNDNN);
  if (!mpfr_inf_p (MPC_RE (c)) || mpfr_signbit (MPC_RE (c))
      || !mpfr_inf_p (MPC_IM (c)) || !mpfr_signbit (MPC_IM (c)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_inf (MPC_RE (c99), +1);
      mpfr_set_inf (MPC_IM (c99), -1);
      test_failed (z, c, c99);
    }
  mpfr_set_ui (MPC_IM (z), +2, MPC_RNDNN);
  mpc_cosh (c, z, MPC_RNDNN);
  if (!mpfr_inf_p (MPC_RE (c)) || !mpfr_signbit (MPC_RE (c))
      || !mpfr_inf_p (MPC_IM (c)) || mpfr_signbit (MPC_IM (c)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_inf (MPC_RE (c99), -1);
      mpfr_set_inf (MPC_IM (c99), +1);
      test_failed (z, c, c99);
    }
  mpc_conj (z, z, MPC_RNDNN);
  mpc_cosh (c, z, MPC_RNDNN);
  if (!mpfr_inf_p (MPC_RE (c)) || !mpfr_signbit (MPC_RE (c))
      || !mpfr_inf_p (MPC_IM (c)) || !mpfr_signbit (MPC_IM (c)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_inf (MPC_RE (c99), -1);
      mpfr_set_inf (MPC_IM (c99), -1);
      test_failed (z, c, c99);
    }

  /* cosh(-Inf +i*y) = +Inf * (cos y -i*sin y) */
  mpfr_set_inf (MPC_RE (z), -1);
  mpfr_set_ui (MPC_IM (z), +1, MPC_RNDNN);
  mpc_cosh (c, z, MPC_RNDNN);
  if (!mpfr_inf_p (MPC_RE (c)) || mpfr_signbit (MPC_RE (c))
      || !mpfr_inf_p (MPC_IM (c)) || !mpfr_signbit (MPC_IM (c)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_inf (MPC_RE (c99), +1);
      mpfr_set_inf (MPC_IM (c99), -1);
      test_failed (z, c, c99);
    }
  mpc_conj (z, z, MPC_RNDNN);
  mpc_cosh (c, z, MPC_RNDNN);
  if (!mpfr_inf_p (MPC_RE (c)) || mpfr_signbit (MPC_RE (c))
      || !mpfr_inf_p (MPC_IM (c)) || mpfr_signbit (MPC_IM (c)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_inf (MPC_RE (c99), +1);
      mpfr_set_inf (MPC_IM (c99), +1);
      test_failed (z, c, c99);
    }
  mpfr_set_ui (MPC_IM (z), +2, MPC_RNDNN);
  mpc_cosh (c, z, MPC_RNDNN);
  if (!mpfr_inf_p (MPC_RE (c)) || !mpfr_signbit (MPC_RE (c))
      || !mpfr_inf_p (MPC_IM (c)) || !mpfr_signbit (MPC_IM (c)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_inf (MPC_RE (c99), -1);
      mpfr_set_inf (MPC_IM (c99), -1);
      test_failed (z, c, c99);
    }
  mpc_conj (z, z, MPC_RNDNN);
  mpc_cosh (c, z, MPC_RNDNN);
  if (!mpfr_inf_p (MPC_RE (c)) || !mpfr_signbit (MPC_RE (c))
      || !mpfr_inf_p (MPC_IM (c)) || mpfr_signbit (MPC_IM (c)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_inf (MPC_RE (c99), -1);
      mpfr_set_inf (MPC_IM (c99), +1);
      test_failed (z, c, c99);
    }

  /* cosh(+0 +i*0) = cosh(-0 -i*0) = 1 +i*0 */
  mpc_set_ui_ui (z, 0, 0, MPC_RNDNN);
  mpc_cosh (c, z, MPC_RNDNN);
  if (mpfr_cmp_si (MPC_RE (c), 1) != 0
      || !mpfr_zero_p (MPC_IM (c)) || mpfr_signbit (MPC_IM (c)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpc_set_ui_ui (c99, 1, 0, MPC_RNDNN);
      test_failed (z, c, c99);
    }
  mpc_neg (z, z, MPC_RNDNN);
  mpc_cosh (c, z, MPC_RNDNN);
  if (mpfr_cmp_si (MPC_RE (c), 1) != 0
      || !mpfr_zero_p (MPC_IM (c)) || mpfr_signbit (MPC_IM (c)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpc_set_ui_ui (c99, 1, 0, MPC_RNDNN);
      test_failed (z, c, c99);
    }

  /* cosh(-0 +i*0) = cosh(+0 -i*0) = 1 -i*0 */
  mpc_set_ui_ui (z, 0, 0, MPC_RNDNN);
  mpc_conj (z, z, MPC_RNDNN);
  mpc_cosh (c, z, MPC_RNDNN);
  if (mpfr_cmp_si (MPC_RE (c), 1) != 0
      || !mpfr_zero_p (MPC_IM (c)) || !mpfr_signbit (MPC_IM (c)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpc_set_ui_ui (c99, 1, 0, MPC_RNDNN);
      mpc_conj (c99, c99, MPC_RNDNN);
      test_failed (z, c, c99);
    }
  mpc_neg (z, z, MPC_RNDNN);
  mpc_cosh (c, z, MPC_RNDNN);
  if (mpfr_cmp_si (MPC_RE (c), 1) != 0
      || !mpfr_zero_p (MPC_IM (c)) || !mpfr_signbit (MPC_IM (c)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpc_set_ui_ui (c99, 1, 0, MPC_RNDNN);
      mpc_conj (c99, c99, MPC_RNDNN);
      test_failed (z, c, c99);
    }

  mpc_clear (c);
  mpc_clear (z);
}

static void
pure_real_argument (void)
{
  /* cosh(x -i*0) = cosh(x) +i*0 if x<0 */
  /* cosh(x -i*0) = cosh(x) -i*0 if x>0 */
  /* cosh(x +i*0) = cosh(x) -i*0 if x<0 */
  /* cosh(x -i*0) = cosh(x) +i*0 if x>0 */
  mpc_t u;
  mpc_t z;
  mpc_t cosh_z;

  mpc_init (u);
  mpc_init (z);
  mpc_init (cosh_z);

  /* cosh(1 +i*0) = cosh(1) +i*0 */
  mpc_set_ui_ui (z, 1, 0, MPC_RNDNN);
  mpfr_cosh (MPC_RE (u), MPC_RE (z), GMP_RNDN);
  mpfr_set_ui (MPC_IM (u), 0, GMP_RNDN);
  mpc_cosh (cosh_z, z, MPC_RNDNN);
  if (mpc_cmp (cosh_z, u) != 0 || mpfr_signbit (MPC_IM (cosh_z)))
    test_failed (z, cosh_z, u);

  /* cosh(1 -i*0) = cosh(1) -i*0 */
  mpc_conj (z, z, MPC_RNDNN);
  mpc_conj (u, u, MPC_RNDNN);
  mpc_cosh (cosh_z, z, MPC_RNDNN);
  if (mpc_cmp (cosh_z, u) != 0 || !mpfr_signbit (MPC_IM (cosh_z)))
    test_failed (z, cosh_z, u);

  /* cosh(-1 +i*0) = cosh(1) -i*0 */
  mpc_neg (z, z, MPC_RNDNN);
  mpc_cosh (cosh_z, z, MPC_RNDNN);
  if (mpc_cmp (cosh_z, u) != 0 || !mpfr_signbit (MPC_IM (cosh_z)))
    test_failed (z, cosh_z, u);

  /* cosh(-1 -i*0) = cosh(1) +i*0 */
  mpc_conj (z, z, MPC_RNDNN);
  mpc_conj (u, u, MPC_RNDNN);
  mpc_cosh (cosh_z, z, MPC_RNDNN);
  if (mpc_cmp (cosh_z, u) != 0 || mpfr_signbit (MPC_IM (cosh_z)))
    test_failed (z, cosh_z, u);

  mpc_clear (cosh_z);
  mpc_clear (z);
  mpc_clear (u);
}

static void
pure_imaginary_argument (void)
{
  /* cosh(+0 +i*y) = cos y +i*0*sin y */
  /* cosh(-0 +i*y) = cos y -i*0*sin y */
  mpc_t u;
  mpc_t z;
  mpc_t cosh_z;

  mpc_init (u);
  mpc_init (z);
  mpc_init (cosh_z);

  /* cosh(+0 +i) = cos(1) +i*0 */
  mpc_set_ui_ui (z, 0, 1, MPC_RNDNN);
  mpfr_cos (MPC_RE (u), MPC_IM (z), GMP_RNDN);
  mpfr_set_ui (MPC_IM (u), 0, GMP_RNDN);
  mpc_cosh (cosh_z, z, MPC_RNDNN);
  if (mpc_cmp (cosh_z, u) != 0 || mpfr_signbit (MPC_IM (cosh_z)))
    test_failed (z, cosh_z, u);

  /* cosh(+0 -i) = cos(1) -i*0 */
  mpc_conj (z, z, MPC_RNDNN);
  mpc_conj (u, u, MPC_RNDNN);
  mpc_cosh (cosh_z, z, MPC_RNDNN);
  if (mpc_cmp (cosh_z, u) != 0 || !mpfr_signbit (MPC_IM (cosh_z)))
    test_failed (z, cosh_z, u);

  /* cosh(-0 +i) = cos(1) -i*0 */
  mpc_neg (z, z, MPC_RNDNN);
  mpc_cosh (cosh_z, z, MPC_RNDNN);
  if (mpc_cmp (cosh_z, u) != 0 || !mpfr_signbit (MPC_IM (cosh_z)))
    test_failed (z, cosh_z, u);

  /* cosh(-0 -i) = cos(1) +i*0 */
  mpc_conj (z, z, MPC_RNDNN);
  mpc_conj (u, u, MPC_RNDNN);
  mpc_cosh (cosh_z, z, MPC_RNDNN);
  if (mpc_cmp (cosh_z, u) != 0 || mpfr_signbit (MPC_IM (cosh_z)))
    test_failed (z, cosh_z, u);

  mpc_clear (cosh_z);
  mpc_clear (z);
  mpc_clear (u);
}

static void
check_53 (void)
{
  mpc_t z;
  mpc_t cosh_z;
  mpc_t c;

  mpc_init2 (z, 53);
  mpc_init2 (cosh_z, 53);
  mpc_init2 (c, 53);

  /* cosh(z) is near (1+2^(-53)) *(0.5+i) */
  mpfr_set_str (MPC_RE (z), "1DA2E1BD2C9EBCp-53", 16, GMP_RNDN);
  mpfr_set_str (MPC_IM (z), "138AADEA15829Fp-52", 16, GMP_RNDN);
  mpfr_set_str (MPC_RE (c), "10000000000001p-53", 16, GMP_RNDN);
  mpfr_set_str (MPC_IM (c), "10000000000001p-52", 16, GMP_RNDN);
  mpc_cosh (cosh_z, z, MPC_RNDNN);
  if (mpc_cmp (cosh_z, c) != 0)
    test_failed (z, cosh_z, c);

  mpc_clear (c);
  mpc_clear (cosh_z);
  mpc_clear (z);
}

int
main (void)
{
  test_start ();

  data_check ("cosh.dat");
  special ();
  pure_real_argument ();
  pure_imaginary_argument ();

  tgeneric (2, 512, 7, 7);

  check_53 ();

  test_end ();

  return 0;
}
