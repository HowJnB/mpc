/* tsqrt -- test file for mpc_sqrt.

Copyright (C) 2008 Philippe Th\'eveny.

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
#include "mpfr.h"
#include "mpc.h"
#include "mpc-impl.h"

#include "random.c"

#define TEST_FUNCTION mpc_sqrt
#include "tgeneric.c"

static void
test_failed (mpc_t op, mpc_t get, mpc_t expected)
{
  printf ("mpc_sqrt(op) failed\n with ");
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
  mpc_t t;

  mpc_init (z);
  mpc_init (t);

  /* sqrt(+Inf +i*Inf) = +Inf +i*Inf */
  mpfr_set_inf (MPC_RE (z), +1);
  mpfr_set_inf (MPC_IM (z), +1);
  mpc_sqrt (t, z, MPC_RNDNN);
  if (!mpfr_inf_p (MPC_RE (t)) || mpfr_signbit (MPC_RE (t))
      || !mpfr_inf_p (MPC_IM (t)) || mpfr_signbit (MPC_IM (t)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_inf (MPC_RE(c99), +1);
      mpfr_set_inf (MPC_IM(c99), +1);
      test_failed (z, t, c99);
    }

  /* sqrt(+Inf -i*Inf) =  +Inf -i*Inf */
  mpc_conj (z, z, MPC_RNDNN);
  mpc_sqrt (t, z, MPC_RNDNN);
  if (!mpfr_inf_p (MPC_RE (t)) || mpfr_signbit (MPC_RE (t))
      || !mpfr_inf_p (MPC_IM (t)) || !mpfr_signbit (MPC_IM (t)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_inf (MPC_RE(c99), +1);
      mpfr_set_inf (MPC_IM(c99), -1);
      test_failed (z, t, c99);
    }

  /* sqrt(-Inf +i*Inf) = +Inf +i*Inf */
  mpc_neg (z, z, MPC_RNDNN);
  mpc_sqrt (t, z, MPC_RNDNN);
  if (!mpfr_inf_p (MPC_RE (t)) || mpfr_signbit (MPC_RE (t))
      || !mpfr_inf_p (MPC_IM (t)) || mpfr_signbit (MPC_IM (t)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_inf (MPC_RE(c99), +1);
      mpfr_set_inf (MPC_IM(c99), +1);
      test_failed (z, t, c99);
    }

  /* sqrt(-Inf -i*Inf) = +Inf -i*Inf */
  mpc_conj (z, z, MPC_RNDNN);
  mpc_sqrt (t, z, MPC_RNDNN);
  if (!mpfr_inf_p (MPC_RE (t)) || mpfr_signbit (MPC_RE (t))
      || !mpfr_inf_p (MPC_IM (t)) || !mpfr_signbit (MPC_IM (t)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_inf (MPC_RE(c99), +1);
      mpfr_set_inf (MPC_IM(c99), -1);
      test_failed (z, t, c99);
    }

  /* tan (NaN -i*Inf) = +Inf -i*Inf */
  mpfr_set_nan (MPC_RE (z));
  mpc_sqrt (t, z, MPC_RNDNN);
  if (!mpfr_inf_p (MPC_RE (t)) || mpfr_signbit (MPC_RE (t))
      || !mpfr_inf_p (MPC_IM (t)) || !mpfr_signbit (MPC_IM (t)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_inf (MPC_RE(c99), +1);
      mpfr_set_inf (MPC_IM(c99), -1);
      test_failed (z, t, c99);
    }

  /* tan (NaN +i*Inf) = +Inf +i*Inf */
  mpc_conj (z, z, MPC_RNDNN);
  mpc_sqrt (t, z, MPC_RNDNN);
  if (!mpfr_inf_p (MPC_RE (t)) || mpfr_signbit (MPC_RE (t))
      || !mpfr_inf_p (MPC_IM (t)) || mpfr_signbit (MPC_IM (t)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_inf (MPC_RE(c99), +1);
      mpfr_set_inf (MPC_IM(c99), +1);
      test_failed (z, t, c99);
    }

  /* sqrt(1 +i*Inf) = +Inf +i*Inf */
  mpfr_set_ui (MPC_RE (z), 1, GMP_RNDN);
  mpc_sqrt (t, z, MPC_RNDNN);
  if (!mpfr_inf_p (MPC_RE (t)) || mpfr_signbit (MPC_RE (t))
      || !mpfr_inf_p (MPC_IM (t)) || mpfr_signbit (MPC_IM (t)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_inf (MPC_RE(c99), +1);
      mpfr_set_inf (MPC_IM(c99), +1);
      test_failed (z, t, c99);
    }

  /* sqrt(1 -i*Inf) = +Inf -i*Inf */
  mpc_conj (z, z, MPC_RNDNN);
  mpc_sqrt (t, z, MPC_RNDNN);
  if (!mpfr_inf_p (MPC_RE (t)) || mpfr_signbit (MPC_RE (t))
      || !mpfr_inf_p (MPC_IM (t)) || !mpfr_signbit (MPC_IM (t)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_inf (MPC_RE(c99), +1);
      mpfr_set_inf (MPC_IM(c99), -1);
      test_failed (z, t, c99);
    }

  /* sqrt(-1 +i*Inf) = +Inf +i*Inf */
  mpc_neg (z, z, MPC_RNDNN);
  mpc_sqrt (t, z, MPC_RNDNN);
  if (!mpfr_inf_p (MPC_RE (t)) || mpfr_signbit (MPC_RE (t))
      || !mpfr_inf_p (MPC_IM (t)) || mpfr_signbit (MPC_IM (t)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_inf (MPC_RE(c99), +1);
      mpfr_set_inf (MPC_IM(c99), +1);
      test_failed (z, t, c99);
    }

  /* sqrt(-1 -i*Inf) = +Inf -i*Inf */
  mpc_conj (z, z, MPC_RNDNN);
  mpc_sqrt (t, z, MPC_RNDNN);
  if (!mpfr_inf_p (MPC_RE (t)) || mpfr_signbit (MPC_RE (t))
      || !mpfr_inf_p (MPC_IM (t)) || !mpfr_signbit (MPC_IM (t)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_inf (MPC_RE(c99), +1);
      mpfr_set_inf (MPC_IM(c99), -1);
      test_failed (z, t, c99);
    }

  /*  sqrt(+0 -i*Inf) = +Inf -i*Inf */
  mpfr_set_ui (MPC_RE (z), 0, GMP_RNDN);
  mpc_sqrt (t, z, MPC_RNDNN);
  if (!mpfr_inf_p (MPC_RE (t)) || mpfr_signbit (MPC_RE (t))
      || !mpfr_inf_p (MPC_IM (t)) || !mpfr_signbit (MPC_IM (t)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_inf (MPC_RE(c99), +1);
      mpfr_set_inf (MPC_IM(c99), -1);
      test_failed (z, t, c99);
    }

  /*  sqrt(+0 +i*Inf) = +Inf +i*Inf */
  mpc_conj (z, z, MPC_RNDNN);
  mpc_sqrt (t, z, MPC_RNDNN);
  if (!mpfr_inf_p (MPC_RE (t)) || mpfr_signbit (MPC_RE (t))
      || !mpfr_inf_p (MPC_IM (t)) || mpfr_signbit (MPC_IM (t)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_inf (MPC_RE(c99), +1);
      mpfr_set_inf (MPC_IM(c99), +1);
      test_failed (z, t, c99);
    }

  /*  sqrt(-0 -i*Inf) = +Inf -i*Inf */
  mpc_neg (z, z, MPC_RNDNN);
  mpc_sqrt (t, z, MPC_RNDNN);
  if (!mpfr_inf_p (MPC_RE (t)) || mpfr_signbit (MPC_RE (t))
      || !mpfr_inf_p (MPC_IM (t)) || !mpfr_signbit (MPC_IM (t)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_inf (MPC_RE(c99), +1);
      mpfr_set_inf (MPC_IM(c99), -1);
      test_failed (z, t, c99);
    }

  /*  sqrt(-0 +i*Inf) = +Inf +i*Inf */
  mpc_conj (z, z, MPC_RNDNN);
  mpc_sqrt (t, z, MPC_RNDNN);
  if (!mpfr_inf_p (MPC_RE (t)) || mpfr_signbit (MPC_RE (t))
      || !mpfr_inf_p (MPC_IM (t)) || mpfr_signbit (MPC_IM (t)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_inf (MPC_RE(c99), +1);
      mpfr_set_inf (MPC_IM(c99), +1);
      test_failed (z, t, c99);
    }

  /* sqrt(-0 +0*i) = +0 +0*i */
  mpfr_set_ui (MPC_IM (z), 0, GMP_RNDN);
  mpc_sqrt (t, z, MPC_RNDNN);
  if (!mpfr_zero_p (MPC_RE (t)) || mpfr_signbit (MPC_RE (t))
      || !mpfr_zero_p (MPC_IM (t)) || mpfr_signbit (MPC_IM (t)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpc_set_ui_ui (c99, 0, 0, MPC_RNDNN);
      test_failed (z, t, c99);
    }

  /* sqrt(-0 -0*i) = +0 -0*i */
  mpc_conj (z, z, MPC_RNDNN);
  mpc_sqrt (t, z, MPC_RNDNN);
  if (!mpfr_zero_p (MPC_RE (t)) || mpfr_signbit (MPC_RE (t))
      || !mpfr_zero_p (MPC_IM (t)) || !mpfr_signbit (MPC_IM (t)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpc_set_ui_ui (c99, 0, 0, MPC_RNDNN);
      mpc_conj (c99, c99, MPC_RNDNN);
      test_failed (z, t, c99);
    }

  /* sqrt(+0 +0*i) = +0 +0*i */
  mpc_neg (z, z, MPC_RNDNN);
  mpc_sqrt (t, z, MPC_RNDNN);
  if (!mpfr_zero_p (MPC_RE (t)) || mpfr_signbit (MPC_RE (t))
      || !mpfr_zero_p (MPC_IM (t)) || mpfr_signbit (MPC_IM (t)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpc_set_ui_ui (c99, 0, 0, MPC_RNDNN);
      test_failed (z, t, c99);
    }

  /* sqrt(+0 -0*i) = +0 -0*i */
  mpc_conj (z, z, MPC_RNDNN);
  mpc_sqrt (t, z, MPC_RNDNN);
  if (!mpfr_zero_p (MPC_RE (t)) || mpfr_signbit (MPC_RE (t))
      || !mpfr_zero_p (MPC_IM (t)) || !mpfr_signbit (MPC_IM (t)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpc_set_ui_ui (c99, 0, 0, MPC_RNDNN);
      mpc_conj (c99, c99, MPC_RNDNN);
      test_failed (z, t, c99);
    }

  /* sqrt(x +i*NaN) = NaN+i*NaN, when x is finite */
  mpfr_set_ui (MPC_RE (z), 1, GMP_RNDN);
  mpfr_set_nan (MPC_IM (z));
  mpc_sqrt (t, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (t)) || !mpfr_nan_p (MPC_IM (t)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_nan (MPC_RE (c99));
      mpfr_set_nan (MPC_IM (c99));
      test_failed (z, t, c99);
    }

  mpc_neg (z, z, MPC_RNDNN);
  mpc_sqrt (t, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (t)) || !mpfr_nan_p (MPC_IM (t)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_nan (MPC_RE (c99));
      mpfr_set_nan (MPC_IM (c99));
      test_failed (z, t, c99);
    }

  mpfr_set_ui (MPC_RE (z), 0, GMP_RNDN);
  mpc_sqrt (t, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (t)) || !mpfr_nan_p (MPC_IM (t)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_nan (MPC_RE (c99));
      mpfr_set_nan (MPC_IM (c99));
      test_failed (z, t, c99);
    }

  mpc_neg (z, z, MPC_RNDNN);
  mpc_sqrt (t, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (t)) || !mpfr_nan_p (MPC_IM (t)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_nan (MPC_RE (c99));
      mpfr_set_nan (MPC_IM (c99));
      test_failed (z, t, c99);
    }

  /* sqrt(NaN +i*y) = NaN+i*NaN, when y is finite */
  mpfr_set_nan (MPC_RE (z));
  mpfr_set_ui (MPC_IM (z), 1, GMP_RNDN);
  mpc_sqrt (t, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (t)) || !mpfr_nan_p (MPC_IM (t)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_nan (MPC_RE (c99));
      mpfr_set_nan (MPC_IM (c99));
      test_failed (z, t, c99);
    }

  mpc_conj (z, z, MPC_RNDNN);
  mpc_sqrt (t, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (t)) || !mpfr_nan_p (MPC_IM (t)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_nan (MPC_RE (c99));
      mpfr_set_nan (MPC_IM (c99));
      test_failed (z, t, c99);
    }

  mpfr_set_ui (MPC_IM (z), 0, GMP_RNDN);
  mpc_sqrt (t, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (t)) || !mpfr_nan_p (MPC_IM (t)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_nan (MPC_RE (c99));
      mpfr_set_nan (MPC_IM (c99));
      test_failed (z, t, c99);
    }

  mpc_conj (z, z, MPC_RNDNN);
  mpc_sqrt (t, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (t)) || !mpfr_nan_p (MPC_IM (t)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_nan (MPC_RE (c99));
      mpfr_set_nan (MPC_IM (c99));
      test_failed (z, t, c99);
    }

  /* sqrt(Nan +i*NaN) = NaN +i*NaN */
  mpfr_set_nan (MPC_IM (z));
  mpc_sqrt (t, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (t)) || !mpfr_nan_p (MPC_IM (t)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_nan (MPC_RE (c99));
      mpfr_set_nan (MPC_IM (c99));
      test_failed (z, t, c99);
    }

  /* sqrt(-Inf +i*NaN) = NaN +/-i*Inf */
  mpfr_set_inf (MPC_RE (z), -1);
  mpc_sqrt (t, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (t)) || !mpfr_inf_p (MPC_IM (t)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_nan (MPC_RE (c99));
      mpfr_set_inf (MPC_IM (c99), -1);
      test_failed (z, t, c99);
    }

  /* sqrt(+Inf +i*NaN) = +Inf +i*NaN */
  mpfr_set_inf (MPC_RE (z), +1);
  mpc_sqrt (t, z, MPC_RNDNN);
  if (!mpfr_inf_p (MPC_RE (t)) || !mpfr_nan_p (MPC_IM (t)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_inf (MPC_RE (c99), +1);
      mpfr_set_nan (MPC_IM (c99));
      test_failed (z, t, c99);
    }

  /* sqrt(+Inf +i*y) = +Inf +i*0, when y is a finite non negative number. */
  /* sqrt(+Inf +i*y) = +Inf -i*0, when y is a finite non positive number. */
  mpfr_set_ui (MPC_IM (z), 1, GMP_RNDN);
  mpc_sqrt (t, z, MPC_RNDNN);
  if (!mpfr_inf_p (MPC_RE (t)) || mpfr_signbit (MPC_RE (t))
      || !mpfr_zero_p (MPC_IM (t)) || mpfr_signbit (MPC_IM (t)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_inf (MPC_RE (c99), +1);
      mpfr_set_ui (MPC_IM (c99), 0, GMP_RNDN);
      test_failed (z, t, c99);
    }

  mpc_conj (z, z, MPC_RNDNN);
  mpc_sqrt (t, z, MPC_RNDNN);
  if (!mpfr_inf_p (MPC_RE (t)) || mpfr_signbit (MPC_RE (t))
      || !mpfr_zero_p (MPC_IM (t)) || !mpfr_signbit (MPC_IM (t)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_inf (MPC_RE (c99), +1);
      mpfr_set_ui (MPC_IM (c99), 0, GMP_RNDN);
      mpc_conj (c99, c99, MPC_RNDNN);
      test_failed (z, t, c99);
    }

  mpfr_set_ui (MPC_IM (z), 0, GMP_RNDN);
  mpc_sqrt (t, z, MPC_RNDNN);
  if (!mpfr_inf_p (MPC_RE (t)) || mpfr_signbit (MPC_RE (t))
      || !mpfr_zero_p (MPC_IM (t)) || mpfr_signbit (MPC_IM (t)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_inf (MPC_RE (c99), +1);
      mpfr_set_ui (MPC_IM (c99), 0, GMP_RNDN);
      test_failed (z, t, c99);
    }

  mpc_conj (z, z, MPC_RNDNN);
  mpc_sqrt (t, z, MPC_RNDNN);
  if (!mpfr_inf_p (MPC_RE (t)) || mpfr_signbit (MPC_RE (t))
      || !mpfr_zero_p (MPC_IM (t)) || !mpfr_signbit (MPC_IM (t)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_inf (MPC_RE (c99), +1);
      mpfr_set_ui (MPC_IM (c99), 0, GMP_RNDN);
      mpc_conj (c99, c99, MPC_RNDNN);
      test_failed (z, t, c99);
    }

  /* sqrt(-Inf +i*y) = +0 -i*Inf, when y is a finite non positive number. */
  /* sqrt(-Inf +i*y) = +0 +i*Inf, when y is a finite non negative number. */
  mpc_neg (z, z, MPC_RNDNN);
  mpc_sqrt (t, z, MPC_RNDNN);
  if (!mpfr_zero_p (MPC_RE (t)) || mpfr_signbit (MPC_RE (t))
      || !mpfr_inf_p (MPC_IM (t)) || mpfr_signbit (MPC_IM (t)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_ui (MPC_RE (c99), 0, GMP_RNDN);
      mpfr_set_inf (MPC_IM (c99), +1);
      test_failed (z, t, c99);
    }

  mpc_conj (z, z, MPC_RNDNN);
  mpc_sqrt (t, z, MPC_RNDNN);
  if (!mpfr_zero_p (MPC_RE (t)) || mpfr_signbit (MPC_RE (t))
      || !mpfr_inf_p (MPC_IM (t)) || !mpfr_signbit (MPC_IM (t)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_ui (MPC_RE (c99), 0, GMP_RNDN);
      mpfr_set_inf (MPC_IM (c99), -1);
      test_failed (z, t, c99);
    }

  mpfr_set_ui (MPC_IM (z), 1, GMP_RNDN);
  mpc_sqrt (t, z, MPC_RNDNN);
  if (!mpfr_zero_p (MPC_RE (t)) || mpfr_signbit (MPC_RE (t))
      || !mpfr_inf_p (MPC_IM (t)) || mpfr_signbit (MPC_IM (t)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_ui (MPC_RE (c99), 0, GMP_RNDN);
      mpfr_set_inf (MPC_IM (c99), +1);
      test_failed (z, t, c99);
    }

  mpc_conj (z, z, MPC_RNDNN);
  mpc_sqrt (t, z, MPC_RNDNN);
  if (!mpfr_zero_p (MPC_RE (t)) || mpfr_signbit (MPC_RE (t))
      || !mpfr_inf_p (MPC_IM (t)) || !mpfr_signbit (MPC_IM (t)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_ui (MPC_RE (c99), 0, GMP_RNDN);
      mpfr_set_inf (MPC_IM (c99), -1);
      test_failed (z, t, c99);
    }

  mpc_clear (t);
  mpc_clear (z);
}

static void
pure_real_argument ()
{
  /* sqrt(x +i*0) = sqrt(x) +i*0, when x>0 */
  /* sqrt(x -i*0) = sqrt(x) -i*0, when x>0 */
  /* sqrt(x +i*0) = +0 +i*sqrt(-x) +i*0, when x<0 */
  /* sqrt(x -i*0) = +0 -i*sqrt(-x) +i*0, when x<0 */
  mpfr_t x;
  mpfr_t sqrt_x;
  mpc_t z;
  mpc_t sqrt_z;

  mpfr_init (x);
  mpfr_init (sqrt_x);
  mpc_init (z);
  mpc_init (sqrt_z);

  /* sqrt(2 +i*0) = sqrt(2) +i*0 */
  mpc_set_ui_ui (z, 2, 0, MPC_RNDNN);
  mpfr_set_ui (x, 2, GMP_RNDN);
  mpfr_sqrt (sqrt_x, x, GMP_RNDN);
  mpc_sqrt (sqrt_z, z, MPC_RNDNN);
  if (mpfr_cmp (MPC_RE (sqrt_z), sqrt_x) != 0
      || !mpfr_zero_p (MPC_IM (sqrt_z)) || mpfr_signbit (MPC_IM (sqrt_z)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set (MPC_RE (c99), sqrt_x, GMP_RNDN);
      mpfr_set_ui (MPC_IM (c99), 0, GMP_RNDN);
      test_failed (z, sqrt_z, c99);
    }

  /* sqrt(2 -i*0) = sqrt(2) -i*0 */
  mpc_conj (z, z, MPC_RNDNN);
  mpc_sqrt (sqrt_z, z, MPC_RNDNN);
  if (mpfr_cmp (MPC_RE (sqrt_z), sqrt_x) != 0
      || !mpfr_zero_p (MPC_IM (sqrt_z)) || !mpfr_signbit (MPC_IM (sqrt_z)))
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set (MPC_RE (c99), sqrt_x, GMP_RNDN);
      mpfr_set_ui (MPC_IM (c99), 0, GMP_RNDN);
      mpc_conj (c99, c99, MPC_RNDNN);
      test_failed (z, sqrt_z, c99);
    }

  /* sqrt(-2 +i*0) = +0 +i*sqrt(2) */
  mpc_neg (z, z, MPC_RNDNN);
  mpc_sqrt (sqrt_z, z, MPC_RNDNN);
  if (!mpfr_zero_p (MPC_RE (sqrt_z)) || mpfr_signbit (MPC_RE (sqrt_z))
      || mpfr_cmp (MPC_IM (sqrt_z), sqrt_x) != 0)
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_ui (MPC_RE (c99), 0, GMP_RNDN);
      mpfr_set (MPC_IM (c99), sqrt_x, GMP_RNDN);
      test_failed (z, sqrt_z, c99);
    }

  /* sqrt(-2 -i*0) = +0 -i*sqrt(2) */
  mpfr_neg (sqrt_x, sqrt_x, GMP_RNDN);
  mpc_conj (z, z, MPC_RNDNN);
  mpc_sqrt (sqrt_z, z, MPC_RNDNN);
  if (!mpfr_zero_p (MPC_RE (sqrt_z)) || mpfr_signbit (MPC_RE (sqrt_z))
      || mpfr_cmp (MPC_IM (sqrt_z), sqrt_x) != 0)
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set_ui (MPC_RE (c99), 0, GMP_RNDN);
      mpfr_set (MPC_IM (c99), sqrt_x, GMP_RNDN);
      test_failed (z, sqrt_z, c99);
    }

  mpc_clear (sqrt_z);
  mpc_clear (z);
  mpfr_clear (sqrt_x);
  mpfr_clear (x);
}

static void
pure_imaginary_argument ()
{
  /* sqrt(+/-0 +i*y) = sqrt(y/2) * (1 +i), when y >0     */
  /* sqrt(+/-0 +i*y) = sqrt(-y/2) * (-1 + i), when y < 0 */
  /* here, b = sqrt(y/2) */
  mpfr_t b;
  mpfr_t minus_b;
  mpc_t z;
  mpc_t sqrt_z;

  mpfr_init (b);
  mpfr_init (minus_b);
  mpc_init (z);
  mpc_init (sqrt_z);

  /* sqrt(+0 +i*4) = sqrt(2) + i* sqrt(2) */
  mpfr_set_ui (b, 2, GMP_RNDN);
  mpfr_sqrt (b, b, GMP_RNDN);
  mpc_set_ui_ui (z, 0, 4, MPC_RNDNN);
  mpc_sqrt (sqrt_z, z, MPC_RNDNN);
  if (mpfr_cmp (MPC_RE (sqrt_z), b) != 0
      || mpfr_cmp (MPC_IM (sqrt_z), b) != 0)
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set (MPC_RE (c99), b, GMP_RNDN);
      mpfr_set (MPC_IM (c99), b, GMP_RNDN);
      test_failed (z, sqrt_z, c99);
    }

  /* sqrt(+0 -i*4) = -sqrt(2) + i* sqrt(2) */
  mpfr_neg (minus_b, b, GMP_RNDN);
  mpc_conj (z, z, MPC_RNDNN);
  mpc_sqrt (sqrt_z, z, MPC_RNDNN);
  if (mpfr_cmp (MPC_RE (sqrt_z), b) != 0
      || mpfr_cmp (MPC_IM (sqrt_z), minus_b) != 0)
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set (MPC_RE (c99), b, GMP_RNDN);
      mpfr_set (MPC_IM (c99), minus_b, GMP_RNDN);
      test_failed (z, sqrt_z, c99);
    }

  /* sqrt(-0 +i*4) = sqrt(2) + i* sqrt(2) */
  mpc_neg (z, z, MPC_RNDNN);
  mpc_sqrt (sqrt_z, z, MPC_RNDNN);
  if (mpfr_cmp (MPC_RE (sqrt_z), b) != 0
      || mpfr_cmp (MPC_IM (sqrt_z), b) != 0)
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set (MPC_RE (c99), b, GMP_RNDN);
      mpfr_set (MPC_IM (c99), b, GMP_RNDN);
      test_failed (z, sqrt_z, c99);
    }

  /* sqrt(-0 -i*4) = -sqrt(2) + i* sqrt(2) */
  mpc_conj (z, z, MPC_RNDNN);
  mpc_sqrt (sqrt_z, z, MPC_RNDNN);
  if (mpfr_cmp (MPC_RE (sqrt_z), b) != 0
      || mpfr_cmp (MPC_IM (sqrt_z), minus_b) != 0)
    {
      mpc_t c99;
      mpc_init (c99);
      mpfr_set (MPC_RE (c99), b, GMP_RNDN);
      mpfr_set (MPC_IM (c99), minus_b, GMP_RNDN);
      test_failed (z, sqrt_z, c99);
    }

  mpc_clear (sqrt_z);
  mpc_clear (z);
  mpfr_clear (minus_b);
  mpfr_clear (b);
}

static void
bugs_fixed ()
{
  mpc_t z;
  mpc_t sqrt_z;
  mpc_t expected;

  mpc_init (z);
  mpc_init (sqrt_z);
  mpc_init (expected);

  /* bug in v0.4.6 */
  mpc_set_prec (z, 19);
  mpc_set_prec (sqrt_z, 19);
  mpc_set_prec (expected, 19);
  mpfr_set_str (MPC_RE (z), "1.101010001010100000e117", 2, GMP_RNDN);
  mpfr_set_str (MPC_IM (z), "-1.001110111101100001e-158", 2, GMP_RNDN);
  mpfr_set_str (MPC_RE (expected), "1.110100100100100110e58", 2, GMP_RNDN);
  mpfr_set_str (MPC_IM (expected), "-1.010110101100111011e-218", 2, GMP_RNDN);
  mpc_sqrt (sqrt_z, z, MPC_RNDNZ);
  if (mpc_cmp (sqrt_z, expected))
    test_failed (z, sqrt_z, expected);

  mpc_set_prec (z, 2);
  mpc_set_prec (sqrt_z, 2);
  mpc_set_prec (expected, 2);
  mpfr_set_str (MPC_RE (z), "-0", 2, GMP_RNDN);
  mpfr_set_str (MPC_IM (z), "-1.1e204", 2, GMP_RNDN);
  mpfr_set_str (MPC_RE (expected), "1.1e101", 2, GMP_RNDN);
  mpfr_set_str (MPC_IM (expected), "-1.1e101", 2, GMP_RNDN);
  mpc_sqrt (sqrt_z, z, MPC_RNDNZ);
  if (mpc_cmp (sqrt_z, expected))
    test_failed (z, sqrt_z, expected);

  mpfr_set_str (MPC_RE (z), "-1.1e236", 2, GMP_RNDN);
  mpfr_set_str (MPC_IM (z), "-0", 2, GMP_RNDN);
  mpfr_set_str (MPC_RE (expected), "0", 2, GMP_RNDN);
  mpfr_set_str (MPC_IM (expected), "-1.0e118", 2, GMP_RNDN);
  mpc_sqrt (sqrt_z, z, MPC_RNDNZ);
  if (mpc_cmp (sqrt_z, expected))
    test_failed (z, sqrt_z, expected);

  mpc_clear (z);
  mpc_clear (sqrt_z);
  mpc_clear (expected);
}

int
main()
{
  test_start ();

  special ();
  pure_real_argument ();
  pure_imaginary_argument ();

  bugs_fixed ();

  tgeneric (2, 1024, 256);

  test_end ();

  return 0;
}
