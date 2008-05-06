/* test file for mpc_sinh.

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

#define TEST_FUNCTION mpc_sinh
#include "tgeneric.c"

static void
test_failed (mpc_t op, mpc_t get, mpc_t expected)
{
  printf ("mpc_sinh(op) failed\n with ");
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
  /* sinh(NaN +i*NaN) = NaN +i*NaN */
  mpfr_set_nan (MPC_IM (z));
  mpc_set (c99, z, MPC_RNDNN);
  mpc_sinh (s, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (s)) || !mpfr_nan_p (MPC_IM (s)))
    test_failed (z, s, z);

  /* sinh(NaN +i*Inf) = NaN +i*NaN */
  mpfr_set_inf (MPC_IM (z), +1);
  mpc_sinh (s, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (s)) || !mpfr_nan_p (MPC_IM (s)))
    test_failed (z, s, c99);

  /* sinh(NaN -i*Inf) = NaN +i*NaN */
  mpc_conj (z, z, MPC_RNDNN);
  mpc_sinh (s, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (s)) || !mpfr_nan_p (MPC_IM (s)))
    test_failed (z, s, c99);

  /* sinh(NaN +i*y) = NaN +i*NaN where 0<|y|<Inf */
  mpfr_set_ui (MPC_IM (z), 1, GMP_RNDN);
  mpc_sinh (s, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (s)) || !mpfr_nan_p (MPC_IM (s)))
    test_failed (z, s, c99);
  mpc_conj (z, z, MPC_RNDNN);
  mpc_sinh (s, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (s)) || !mpfr_nan_p (MPC_IM (s)))
    test_failed (z, s, c99);

  /* sinh(NaN +i*0) = NaN +i*0 */
  mpfr_set_ui (MPC_IM (z), 0, GMP_RNDN);
  mpc_sinh (s, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (s)) || !mpfr_zero_p (MPC_IM (s))
      || mpfr_signbit (MPC_IM(s)))
    test_failed (z, s, z);
  
  /* sinh(NaN -i*0) = NaN -i*0 */
  mpc_conj (z, z, MPC_RNDNN);
  mpc_sinh (s, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (s)) || !mpfr_zero_p (MPC_IM (s))
      || !mpfr_signbit (MPC_IM (s)))
    test_failed (z, s, z);

  /* sinh(x +i*NaN) = NaN +i*NaN where 0<|x|<Inf */
  mpfr_set_ui (MPC_RE (z), +1, MPC_RNDNN);
  mpfr_set_nan (MPC_IM (z));
  mpc_sinh (s, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (s)) || !mpfr_nan_p (MPC_IM (s)))
    test_failed (z, s, c99);
  mpc_neg (z, z, MPC_RNDNN);
  mpc_sinh (s, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (s)) || !mpfr_nan_p (MPC_IM (s)))
    test_failed (z, s, c99);

  /* sinh(+Inf +i*NaN) = +/-Inf +i*NaN */
  mpfr_set_inf (MPC_RE (z), +1);
  mpfr_set_nan (MPC_IM (z));
  mpc_sinh (s, z, MPC_RNDNN);
  if (!mpfr_inf_p (MPC_RE (s)) || mpfr_signbit (MPC_RE (s))
      || !mpfr_nan_p (MPC_IM (s)))
    test_failed (z, s, z);

  /* sinh(-Inf +i*NaN) = -/+Inf +i*NaN */
  mpc_neg (z, z, MPC_RNDNN);
  mpc_sinh (s, z, MPC_RNDNN);
  if (!mpfr_inf_p (MPC_RE (s)) || !mpfr_signbit (MPC_RE (s))
      || !mpfr_nan_p (MPC_IM (s)))
    test_failed (z, s, z);

  /* sinh(+0 +i*NaN) = +/-0 +i*NaN */
  mpfr_set_ui (MPC_RE (z), 0, GMP_RNDN);
  mpc_sinh (s, z, MPC_RNDNN);
  if (!mpfr_zero_p (MPC_RE (s)) || !mpfr_nan_p (MPC_IM (s)))
    test_failed (z, s, z);

  /* sinh(-0 +i*NaN) = -/+0 +i*NaN */
  mpc_neg (z, z, MPC_RNDNN);
  mpc_sinh (s, z, MPC_RNDNN);
  if (!mpfr_zero_p (MPC_RE (s)) || !mpfr_nan_p (MPC_IM (s)))
    test_failed (z, s, z);

  /* sinh(x +/-i*Inf) = NaN +i*NaN where 0<|x|<infinity*/
  mpfr_set_ui (MPC_RE (z), 1, GMP_RNDN);
  mpfr_set_inf (MPC_IM (z), +1);
  mpc_sinh (s, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (s)) || !mpfr_nan_p (MPC_IM (s)))
    test_failed (z, s, c99);
  mpc_conj (z, z, MPC_RNDNN);
  mpc_sinh (s, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (s)) || !mpfr_nan_p (MPC_IM (s)))
    test_failed (z, s, c99);
  mpc_neg (z, z, MPC_RNDNN);
  mpc_sinh (s, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (s)) || !mpfr_nan_p (MPC_IM (s)))
    test_failed (z, s, c99);
  mpc_conj (z, z, MPC_RNDNN);
  mpc_sinh (s, z, MPC_RNDNN);
  if (!mpfr_nan_p (MPC_RE (s)) || !mpfr_nan_p (MPC_IM (s)))
    test_failed (z, s, c99);

  /* sinh(+0 -i*Inf) = +/-0 +i*NaN */
  mpfr_set_ui (MPC_RE (z), 0, GMP_RNDN);
  mpfr_set_ui (MPC_RE (c99), 0, GMP_RNDN);
  mpfr_set_nan (MPC_IM (c99));
  mpc_sinh (s, z, MPC_RNDNN);
  if (!mpfr_zero_p (MPC_RE (s)) || !mpfr_nan_p (MPC_IM (s)))
    test_failed (z, s, c99);

  /* sinh(+0 +i*Inf) = +/-0 +i*NaN */
  mpc_conj (z, z, MPC_RNDNN);
  mpc_sinh (s, z, MPC_RNDNN);
  if (!mpfr_zero_p (MPC_RE (s)) || !mpfr_nan_p (MPC_IM (s)))
    test_failed (z, s, c99);

  /* sinh(-0 -i*Inf) = -/+0 -i*NaN */
  mpc_neg (z, z, MPC_RNDNN);
  mpc_neg (c99, c99, MPC_RNDNN);
  mpc_sinh (s, z, MPC_RNDNN);
  if (!mpfr_zero_p (MPC_RE (s)) || !mpfr_nan_p (MPC_IM (s)))
    test_failed (z, s, c99);

  /* sinh(-0 +i*Inf) = -/+0 +i*NaN */
  mpc_conj (z, z, MPC_RNDNN);
  mpc_sinh (s, z, MPC_RNDNN);
  if (!mpfr_zero_p (MPC_RE (s)) || !mpfr_nan_p (MPC_IM (s)))
    test_failed (z, s, c99);

  mpfr_set_inf (MPC_RE (z), -1);
  /* sinh(-Inf +i*Inf) = -/+Inf +i*NaN*/
  mpfr_set_inf (MPC_RE (c99), -1);
  mpfr_set_nan (MPC_IM (c99));
  mpc_sinh (s, z, MPC_RNDNN);
  if (!mpfr_inf_p (MPC_RE (s)) || !mpfr_nan_p (MPC_IM (s)))
    test_failed (z, s, c99);

  /* sinh(-Inf -i*Inf) =  -/+Inf +i*NaN */
  mpc_conj (z, z, MPC_RNDNN);
  mpc_sinh (s, z, MPC_RNDNN);
  if (!mpfr_inf_p (MPC_RE (s)) || !mpfr_nan_p (MPC_IM (s)))
    test_failed (z, s, c99);

  /* sinh(+Inf +i*Inf) = +/-Inf +i*NaN */
  mpc_neg (z, z, MPC_RNDNN);
  mpc_neg (c99, c99, MPC_RNDNN);
  mpc_sinh (s, z, MPC_RNDNN);
  if (!mpfr_inf_p (MPC_RE (s)) || !mpfr_nan_p (MPC_IM (s)))
    test_failed (z, s, c99);

  /* sinh(+Inf -i*Inf) = +/-Inf +i*NaN */
  mpc_conj (z, z, MPC_RNDNN);
  mpc_sinh (s, z, MPC_RNDNN);
  if (!mpfr_inf_p (MPC_RE (s)) || !mpfr_nan_p (MPC_IM (s)))
    test_failed (z, s, c99);

  /* sinh(-Inf +i*y) = -Inf*(cos y -i*sin y) when 0<|y|<Inf */
  /* sinh(+Inf +i*y) = +Inf*(cos y +i*sin y) when 0<|y|<Inf */
  mpfr_set_ui (MPC_IM (z), 1, GMP_RNDN);
  mpfr_set_inf (MPC_RE (c99), +1);
  mpfr_set_inf (MPC_IM (c99), +1);
  mpc_sinh (s, z, MPC_RNDUU);
  if (mpc_cmp (s, c99) != 0)
    test_failed (z, s, c99);
  mpc_conj (z, z, MPC_RNDNN);
  mpc_conj (c99, c99, MPC_RNDNN);
  mpc_sinh (s, z, MPC_RNDUU);
  if (mpc_cmp (s, c99) != 0)
    test_failed (z, s, c99);
  mpfr_neg (MPC_RE (z), MPC_RE (z), GMP_RNDN);
  mpfr_neg (MPC_RE (c99), MPC_RE (c99), GMP_RNDN);
  mpc_sinh (s, z, MPC_RNDUU);
  if (mpc_cmp (s, c99) != 0)
    test_failed (z, s, c99);
  mpc_conj (z, z, MPC_RNDNN);
  mpc_conj (c99, c99, MPC_RNDNN);
  mpc_sinh (s, z, MPC_RNDUU);
  if (mpc_cmp (s, c99) != 0)
    test_failed (z, s, c99);

  /* sinh(-Inf +i*0) = -Inf +i*0 */
  mpfr_set_ui (MPC_IM (z), 0, GMP_RNDN);
  mpc_sinh (s, z, MPC_RNDNN);
  if (mpc_cmp (s, z) != 0)
    test_failed (z, s, z);

  /* sinh(+Inf +i*0) = +Inf +i*0 */
  mpfr_neg (MPC_RE (z), MPC_RE (z), GMP_RNDN);
  mpc_sinh (s, z, MPC_RNDNN);
  if (mpc_cmp (s, z) != 0)
    test_failed (z, s, z);

  /* sinh(+Inf -i*0) = +Inf -i*0 */
  mpc_conj (z, z, MPC_RNDNN);
  mpc_sinh (s, z, MPC_RNDNN);
  if (mpc_cmp (s, z) != 0)
    test_failed (z, s, z);

  /* sinh(-Inf -i*0) = -Inf -i*0 */
  mpfr_neg (MPC_RE (z), MPC_RE (z), GMP_RNDN);
  mpc_sinh (s, z, MPC_RNDNN);
  if (mpc_cmp (s, z) != 0)
    test_failed (z, s, z);

  /* sinh(+0 +0*i) = +0 +0*i */
  mpc_set_ui_ui (z, 0, 0, MPC_RNDNN);
  mpc_sinh (s, z, MPC_RNDNN);
  if (mpc_cmp(s, z) != 0)
    test_failed (z, s, z);

  /* sinh(+0 -0*i) = +0 -0*i */
  mpc_conj (z, z, MPC_RNDNN);
  mpc_sinh (s, z, MPC_RNDNN);
  if (mpc_cmp(s, z) != 0)
    test_failed (z, s, z);

  /* sinh(-0 +0*i) = -0 +0*i */
  mpc_neg (z, z, MPC_RNDNN);
  mpc_sinh (s, z, MPC_RNDNN);
  if (mpc_cmp(s, z) != 0)
    test_failed (z, s, z);

  /* sinh(-0 -0*i) = -0 -0*i */
  mpc_conj (z, z, MPC_RNDNN);
  mpc_sinh (s, z, MPC_RNDNN);
  if (mpc_cmp(s, z) != 0)
    test_failed (z, s, z);

  mpc_clear (c99);
  mpc_clear (s);
  mpc_clear (z);
}

