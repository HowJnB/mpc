/* File for generic tests associated with complex function of two complex
   variables.

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

/* Warning: unlike the MPFR macro (defined in mpfr-impl.h), this one returns
   true when b is singular */
#define MPFR_CAN_ROUND(b,err,prec,rnd)                                  \
  (mpfr_zero_p (b) || mpfr_inf_p (b)                                    \
   || mpfr_can_round (b, mpfr_get_prec (b) - (err), (rnd), (rnd),       \
                      (prec) + ((rnd)==GMP_RNDN)))

/* tgeneric usage:
 1. use tgeneric_ccc.c with function whose prototype is
 (mpc_t rop, mpc_t op1, mpc_t op2, mpc_rnd_t rnd)
 2. Copy the following lines in front of your test file:

 #include <stdio.h>
 #include <stdlib.h>
 #include "gmp.h"
 #include "mpfr.h"
 #include "mpc.h"

 #include "random.c"
 #define TEST_FUNCTION mpc_function
 #define REFERENCE_FUNCTION optional_reference_function
 #include "tgeneric_ccc.c"

 3. #define TEST_FUNCTION as the function under test before including
 tgeneric_ccc.c in your test file.
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
rounding_failed (mpc_srcptr op, mpc_srcptr op2, mpc_srcptr rop,
		mpc_srcptr rop4, mpc_srcptr rop4rnd, mpc_rnd_t rnd)
{
  printf ("Rounding in " FUNCTION_NAME(TEST_FUNCTION) " might be "
	  "incorrect for\n");
  OUT (op);
  OUT (op2);

  printf ("with rounding mode (%s, %s)",
	  mpfr_print_rnd_mode (MPC_RND_RE (rnd)),
	  mpfr_print_rnd_mode (MPC_RND_IM (rnd)));

  printf ("\n" FUNCTION_NAME(TEST_FUNCTION) "                     gives ");
  OUT (rop);
  printf (FUNCTION_NAME(TEST_FUNCTION) " quadruple precision gives ");
  OUT (rop4);
  printf ("and is rounded to                  ");
  OUT (rop4rnd);

  exit (1);
}

static void
tgeneric(mpfr_prec_t prec_min, mpfr_prec_t prec_max, mpfr_prec_t step, mp_exp_t exp_max)
{
  mpc_t  x, y, z, t, u;
  mpc_rnd_t rnd_re;
  mpc_rnd_t rnd_im;
  mpfr_prec_t prec;
  mp_exp_t exp_min;

  mpc_init (x);
  mpc_init (y);
  mpc_init (z);
  mpc_init (t);
  mpc_init (u);

  exp_min = mpfr_get_emin ();
  if (exp_max <= 0)
    exp_max = mpfr_get_emax ();
  else if (exp_max > mpfr_get_emax ())
    exp_max = mpfr_get_emax();
  if (-exp_max > exp_min)
    exp_min = - exp_max;

  for (prec = prec_min; prec <= prec_max; prec+=step)
    {
      mpc_set_prec (x, prec);
      mpc_set_prec (z, prec);
      mpc_set_prec (t, prec);
      mpc_set_prec (u, 4*prec);
      mpc_set_prec (y, prec);

      test_default_random (x, exp_min, exp_max, 1);
      test_default_random (y, exp_min, exp_max, 1);

      for (rnd_re = 0; rnd_re < 4; rnd_re ++)
        for (rnd_im = 0; rnd_im < 4; rnd_im ++)
          {

            /* We compute the result with four times the precision and      */
            /* check whether the rounding is correct. Error reports in this */
            /* part of the algorithm might still be wrong, though, since    */
            /* there are two consecutive roundings (but we try to avoid     */
            /* them).                                                       */
            const mpc_rnd_t rnd = RNDC (rnd_re, rnd_im);

	    TEST_FUNCTION (u, x, y, rnd);
	    TEST_FUNCTION (z, x, y, rnd);
            if (MPFR_CAN_ROUND (MPC_RE (u), 1, prec, rnd_re)
                && MPFR_CAN_ROUND (MPC_IM (u), 1, prec, rnd_im))
              {
                mpc_set (t, u, rnd);
                if (mpc_cmp (z, t) != 0)
                  rounding_failed (x, y, z, u, t, rnd);
              }

#ifdef REFERENCE_FUNCTION
            /* check against reference implementation */
            REFERENCE_FUNCTION (t, x, y, rnd);
            if (mpc_cmp (z, t) != 0)
              {
                mpc_t ref;
                mpc_t got;

                /* aliases for display */
                ref[0] = t[0];
                got[0] = z[0];

                printf ("%s(x,y) and %s(x,y) differ for rnd=(%s,%s)\n",
                        FUNCTION_NAME(TEST_FUNCTION),
                        FUNCTION_NAME(REFERENCE_FUNCTION),
                        mpfr_print_rnd_mode (rnd_re),
                        mpfr_print_rnd_mode (rnd_im));
                OUT (x);
                OUT (y);
                OUT (ref);
                OUT (got);

                exit (1);
              }
#endif /* REFERENCE_FUNCTION */
          }

      /* Test parameter reuse: the function should not use its output
         parameter in internal computations. */
      mpc_set (t, x, MPC_RNDNN); /* exact */
      TEST_FUNCTION (z, x, y, MPC_RNDNN);
      TEST_FUNCTION (x, x, y, MPC_RNDNN);
      if (mpc_cmp (z, x) != 0)
        {
          mpc_t expected;
          mpc_t got;

          /* aliases for display */
          expected[0] = z[0];
          got[0] = x[0];

          printf ("Error for %s(t, t, y) for ", FUNCTION_NAME(TEST_FUNCTION));
          OUT (t);
          OUT (y);
          OUT (expected);
          OUT (got);

          exit (1);
        }

      mpc_set (x, t, MPC_RNDNN); /* exact */
      TEST_FUNCTION (z, x, y, MPC_RNDNN);
      TEST_FUNCTION (y, x, y, MPC_RNDNN);
      if (mpc_cmp (z, y) != 0)
        {
          mpc_t expected;
          mpc_t got;

          /* aliases for display */
          expected[0] = z[0];
          got[0] = y[0];

          printf ("Error for %s(t, x, t) for ", FUNCTION_NAME(TEST_FUNCTION));
          OUT (x);
          OUT (t);
          OUT (expected);
          OUT (got);

          exit (1);
        }
    }

  mpc_clear (x);
  mpc_clear (y);
  mpc_clear (z);
  mpc_clear (t);
  mpc_clear (u);
}
