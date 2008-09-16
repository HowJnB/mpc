/* test file for mpc_cos.

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

#define TEST_FUNCTION mpc_cos
#include "tgeneric.c"

static void
test_failed (mpc_t op, mpc_t get, mpc_t expected)
{
  printf ("mpc_cos(op) failed\nwith  ");
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
  mpc_t c99;

  mpc_init (z);
  mpc_init (c);
  mpc_init (c99);

  mpfr_set_nan (MPC_RE (z));
  /* cos(NaN +i*NaN) = NaN +i*NaN */
  mpfr_set_nan (MPC_IM (z));
  mpc_cos (c, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (c)) || !mpfr_nan_p (MPC_IM (c)))
    test_failed (z, c, z);

  /* cos(NaN +i*Inf) = +Inf +i*NaN */
  mpfr_set_inf (MPC_IM (z), +1);
  mpfr_set_inf (MPC_RE (c99), +1);
  mpfr_set_nan (MPC_IM (c99));
  mpc_cos (c, z, MPC_RNDNN);
  if (!mpfr_inf_p (MPC_RE (c)) || mpfr_signbit (MPC_RE (c))
      || !mpfr_nan_p (MPC_IM (c)))
    test_failed (z, c, c99);

  /* cos(NaN -i*Inf) = +Inf +i*NaN */
  mpc_conj (z, z, MPC_RNDNN);
  mpc_cos (c, z, MPC_RNDNN);
  if (!mpfr_inf_p (MPC_RE (c)) || mpfr_signbit (MPC_RE (c))
      || !mpfr_nan_p (MPC_IM (c)))
    test_failed (z, c, z);

  /* cos(NaN +i*y) = NaN +i*NaN where 0<|y|<infinity */
  mpfr_set_ui (MPC_IM (z), 1, GMP_RNDN);
  mpfr_set_nan (MPC_RE (c99));
  mpc_cos (c, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (c)) || !mpfr_nan_p (MPC_IM (c)))
    test_failed (z, c, c99);
  mpc_conj (z, z, MPC_RNDNN);
  mpc_cos (c, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (c)) || !mpfr_nan_p (MPC_IM (c)))
    test_failed (z, c, c99);

  /* cos(NaN +i*0) = NaN +/-i*0 */
  mpfr_set_ui (MPC_IM (z), 0, GMP_RNDN);
  mpc_cos (c, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (c)) || !mpfr_zero_p (MPC_IM (c)))
    test_failed (z, c, z);

  /* cos(NaN -i*0) = NaN +/-i*0 */
  mpc_conj (z, z, MPC_RNDNN);
  mpc_cos (c, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (c)) || !mpfr_zero_p (MPC_IM (c)))
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
  if (!mpfr_nan_p (MPC_RE (c)) || !mpfr_nan_p (MPC_IM (c)))
    test_failed (z, c, c99);
  mpc_neg (z, z, MPC_RNDNN);
  mpc_cos (c, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (c)) || !mpfr_nan_p (MPC_IM (c)))
    test_failed (z, c, c99);
  mpfr_set_ui (MPC_RE (z), +1, MPC_RNDNN);
  mpc_cos (c, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (c)) || !mpfr_nan_p (MPC_IM (c)))
    test_failed (z, c, c99);
  mpc_neg (z, z, MPC_RNDNN);
  mpc_cos (c, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (c)) || !mpfr_nan_p (MPC_IM (c)))
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

  /* cos(+0 +i*Inf) = +Inf -i*0 */
  mpfr_set_ui (MPC_RE (z), 0, GMP_RNDN);
  mpfr_set_inf (MPC_RE (c99), +1);
  mpfr_set_ui (MPC_IM (c99), 0, GMP_RNDN);
  mpc_conj (c99, c99, MPC_RNDNN);
  mpc_cos (c, z, MPC_RNDNN);
  if (mpc_cmp (c, c99) != 0 || !mpfr_signbit (MPC_IM (c)))
    test_failed (z, c, c99);

  /* cos(+0 -i*Inf) = +Inf +i*0 */
  mpc_conj (z, z, MPC_RNDNN);
  mpc_conj (c99, c99, MPC_RNDNN);
  mpc_cos (c, z, MPC_RNDNN);
  if (mpc_cmp (c, c99) != 0 || mpfr_signbit (MPC_IM (c)))
    test_failed (z, c, c99);

  /* cos(-0 +i*Inf) = +Inf +i0 */
  mpc_neg (z, z, MPC_RNDNN);
  mpc_cos (c, z, MPC_RNDNN);
  if (mpc_cmp (c, c99) != 0 || mpfr_signbit (MPC_IM (c)))
    test_failed (z, c, c99);

  /* cos(-0 -i*Inf) = +Inf -i*0 */
  mpc_conj (z, z, MPC_RNDNN);
  mpc_conj (c99, c99, MPC_RNDNN);
  mpc_cos (c, z, MPC_RNDNN);
  if (mpc_cmp (c, c99) != 0 || !mpfr_signbit (MPC_IM (c)))
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

  /* cos(+0 +i*0) = 1 -i*0 */
  mpc_set_ui_ui (z, 0, 0, MPC_RNDNN);
  mpc_set_ui_ui (c99, 1, 0, MPC_RNDNN);
  mpc_conj (c99, c99, MPC_RNDNN);
  mpc_cos (c, z, MPC_RNDNN);
  if (mpc_cmp (c, c99) != 0 || !mpfr_signbit (MPC_IM (c)))
    test_failed (z, c, c99);

  /* cos(+0 -i*0) = 1 +i*0 */
  mpc_conj (z, z, MPC_RNDNN);
  mpc_conj (c99, c99, MPC_RNDNN);
  mpc_cos (c, z, MPC_RNDNN);
  if (mpc_cmp (c, c99) != 0 || mpfr_signbit (MPC_IM (c)))
    test_failed (z, c, c99);

  /* cos(-0 +i*0) = 1 +i*0 */
  mpc_neg (z, z, MPC_RNDNN);
  mpc_cos (c, z, MPC_RNDNN);
  if (mpc_cmp (c, c99) != 0 || mpfr_signbit (MPC_IM (c)))
    test_failed (z, c, c99);

  /* cos(-0 -i*0) = 1 -i*0 */
  mpc_conj (z, z, MPC_RNDNN);
  mpc_conj (c99, c99, MPC_RNDNN);
  mpc_cos (c, z, MPC_RNDNN);
  if (mpc_cmp (c, c99) != 0 || !mpfr_signbit (MPC_IM (c)))
    test_failed (z, c, c99);

  mpc_clear (c99);
  mpc_clear (c);
  mpc_clear (z);
}