static void
pure_real_argument ()
{
  /* sinh(x -i*0) = sinh(x) -i*0 */
  /* sinh(x +i*0) = sinh(x) +i*0 */
  mpc_t u;
  mpc_t z;
  mpc_t sinh_z;

  mpc_init (u);
  mpc_init (z);
  mpc_init (sinh_z);

  /* sinh(1 +i*0) = sinh(1) +i*0 */
  mpc_set_ui_ui (z, 1, 0, MPC_RNDNN);
  mpfr_sinh (MPC_RE (u), MPC_RE (z), GMP_RNDN);
  mpfr_set_ui (MPC_IM (u), 0, GMP_RNDN);
  mpc_sinh (sinh_z, z, MPC_RNDNN);
  if (mpc_cmp (sinh_z, u) != 0)
    test_failed (z, sinh_z, u);

  /* sinh(-1 -i*0) = sinh(-1) -i*0 */
  mpc_neg (z, z, MPC_RNDNN);
  mpc_neg (u, u, MPC_RNDNN);
  mpc_sinh (sinh_z, z, MPC_RNDNN);
  if (mpc_cmp (sinh_z, u) != 0)
    test_failed (z, sinh_z, u);

  /* sinh(-1 +i*0) = sinh(-1) +i*0 */
  mpc_conj (z, z, MPC_RNDNN);
  mpc_conj (u, u, MPC_RNDNN);
  mpc_sinh (sinh_z, z, MPC_RNDNN);
  if (mpc_cmp (sinh_z, u) != 0)
    test_failed (z, sinh_z, u);

  /* sinh(1 -i*0) = sinh(1) -i*0 */
  mpc_neg (z, z, MPC_RNDNN);
  mpc_neg (u, u, MPC_RNDNN);
  mpc_sinh (sinh_z, z, MPC_RNDNN);
  if (mpc_cmp (sinh_z, u) != 0)
    test_failed (z, sinh_z, u);

  mpc_clear (sinh_z);
  mpc_clear (z);
  mpc_clear (u);
}

