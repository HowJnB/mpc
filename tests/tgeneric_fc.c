/* File for generic tests associated with real function of one complex
   variable.

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

#define FUNCTION_NAME(F) NAME_STR(F)
#define NAME_STR(F) #F

#define MPFR_OUT(X) \
  printf (#X" [%ld]=", MPFR_PREC (X));\
  mpfr_out_str (stdout, 2, 0, (X), GMP_RNDN);\
  printf ("\n");

/* tgeneric usage:
 1. use tgeneric_fc.c with function whose prototype is
 (mpfr_t rop, mpc_t op, mpfr_rnd_t rnd)
 2. Copy the following lines in front of your test file:

 #include <stdio.h>
 #include <stdlib.h>
 #include "gmp.h"
 #include "mpfr.h"
 #include "mpc.h"

 #include "random.c"
 #define TEST_FUNCTION mpc_function
 #define REFERENCE_FUNCTION optional_reference_function
 #include "tgeneric_fc.c"

 3. #define TEST_FUNCTION as the function under test before including
 tgeneric_fc.c in your test file.
 If you want to check against a reference implementation,
 #define REFERENCE_FUNCTION as it function name.
 
 tgeneric(prec_min, prec_max, step, exp_max) checks rounding with random
 numbers:
 - with precision ranging from prec_min to prec_max with an increment of
   step,
 - with exponent between -prec_max and prec_max.

 It also checks parameters reuse (it is assumed here that either two mpc_t
 variables are equal or they are different, in the sense that the real part of
 one of them cannot be the imaginary part of the other). */

static void
rounding_failed (mpc_srcptr op, mpfr_srcptr rop,
                 mpfr_srcptr rop4, mpfr_srcptr rop4rnd, mpfr_rnd_t rnd)
{
  printf ("Rounding in " FUNCTION_NAME(TEST_FUNCTION) " might be "
	  "incorrect for\n");
  OUT (op);
  printf ("with rounding mode %s", mpfr_print_rnd_mode (rnd));

  printf ("\n" FUNCTION_NAME(TEST_FUNCTION) "                     gives ");
  MPFR_OUT (rop);
  printf (FUNCTION_NAME(TEST_FUNCTION) " quadruple precision gives ");
  MPFR_OUT (rop4);
  printf ("and is rounded to                  ");
  MPFR_OUT (rop4rnd);

  exit (1);
}

static void
tgeneric(mpfr_prec_t prec_min, mpfr_prec_t prec_max, mpfr_prec_t step, mp_exp_t exp_max)
{
  mpc_t  z, t;
  mpfr_t v, w, x;
  mpfr_rnd_t rnd;
  mpfr_prec_t prec;
  mp_exp_t exp_min;

  mpc_init (z);
  mpc_init (t);
  mpfr_init (v);
  mpfr_init (w);
  mpfr_init (x);

  exp_min = mpfr_get_emin ();
  if (exp_max <= 0)
    exp_max = mpfr_get_emax ();
  else if (exp_max > mpfr_get_emax ())
    exp_max = mpfr_get_emax();
  if (-exp_max > exp_min)
    exp_min = - exp_max;

  if (prec_min < 2)
    prec_min = 2;

  for (prec = prec_min; prec <= prec_max; prec+=step)
    {
      mpc_set_prec (z, prec);
      mpc_set_prec (t, prec);
      mpfr_set_prec (w, 4*prec);
      mpfr_set_prec (v, prec);
      mpfr_set_prec (x, prec);

      test_default_random (z, exp_min, exp_max, 1);
      mpfr_set (x, MPC_RE (z), GMP_RNDN);
      test_default_random (z, exp_min, exp_max, 1);
      mpc_set (t, z, MPC_RNDNN); /* exact */

      for (rnd = 0; rnd < 4; rnd ++)
        {
          /* We compute the result with four times the precision and      */
          /* check whether the rounding is correct. Error reports in this */
          /* part of the algorithm might still be wrong, though, since    */
          /* there are two consecutive roundings (but we try to avoid     */
          /* them).                                                       */

          TEST_FUNCTION (x, z, rnd);
          TEST_FUNCTION (w, z, rnd);

          /* can't use mpfr_can_round when argument is singular */
          /* and avoid double rounding error */
          if (mpfr_zero_p (w) || mpfr_inf_p (w)
              || mpfr_can_round (w, 4 * prec - 1, rnd, rnd, prec))
            {
              mpfr_set (v, w, rnd);
              if (mpfr_cmp (x, v) != 0)
                rounding_failed (z, x, w, v, rnd);
            }

#ifdef REFERENCE_FUNCTION
          /* check against reference implementation */
          REFERENCE_FUNCTION (v, z, rnd);
          if (mpfr_cmp (x, v) != 0)
            {
              mpfr_t ref;
              mpfr_t got;

              /* aliases for display */
              ref[0] = v[0];
              got[0] = x[0];

              printf ("%s(z) and %s(z) differ for rnd=%s\n",
                      FUNCTION_NAME(TEST_FUNCTION),
                      FUNCTION_NAME(REFERENCE_FUNCTION),
                      mpfr_print_rnd_mode (rnd));
              OUT (z);
              MPFR_OUT (ref);
              MPFR_OUT (got);

              exit (1);
            }
#endif /* REFERENCE_FUNCTION */
        }

      /* Test parameter reuse: the function should not use its output
         parameter in internal computations. */
      TEST_FUNCTION (x, z, GMP_RNDN);
      TEST_FUNCTION (MPC_RE (z), z, GMP_RNDN);
      if (mpfr_cmp (x, MPC_RE (z)) != 0)
        {
          mpfr_t expected;
          mpfr_t got;

          /* aliases for display */
          expected[0] = x[0];
          got[0] = MPC_RE (z)[0];

          printf ("Error for %s(MPC_RE (t), t) with ",
                  FUNCTION_NAME(TEST_FUNCTION));
          OUT (t);
          MPFR_OUT (expected);
          MPFR_OUT (got);

          exit (1);
        }

      mpc_set (z, t, MPC_RNDNN); /* exact */
      TEST_FUNCTION (MPC_IM (z), z, GMP_RNDN);
      if (mpfr_cmp (x, MPC_IM (z)) != 0)
        {
          mpfr_t expected;
          mpfr_t got;

          /* aliases for display */
          expected[0] = x[0];
          got[0] = MPC_IM (z)[0];

          printf ("Error for %s(MPC_IM (t), t) with ",
                  FUNCTION_NAME(TEST_FUNCTION));
          OUT (t);
          MPFR_OUT (expected);
          MPFR_OUT (got);

          exit (1);
        }
    }

  mpfr_clear (v);
  mpfr_clear (w);
  mpfr_clear (x);
  mpc_clear (z);
  mpc_clear (t);
}
