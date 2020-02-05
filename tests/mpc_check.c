/* mpc_check -- compare mpc functions against the GNU libc implementation

Copyright (C) 2020 INRIA

This file is part of GNU MPC.

GNU MPC is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the
Free Software Foundation; either version 3 of the License, or (at your
option) any later version.

GNU MPC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for
more details.

You should have received a copy of the GNU Lesser General Public License
along with this program. If not, see http://www.gnu.org/licenses/ .
*/

/* the GNU libc provides the following functions (as of 2.31),
   with 'f' suffix for the float/binary32 version, with no suffix
   for the double/binary64 version, with 'l' suffix for the long double
   version, and with 'f128' suffix for the __float128 version:

   cabs     casinh    cexp      csinh
   cacos    catan     clog      csqrt
   cacosh   catanh    clog10    ctan
   carg     ccos      cpow      ctanh
   casin    ccosh     csin
*/

#define _GNU_SOURCE /* for clog10 */
#include <stdlib.h>
#include <string.h>
#include <complex.h>
#include "mpc-tests.h"
#ifdef __GNUC__
#include <gnu/libc-version.h>
#endif

gmp_randstate_t state;
unsigned long seed = 1;
int verbose = 0;

static unsigned long
ulp_error (mpfr_t x, mpfr_t y)
{
  mpfr_t z;
  mpfr_prec_t p = mpfr_get_prec (y);
  unsigned long n;

  if (mpfr_cmp (x, y) == 0)
    return 0;

  mpfr_init2 (z, p);
  mpfr_sub (z, x, y, MPFR_RNDN);
  mpfr_abs (z, z, MPFR_RNDN);
  /* divide by ulp(y) = 2^(EXP(y) - p) */
  mpfr_div_2si (z, z, mpfr_get_exp (y) - p, MPFR_RNDN);
  n = mpfr_get_ui (z, MPFR_RNDZ);
  mpfr_clear (z);
  return n;
}

#define FOO add
#define CFOO(x,y) (x+y)
#include "mpc_check_template3.c"

#define FOO sub
#define CFOO(x,y) (x-y)
#include "mpc_check_template3.c"

#define FOO mul
#define CFOO(x,y) (x*y)
#include "mpc_check_template3.c"

#define FOO div
#define CFOO(x,y) (x/y)
#include "mpc_check_template3.c"

#define FOO pow
#include "mpc_check_template3.c"

#define FOO abs
#include "mpc_check_template2.c"

#define FOO arg
#include "mpc_check_template2.c"

#define FOO sqrt
#include "mpc_check_template1.c"

#define FOO acos
#include "mpc_check_template1.c"

#define FOO acosh
#include "mpc_check_template1.c"

#define FOO asin
#include "mpc_check_template1.c"

#define FOO asinh
#include "mpc_check_template1.c"

#define FOO atan
#include "mpc_check_template1.c"

#define FOO atanh
#include "mpc_check_template1.c"

#define FOO cos
#include "mpc_check_template1.c"

#define FOO cosh
#include "mpc_check_template1.c"

#define FOO exp
#include "mpc_check_template1.c"

#define FOO log
#include "mpc_check_template1.c"

#define FOO log10
#include "mpc_check_template1.c"

#define FOO sin
#include "mpc_check_template1.c"

#define FOO sinh
#include "mpc_check_template1.c"

#define FOO tan
#include "mpc_check_template1.c"

#define FOO tanh
#include "mpc_check_template1.c"

int
main (int argc, char *argv[])
{
  mpfr_prec_t p = 53; /* default precision */
  unsigned long n = 1000000;

  while (argc >= 2 && argv[1][0] == '-')
    {
      if (argc >= 3 && strcmp (argv[1], "-p") == 0)
        {
          p = atoi (argv[2]);
          argc -= 2;
          argv += 2;
        }
      else if (argc >= 3 && strcmp (argv[1], "-seed") == 0)
        {
          seed = atoi (argv[2]);
          argc -= 2;
          argv += 2;
        }
      else if (strcmp (argv[1], "-v") == 0)
        {
          verbose ++;
          argc --;
          argv ++;
        }
      else
        {
          fprintf (stderr, "Unknown option %s\n", argv[1]);
          exit (1);
        }
    }

  MPC_ASSERT (p == 53);

  if (p == 53)
    {
      mpfr_set_emin (-1073);
      mpfr_set_emax (1024);
    }

  gmp_randinit_default (state);

#ifdef __GNUC__
  printf ("GNU libc version: %s\n", gnu_get_libc_version ());
  printf ("GNU libc release: %s\n", gnu_get_libc_release ());
#endif
  printf ("Using random seed %lu\n", seed);

  /* (complex,complex) -> complex */
  test_add (p, n);
  test_sub (p, n);
  test_mul (p, n);
  test_div (p, n);
  test_pow (p, n);

  /* complex -> real */
  test_abs (p, n);
  test_arg (p, n);

  /* complex -> complex */
  test_sqrt (p, n);
  test_acos (p, n);
  test_acosh (p, n);
  test_asin (p, n);
  test_asinh (p, n);
  test_atan (p, n);
  test_atanh (p, n);
  test_cos (p, n);
  test_cosh (p, n);
  test_exp (p, n);
  test_log (p, n);
  test_log10 (p, n);
  test_sin (p, n);
  test_sinh (p, n);
  test_tan (p, n);
  test_tanh (p, n);

  gmp_randclear (state);

  return 0;
}