static void
pure_imaginary_argument ()
{
  /* sinh(-0 +i*y) = -0*cos(y) +i*sin(y) */
  /* sinh(+0 +i*y) = +0*cos(y) +i*sin(y) */
  mpc_t u;
  mpc_t z;
  mpc_t sinh_z;

  mpc_init (u);
  mpc_init (z);
  mpc_init (sinh_z);

  /* sinh(+0 +i) = +0 +i*sin(1) */
  mpc_set_ui_ui (z, 0, 1, MPC_RNDNN);
  mpfr_sin (MPC_IM (u), MPC_IM (z), GMP_RNDN);
  mpfr_set_ui (MPC_RE (u), 0, GMP_RNDN);
  mpc_sinh (sinh_z, z, MPC_RNDNN);
  if (mpc_cmp (sinh_z, u) != 0)
    test_failed (z, sinh_z, u);

  /* sinh(-0 -i) = -0 +i*sin(-1) */
  mpc_neg (z, z, MPC_RNDNN);
  mpc_neg (u, u, MPC_RNDNN);
  mpc_sinh (sinh_z, z, MPC_RNDNN);
  if (mpc_cmp (sinh_z, u) != 0)
    test_failed (z, sinh_z, u);

  /* sinh(-0 +i) = -0 +i*sin(1) */
  mpc_conj (z, z, MPC_RNDNN);
  mpc_conj (u, u, MPC_RNDNN);
  mpc_sinh (sinh_z, z, MPC_RNDNN);
  if (mpc_cmp (sinh_z, u) != 0)
    test_failed (z, sinh_z, u);

  /* sinh(+0 -i) = +0 +i*sin(-1) */
  mpc_neg (z, z, MPC_RNDNN);
  mpc_neg (u, u, MPC_RNDNN);
  mpc_sinh (sinh_z, z, MPC_RNDNN);
  if (mpc_cmp (sinh_z, u) != 0)
    test_failed (z, sinh_z, u);

  /* sinh(+0 +i*2) = +0 +i*sin(2) */
  mpc_set_ui_ui (z, 0, 2, MPC_RNDNN);
  mpfr_sin (MPC_IM (u), MPC_IM (z), GMP_RNDN);
  mpc_sinh (sinh_z, z, MPC_RNDNN);
  if (mpc_cmp (sinh_z, u) != 0)
    test_failed (z, sinh_z, u);

  /* sinh(-0 -i*2) = -0 +i*sin(-2) */
  mpc_neg (z, z, MPC_RNDNN);
  mpc_neg (u, u, MPC_RNDNN);
  mpc_sinh (sinh_z, z, MPC_RNDNN);
  if (mpc_cmp (sinh_z, u) != 0)
    test_failed (z, sinh_z, u);

  /* sinh(-0 +i*2) = -0 +i*sin(2) */
  mpc_conj (z, z, MPC_RNDNN);
  mpc_conj (u, u, MPC_RNDNN);
  mpc_sinh (sinh_z, z, MPC_RNDNN);
  if (mpc_cmp (sinh_z, u) != 0)
    test_failed (z, sinh_z, u);

  /* sinh(+0 -i*2) = +0 +i*sin(-2) */
  mpc_neg (z, z, MPC_RNDNN);
  mpc_neg (u, u, MPC_RNDNN);
  mpc_sinh (sinh_z, z, MPC_RNDNN);
  if (mpc_cmp (sinh_z, u) != 0)
    test_failed (z, sinh_z, u);

  mpc_clear (sinh_z);
  mpc_clear (z);
  mpc_clear (u);
}

static void
check_53()
{
  mpc_t z;
  mpc_t sinh_z;
  mpc_t s;

  mpc_init2 (z, 53);
  mpc_init2 (sinh_z, 53);
  mpc_init2 (s, 53);

  /* sin(z) is almost 2*i */
  mpfr_set_str (MPC_RE (z), "15124271980435p-52", 16, GMP_RNDN);
  mpfr_set_str (MPC_IM (z), "3243F6A8885A3p-49", 16, GMP_RNDN);
  mpfr_set_str (MPC_RE (s), "F48D4FDF29C53p-105", 16, GMP_RNDN);
  mpfr_set_str (MPC_IM (s), "2", 16, GMP_RNDN);
  mpc_sinh (sinh_z, z, MPC_RNDNN);
  if (mpc_cmp (sinh_z, s) != 0)
    test_failed (z, sinh_z, s);

  mpc_clear (s);
  mpc_clear (sinh_z);
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
