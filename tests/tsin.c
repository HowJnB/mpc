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
  if (!mpfr_nan_p (MPC_RE (s)) || !mpfr_nan_p (MPC_IM (s)))
    test_failed (z, s, z);

  /* sin(NaN +i*infinity) = NaN +/-i*infinity */
  mpfr_set_inf (MPC_IM (z), +1);
  mpc_sin (s, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (s)) || !mpfr_inf_p (MPC_IM (s)))
    test_failed (z, s, z);

  /* sin(NaN -i*infinity) = NaN -/+i*infinity */
  mpc_conj (z, z, MPC_RNDNN);
  mpc_sin (s, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (s)) || !mpfr_inf_p (MPC_IM (s)))
    test_failed (z, s, z);

  /* sin(NaN +i*y) = NaN +i*NaN where 0<|y|<infinity */
  mpfr_set_ui (MPC_IM (z), 1, GMP_RNDN);
  mpfr_set_nan (MPC_RE (c99));
  mpfr_set_nan (MPC_IM (c99));
  mpc_sin (s, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (s)) || !mpfr_nan_p (MPC_IM (s)))
    test_failed (z, s, c99);
  mpc_conj (z, z, MPC_RNDNN);
  mpc_sin (s, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (s)) || !mpfr_nan_p (MPC_IM (s)))
    test_failed (z, s, c99);

  /* sin(NaN +i*0) = NaN +/-i*0 */
  /* sin(NaN -i*0) = NaN -/+i*0 */
  mpfr_set_ui (MPC_IM (z), 0, GMP_RNDN);
  mpc_sin (s, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (s)) || !mpfr_zero_p (MPC_IM (s)))
    test_failed (z, s, z);
  mpc_conj (z, z, MPC_RNDNN);
  mpc_sin (s, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (s)) || !mpfr_zero_p (MPC_IM (s)))
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
  if (!mpfr_nan_p (MPC_RE (s)) || !mpfr_nan_p (MPC_IM (s)))
    test_failed (z, s, c99);
  mpc_neg (z, z, MPC_RNDNN);
  mpc_sin (s, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (s)) || !mpfr_nan_p (MPC_IM (s)))
    test_failed (z, s, c99);
  mpfr_set_ui (MPC_RE (z), +1, MPC_RNDNN);
  mpc_sin (s, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (s)) || !mpfr_nan_p (MPC_IM (s)))
    test_failed (z, s, c99);
  mpc_neg (z, z, MPC_RNDNN);
  mpc_sin (s, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (s)) || !mpfr_nan_p (MPC_IM (s)))
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
  if (mpc_cmp (s, z) != 0 || mpfr_signbit (MPC_RE (s)))
    test_failed (z, s, z);

  /* sin(+0 +i*infinity) = +0 +i*infinity */
  mpc_conj (z, z, MPC_RNDNN);
  mpc_sin (s, z, MPC_RNDNN);
  if (mpc_cmp (s, z) != 0 || mpfr_signbit (MPC_RE (s)))
    test_failed (z, s, z);

  /* sin(-0 -i*infinity) = -0 -i*infinity */
  mpc_neg (z, z, MPC_RNDNN);
  mpc_sin (s, z, MPC_RNDNN);
  if (mpc_cmp (s, z) != 0 || !mpfr_signbit (MPC_RE (s)))
    test_failed (z, s, z);

  /* sin(-0 +i*infinity) = -0 +i*infinity */
  mpc_conj (z, z, MPC_RNDNN);
  mpc_sin (s, z, MPC_RNDNN);
  if (mpc_cmp (s, z) != 0 || !mpfr_signbit (MPC_RE (s)))
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

  /* sin(+0 +0*i) = +0 +0*i */
  mpc_set_ui_ui (z, 0, 0, MPC_RNDNN);
  mpc_sin (s, z, MPC_RNDNN);
  if (mpc_cmp(s, z) != 0)
    test_failed (z, s, z);

  /* sin(+0 -0*i) = +0 -0*i */
  mpc_conj (z, z, MPC_RNDNN);
  mpc_sin (s, z, MPC_RNDNN);
  if (mpc_cmp(s, z) != 0)
    test_failed (z, s, z);

  /* sin(-0 +0*i) = -0 +0*i */
  mpc_neg (z, z, MPC_RNDNN);
  mpc_sin (s, z, MPC_RNDNN);
  if (mpc_cmp(s, z) != 0)
    test_failed (z, s, z);

  /* sin(-0 -0*i) = -0 -0*i */
  mpc_conj (z, z, MPC_RNDNN);
  mpc_sin (s, z, MPC_RNDNN);
  if (mpc_cmp(s, z) != 0)
    test_failed (z, s, z);

  mpc_clear (c99);
  mpc_clear (s);
  mpc_clear (z);
}