static void
pure_real_argument (void)
{
  /* cos(x -i*0) = cos(x) +i*0*sin(x) */
  /* cos(x +i*0) = cos(x) -i*0*sin(x) */
  mpc_t u;
  mpc_t z;
  mpc_t cos_z;

  mpc_init (u);
  mpc_init (z);
  mpc_init (cos_z);

  /* cos(1 +i*0) = cos(1) -i*0*sin(1) */
  mpc_set_ui_ui (z, 1, 0, MPC_RNDNN);
  mpfr_cos (MPC_RE (u), MPC_RE (z), GMP_RNDN);
  mpfr_set_ui (MPC_IM (u), 0, GMP_RNDN);
  mpc_conj (u, u, MPC_RNDNN);
  mpc_cos (cos_z, z, MPC_RNDNN);
  if (mpc_cmp (cos_z, u) != 0 || !mpfr_signbit (MPC_IM (cos_z)))
    test_failed (z, cos_z, u);

  /* cos(1 -i*0) = cos(1) +i*0*sin(1) */
  mpc_conj (z, z, MPC_RNDNN);
  mpc_conj (u, u, MPC_RNDNN);
  mpc_cos (cos_z, z, MPC_RNDNN);
  if (mpc_cmp (cos_z, u) != 0 || mpfr_signbit (MPC_IM (cos_z)))
    test_failed (z, cos_z, u);

  /* cos(-1 +i*0) = cos(-1) -i*0*sin(-1) */
  mpc_neg (z, z, MPC_RNDNN);
  mpc_cos (cos_z, z, MPC_RNDNN);
  if (mpc_cmp (cos_z, u) != 0 || mpfr_signbit (MPC_IM (cos_z)))
    test_failed (z, cos_z, u);

  /* cos(-1 -i*0) = cos(-1) +i*0*sin(-1) */
  mpc_conj (z, z, MPC_RNDNN);
  mpc_conj (u, u, MPC_RNDNN);
  mpc_cos (cos_z, z, MPC_RNDNN);
  if (mpc_cmp (cos_z, u) != 0 || !mpfr_signbit (MPC_IM (cos_z)))
    test_failed (z, cos_z, u);

  /* cos(2 +i*0) = cos(2) -i*0*sin(2) */
  mpc_set_ui_ui (z, 2, 0, MPC_RNDNN);
  mpfr_cos (MPC_RE (u), MPC_RE (z), GMP_RNDN);
  mpc_cos (cos_z, z, MPC_RNDNN);
  if (mpc_cmp (cos_z, u) != 0 || !mpfr_signbit (MPC_IM (cos_z)))
    test_failed (z, cos_z, u);

  /* cos(2 -i*0) = cos(2) +i*0*sin(2) */
  mpc_conj (z, z, MPC_RNDNN);
  mpc_conj (u, u, MPC_RNDNN);
  mpc_cos (cos_z, z, MPC_RNDNN);
  if (mpc_cmp (cos_z, u) != 0 || mpfr_signbit (MPC_IM (cos_z)))
    test_failed (z, cos_z, u);

  /* cos(-2 +i*0) = cos(-2) -i*0*sin(-2) */
  mpc_neg (z, z, MPC_RNDNN);
  mpc_cos (cos_z, z, MPC_RNDNN);
  if (mpc_cmp (cos_z, u) != 0 || mpfr_signbit (MPC_IM (cos_z)))
    test_failed (z, cos_z, u);

  /* cos(-2 -i*0) = cos(-2) +i*0*sin(-2) */
  mpc_conj (z, z, MPC_RNDNN);
  mpc_conj (u, u, MPC_RNDNN);
  mpc_cos (cos_z, z, MPC_RNDNN);
  if (mpc_cmp (cos_z, u) != 0 || !mpfr_signbit (MPC_IM (cos_z)))
    test_failed (z, cos_z, u);

  mpc_clear (cos_z);
  mpc_clear (z);
  mpc_clear (u);
}