static void
pure_real_argument ()
{
  /* sin(x -i*0) = sin(x) -i*0*cos(x) */
  /* sin(x +i*0) = sin(x) +i*0*cos(x) */
  mpfr_t x;
  mpfr_t sin_x;
  mpc_t z;
  mpc_t sin_z;

  mpfr_init (x);
  mpfr_init (sin_x);
  mpc_init (z);
  mpc_init (sin_z);

  /* sin(1 +i*0) = sin(1) +i*0*cos(1) */
  mpc_set_ui_ui (z, 1, 0, MPC_RNDNN);
  mpfr_set_ui (x, 1, GMP_RNDN);
  mpfr_sin (sin_x, x, GMP_RNDN);
  mpc_sin (sin_z, z, MPC_RNDNN);
  if (mpfr_cmp (MPC_RE (sin_z), sin_x) != 0
      || !mpfr_zero_p (MPC_IM (sin_z)) || mpfr_signbit (MPC_IM (sin_z)))
    {
      printf ("mpc_sin(1 + i * 0) failed\n");
      exit (1);
    }

  /* sin(-1 -i*0) = sin(-1) -i*0*cos(1) */
  mpc_neg (z, z, MPC_RNDNN);
  mpfr_neg (sin_x, sin_x, GMP_RNDN);
  mpc_sin (sin_z, z, MPC_RNDNN);
  if (mpfr_cmp (MPC_RE (sin_z), sin_x) != 0
      || !mpfr_zero_p (MPC_IM (sin_z)) || !mpfr_signbit (MPC_IM (sin_z)))
    {
      printf ("mpc_sin(-1 - i * 0) failed\n");
      exit (1);
    }

  /* sin(-1 +i*0) = sin(-1) +i*0*cos(1) */
  mpc_conj (z, z, MPC_RNDNN);
  mpc_sin (sin_z, z, MPC_RNDNN);
  if (mpfr_cmp (MPC_RE (sin_z), sin_x) != 0
      || !mpfr_zero_p (MPC_IM (sin_z)) || mpfr_signbit (MPC_IM (sin_z)))
    {
      printf ("mpc_sin(-1 + i * 0) failed\n");
      exit (1);
    }

  /* sin(1 -i*0) = sin(1) -i*0*cos(1) */
  mpc_neg (z, z, MPC_RNDNN);
  mpfr_neg (sin_x, sin_x, GMP_RNDN);
  mpc_sin (sin_z, z, MPC_RNDNN);
  if (mpfr_cmp (MPC_RE (sin_z), sin_x) != 0
      || !mpfr_zero_p (MPC_IM (sin_z)) || !mpfr_signbit (MPC_IM (sin_z)))
    {
      printf ("mpc_sin(1 - i * 0) failed\n");
      exit (1);
    }

  /* sin(2 +i*0) = sin(2) +i*0*cos(2) */
  mpc_set_ui_ui (z, 2, 0, MPC_RNDNN);
  mpfr_set_ui (x, 2, GMP_RNDN);
  mpfr_sin (sin_x, x, GMP_RNDN);
  mpc_sin (sin_z, z, MPC_RNDNN);
  if (mpfr_cmp (MPC_RE (sin_z), sin_x) != 0
      || !mpfr_zero_p (MPC_IM (sin_z)) || !mpfr_signbit (MPC_IM (sin_z)))
    {
      printf ("mpc_sin(2 + i * 0) failed\n");
      exit (1);
    }

  /* sin(-2 -i*0) = sin(-2) -i*0*cos(-2) */
  mpc_neg (z, z, MPC_RNDNN);
  mpfr_neg (sin_x, sin_x, GMP_RNDN);
  mpc_sin (sin_z, z, MPC_RNDNN);
  if (mpfr_cmp (MPC_RE (sin_z), sin_x) != 0
      || !mpfr_zero_p (MPC_IM (sin_z)) || mpfr_signbit (MPC_IM (sin_z)))
    {
      printf ("mpc_sin(1 - i * 0) failed\n");
      exit (1);
    }

  /* sin(-2 +i*0) = sin(-2) +i*0*cos(-2) */
  mpc_conj (z, z, MPC_RNDNN);
  mpc_sin (sin_z, z, MPC_RNDNN);
  if (mpfr_cmp (MPC_RE (sin_z), sin_x) != 0
      || !mpfr_zero_p (MPC_IM (sin_z)) || !mpfr_signbit (MPC_IM (sin_z)))
    {
      printf ("mpc_sin(-2 + i * 0) failed\n");
      exit (1);
    }

  /* sin(2 -i*0) = sin(2) -i*0*cos(2) */
  mpc_neg (z, z, MPC_RNDNN);
  mpfr_neg (sin_x, sin_x, GMP_RNDN);
  mpc_sin (sin_z, z, MPC_RNDNN);
  if (mpfr_cmp (MPC_RE (sin_z), sin_x) != 0
      || !mpfr_zero_p (MPC_IM (sin_z)) || mpfr_signbit (MPC_IM (sin_z)))
    {
      printf ("mpc_sin(2 - i * 0) failed\n");
      exit (1);
    }

  mpc_clear (sin_z);
  mpc_clear (z);
  mpfr_clear (sin_x);
  mpfr_clear (x);
}

static void
pure_imaginary_argument ()
{
  /* sin(-0 +i*y) = -0 +i*sinh(y) */
  /* sin(+0 +i*y) = +0 +i*sinh(y) */
  mpfr_t y;
  mpfr_t sinh_y;
  mpc_t z;
  mpc_t sin_z;

  mpfr_init (y);
  mpfr_init (sinh_y);
  mpc_init (z);
  mpc_init (sin_z);

  /* sin(+0 +i) = +0 +i*sinh(1) */
  mpc_set_ui_ui (z, 0, 1, MPC_RNDNN);
  mpfr_set_ui (y, 1, GMP_RNDN);
  mpfr_sinh (sinh_y, y, GMP_RNDN);
  mpc_sin (sin_z, z, MPC_RNDNN);
  if (mpfr_cmp (MPC_IM (sin_z), sinh_y) != 0
      || !mpfr_zero_p (MPC_RE (sin_z)) || mpfr_signbit (MPC_RE (sin_z)))
    {
      printf ("mpc_sin(+0 + i) failed\n");
      exit (1);
    }

  /* sin(+0 -i) = +0 +i*sinh(-1) */
  mpc_conj (z, z, MPC_RNDNN);
  mpfr_neg (sinh_y, sinh_y, GMP_RNDN);
  mpc_sin (sin_z, z, MPC_RNDNN);
  if (mpfr_cmp (MPC_IM (sin_z), sinh_y) != 0
      || !mpfr_zero_p (MPC_RE (sin_z)) || mpfr_signbit (MPC_RE (sin_z)))
    {
      printf ("mpc_sin(+0 - i) failed\n");
      exit (1);
    }

  /* sin(-0 +i) = -0 +i*sinh(1) */
  mpc_neg (z, z, MPC_RNDNN);
  mpfr_neg (sinh_y, sinh_y, GMP_RNDN);
  mpc_sin (sin_z, z, MPC_RNDNN);
  if (mpfr_cmp (MPC_IM (sin_z), sinh_y) != 0
      || !mpfr_zero_p (MPC_RE (sin_z)) || !mpfr_signbit (MPC_RE (sin_z)))
    {
      printf ("mpc_sin(-0 + i) failed\n");
      exit (1);
    }

  /* sin(-0 -i) = -0 -i*sinh(1) */
  mpc_conj (z, z, MPC_RNDNN);
  mpfr_neg (sinh_y, sinh_y, GMP_RNDN);
  mpc_sin (sin_z, z, MPC_RNDNN);
  if (mpfr_cmp (MPC_IM (sin_z), sinh_y) != 0
      || !mpfr_zero_p (MPC_RE (sin_z)) || !mpfr_signbit (MPC_RE (sin_z)))
    {
      printf ("mpc_sin(-0 - i) failed\n");
      exit (1);
    }

  mpc_clear (sin_z);
  mpc_clear (z);
  mpfr_clear (sinh_y);
  mpfr_clear (y);
}

static void
check_53()
{
  mpc_t z;
  mpc_t sin_z;
  mpc_t s;

  mpc_init2 (z, 53);
  mpc_init2 (sin_z, 53);
  mpc_init2 (s, 53);

  /* sin(z) is almost 514 */
  mpfr_set_str (MPC_RE (z), "3243F6A8885A3p-49", 16, GMP_RNDN);
  mpfr_set_str (MPC_IM (z), "-1BBDD1808C59A3p-50", 16, GMP_RNDN);
  mpfr_set_str (MPC_RE (s), "514", 10, GMP_RNDN);
  mpfr_set_str (MPC_IM (s), "-11B7CA26B51951p-97", 16, GMP_RNDN);
  mpc_sin (sin_z, z, MPC_RNDNN);
  if (mpc_cmp (sin_z, s) != 0)
    test_failed (z, sin_z, s);

  mpc_clear (s);
  mpc_clear (sin_z);
  mpc_clear (z);
}

int
main()
{
  special ();
  pure_real_argument ();
  pure_imaginary_argument ();

  tgeneric ();

  check_53 ();

  return 0;
}