static void
pure_imaginary_argument (void)
{
  /* cos(-0 +i*y) = cosh(y) -i*0, when y < 0 */
  /* cos(-0 +i*y) = cosh(y) +i*0, when y > 0 */
  /* cos(+0 +i*y) = cosh(y) +i*0, when y < 0 */
  /* cos(+0 +i*y) = cosh(y) -i*0, when y > 0 */
  mpc_t u;
  mpc_t z;
  mpc_t cos_z;

  mpc_init (u);
  mpc_init (z);
  mpc_init (cos_z);

  /* cos(+0 -i) = cosh(-1) +i*0 */
  mpc_set_si_si (z, 0, -1, MPC_RNDNN);
  mpfr_cosh (MPC_RE (u), MPC_IM (z), GMP_RNDN);
  mpfr_set_ui (MPC_IM (u), 0, GMP_RNDN);
  mpc_cos (cos_z, z, MPC_RNDNN);
  if (mpc_cmp (cos_z, u) != 0 || mpfr_signbit (MPC_IM (cos_z)))
    test_failed (z, cos_z, u);

  /* cos(-0 +i) = cosh(1) +i*0 */
  mpc_neg (z, z, MPC_RNDNN);
  mpc_cos (cos_z, z, MPC_RNDNN);
  if (mpc_cmp (cos_z, u) != 0 || mpfr_signbit (MPC_IM (cos_z)))
    test_failed (z, cos_z, u);

  /* cos(-0 -i) = cosh(-1) -i*0 */
  mpc_conj (z, z, MPC_RNDNN);
  mpc_conj (u, u, MPC_RNDNN);
  mpc_cos (cos_z, z, MPC_RNDNN);
  if (mpc_cmp (cos_z, u) != 0 || !mpfr_signbit (MPC_IM (cos_z)))
    test_failed (z, cos_z, u);

  /* cos(+0 +i) = cosh(1) -i*0 */
  mpc_neg (z, z, MPC_RNDNN);
  mpc_cos (cos_z, z, MPC_RNDNN);
  if (mpc_cmp (cos_z, u) != 0 || !mpfr_signbit (MPC_IM (cos_z)))
    test_failed (z, cos_z, u);

  mpc_clear (cos_z);
  mpc_clear (z);
  mpc_clear (u);
}

static void
check_53 (void)
{
  mpc_t z;
  mpc_t cos_z;
  mpc_t c;

  mpc_init2 (z, 53);
  mpc_init2 (cos_z, 53);
  mpc_init2 (c, 53);

  /* cos(z) is 514 */
  mpfr_set_ui (MPC_RE (z), 0, GMP_RNDN);
  mpfr_set_str (MPC_IM (z), "1BBDD1808C59A3p-50", 16, GMP_RNDN);
  mpc_set_ui_ui (c, 514, 0, MPC_RNDNN);
  mpc_cos (cos_z, z, MPC_RNDNN);
  if (mpc_cmp (cos_z, c) != 0)
    test_failed (z, cos_z, c);

  mpc_clear (c);
  mpc_clear (cos_z);
  mpc_clear (z);
}

int
main (void)
{
  test_start ();

  special ();
  pure_real_argument ();
  pure_imaginary_argument ();

  tgeneric (2, 512, 7, 7);

  check_53 ();

  test_end ();

  return 0;
}
