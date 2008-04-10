/* tmul -- test file for mpc_mul.

Copyright (C) 2002, 2005 Andreas Enge, Paul Zimmermann

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
#ifdef TIMING
#include <sys/times.h>
#endif
#include "gmp.h"
#include "mpfr.h"
#include "mpc.h"
#include "mpc-impl.h"

void cmpmul (mpc_srcptr, mpc_srcptr, mpc_rnd_t);
void cmpmului (mpc_srcptr, unsigned long int, mpc_rnd_t);
void cmpmulsi (mpc_srcptr, long int, mpc_rnd_t);
void testmul (long, long, long, long, mp_prec_t, mpc_rnd_t);
void special (void);
#ifdef TIMING
void timemul (void);
#endif

void cmpmul (mpc_srcptr x, mpc_srcptr y, mpc_rnd_t rnd)
   /* computes the product of x and y with the naive and Karatsuba methods */
   /* using the rounding mode rnd and compares the results and return      */
   /* values.                                                              */
   /* In our current test suite, the real and imaginary parts of x and y   */
   /* all have the same precision, and we use this precision also for the  */
   /* result.                                                              */
   /* Furthermore, we check whether the multiplication with one of the     */
   /* input arguments being also the output variable yields the same       */
   /* result.                                                              */
   /* We also compute the result with four times the precision and check   */
   /* whether the rounding is correct. Error reports in this part of the   */
   /* algorithm might still be wrong, though, since there are two          */
   /* consecutive roundings.                                               */
{
  mpc_t z, t, u;
  int   inexact_z, inexact_t;

  mpc_init2 (z, MPC_MAX_PREC (x));
  mpc_init2 (t, MPC_MAX_PREC (x));
  mpc_init2 (u, 4 * MPC_MAX_PREC (x));

  inexact_z = mpc_mul_naive (z, x, y, rnd);
  inexact_t = mpc_mul_karatsuba (t, x, y, rnd);

  if (mpc_cmp (z, t))
    {
      fprintf (stderr, "mul and mul2 differ for rnd=(%s,%s) \nx=",
               mpfr_print_rnd_mode(MPC_RND_RE(rnd)),
               mpfr_print_rnd_mode(MPC_RND_IM(rnd)));
      mpc_out_str (stderr, 2, 0, x, MPC_RNDNN);
      fprintf (stderr, "\nand y=");
      mpc_out_str (stderr, 2, 0, y, MPC_RNDNN);
      fprintf (stderr, "\nmpc_mul_naive     gives ");
      mpc_out_str (stderr, 2, 0, z, MPC_RNDNN);
      fprintf (stderr, "\nmpc_mul_karatsuba gives ");
      mpc_out_str (stderr, 2, 0, t, MPC_RNDNN);
      fprintf (stderr, "\n");
      exit (1);
    }
  if (inexact_z != inexact_t)
    {
      fprintf (stderr, "The return values of mul and mul2 differ for rnd=(%s,%s) \nx=",
               mpfr_print_rnd_mode(MPC_RND_RE(rnd)),
               mpfr_print_rnd_mode(MPC_RND_IM(rnd)));
      mpc_out_str (stderr, 2, 0, x, MPC_RNDNN);
      fprintf (stderr, "\nand y=");
      mpc_out_str (stderr, 2, 0, y, MPC_RNDNN);
      fprintf (stderr, "\nand x*y=");
      mpc_out_str (stderr, 2, 0, z, MPC_RNDNN);
      fprintf (stderr, "\nmpc_mul_naive     gives %i", inexact_z);
      fprintf (stderr, "\nmpc_mul_karatsuba gives %i", inexact_t);
      fprintf (stderr, "\n");
      exit (1);
    }

  mpc_set (t, x, MPC_RNDNN);
  inexact_t = mpc_mul (t, t, y, rnd);
  if (mpc_cmp (z, t))
    {
      fprintf (stderr, "mul and mul with the first variable in place differ for rnd=(%s,%s) \nx=",
               mpfr_print_rnd_mode(MPC_RND_RE(rnd)),
               mpfr_print_rnd_mode(MPC_RND_IM(rnd)));
      mpc_out_str (stderr, 2, 0, x, MPC_RNDNN);
      fprintf (stderr, "\nand y=");
      mpc_out_str (stderr, 2, 0, y, MPC_RNDNN);
      fprintf (stderr, "\nmpc_mul          gives ");
      mpc_out_str (stderr, 2, 0, z, MPC_RNDNN);
      fprintf (stderr, "\nmpc_mul in place gives ");
      mpc_out_str (stderr, 2, 0, t, MPC_RNDNN);
      fprintf (stderr, "\n");
      exit (1);
    }
  if (inexact_z != inexact_t)
    {
      fprintf (stderr, "The return values of mul and mul with the first variable in place differ for rnd=(%s,%s) \nx=",
               mpfr_print_rnd_mode(MPC_RND_RE(rnd)),
               mpfr_print_rnd_mode(MPC_RND_IM(rnd)));
      mpc_out_str (stderr, 2, 0, x, MPC_RNDNN);
      fprintf (stderr, "\nand y=");
      mpc_out_str (stderr, 2, 0, y, MPC_RNDNN);
      fprintf (stderr, "\nand x*y=");
      mpc_out_str (stderr, 2, 0, z, MPC_RNDNN);
      fprintf (stderr, "\nmpc_mul          gives %i", inexact_z);
      fprintf (stderr, "\nmpc_mul in place gives %i", inexact_t);
      fprintf (stderr, "\n");
      exit (1);
    }

  /* the following test is valid only when y can be copied in t exactly */
  if (mpc_set (t, y, MPC_RNDNN) == 0)
    {
      inexact_t = mpc_mul (t, x, t, rnd);
      if (mpc_cmp (z, t))
        {
          fprintf (stderr, "mul and mul with the second variable in place differ for rnd=(%s,%s) \nx=",
                   mpfr_print_rnd_mode(MPC_RND_RE(rnd)),
                   mpfr_print_rnd_mode(MPC_RND_IM(rnd)));
          mpc_out_str (stderr, 2, 0, x, MPC_RNDNN);
          fprintf (stderr, "\nand y=");
          mpc_out_str (stderr, 2, 0, y, MPC_RNDNN);
          fprintf (stderr, "\nmpc_mul          gives ");
          mpc_out_str (stderr, 2, 0, z, MPC_RNDNN);
          fprintf (stderr, "\nmpc_mul in place gives ");
          mpc_out_str (stderr, 2, 0, t, MPC_RNDNN);
          fprintf (stderr, "\n");
          exit (1);
        }
      if (inexact_z != inexact_t)
        {
          fprintf (stderr, "The return values of mul and mul with the second variable in place differ for rnd=(%s,%s) \nx=",
                   mpfr_print_rnd_mode(MPC_RND_RE(rnd)),
                   mpfr_print_rnd_mode(MPC_RND_IM(rnd)));
          mpc_out_str (stderr, 2, 0, x, MPC_RNDNN);
          fprintf (stderr, "\nand y=");
          mpc_out_str (stderr, 2, 0, y, MPC_RNDNN);
          fprintf (stderr, "\nand x*y=");
          mpc_out_str (stderr, 2, 0, z, MPC_RNDNN);
          fprintf (stderr, "\nmpc_mul          gives %i", inexact_z);
          fprintf (stderr, "\nmpc_mul in place gives %i", inexact_t);
          fprintf (stderr, "\n");
          exit (1);
        }
    }

  mpc_mul (u, x, y, rnd);
  mpc_set (t, u, rnd);
  if (mpc_cmp (z, t))
    {
      fprintf (stderr, "rounding in mul might be incorrect for rnd=(%s,%s) \nx=",
               mpfr_print_rnd_mode(MPC_RND_RE(rnd)),
               mpfr_print_rnd_mode(MPC_RND_IM(rnd)));
      mpc_out_str (stderr, 2, 0, x, MPC_RNDNN);
      fprintf (stderr, "\nand y=");
      mpc_out_str (stderr, 2, 0, y, MPC_RNDNN);
      fprintf (stderr, "\nmpc_mul                     gives ");
      mpc_out_str (stderr, 2, 0, z, MPC_RNDNN);
      fprintf (stderr, "\nmpc_mul quadruple precision gives ");
      mpc_out_str (stderr, 2, 0, u, MPC_RNDNN);
      fprintf (stderr, "\nand is rounded to                 ");
      mpc_out_str (stderr, 2, 0, t, MPC_RNDNN);
      fprintf (stderr, "\n");
      exit (1);
    }

  mpc_clear (z);
  mpc_clear (t);
  mpc_clear (u);
}


void cmpmului (mpc_srcptr x, unsigned long int y, mpc_rnd_t rnd)
     /* computes the product of x and y using mpc_mul_fr and mpc_mul_ui      */
     /* using the rounding mode rnd and compares the results and return      */
     /* values.                                                              */
     /* In our current test suite, the real and imaginary parts of x have    */
     /* the same precision, and we use this precision also for the result.   */

{
  mpc_t z, t;
  mpfr_t yf;
  int   inexact_z, inexact_t;

  mpc_init2 (z, MPC_MAX_PREC (x));
  mpc_init2 (t, MPC_MAX_PREC (x));
  mpfr_init2 (yf, 8 * sizeof (long int));
  mpfr_set_si (yf, y, GMP_RNDN);

  inexact_z = mpc_mul_fr (z, x, yf, rnd);
  inexact_t = mpc_mul_ui (t, x, y, rnd);

  if (mpc_cmp (z, t))
    {
      fprintf (stderr, "mul_fr and mul_ui differ for rnd=(%s,%s) \nx=",
               mpfr_print_rnd_mode(MPC_RND_RE(rnd)),
               mpfr_print_rnd_mode(MPC_RND_IM(rnd)));
      mpc_out_str (stderr, 2, 0, x, MPC_RNDNN);
      fprintf (stderr, "\nand y=%li", y);
      fprintf (stderr, "\nmpc_mul_fr gives ");
      mpc_out_str (stderr, 2, 0, z, MPC_RNDNN);
      fprintf (stderr, "\nmpc_mul_ui gives ");
      mpc_out_str (stderr, 2, 0, t, MPC_RNDNN);
      fprintf (stderr, "\n");
      exit (1);
    }
  if (inexact_z != inexact_t)
    {
      fprintf (stderr, "The return values of mul_fr and mul_ui differ for ");
      fprintf (stderr, "rnd=(%s,%s) \nx=",
               mpfr_print_rnd_mode(MPC_RND_RE(rnd)),
               mpfr_print_rnd_mode(MPC_RND_IM(rnd)));
      mpc_out_str (stderr, 2, 0, x, MPC_RNDNN);
      fprintf (stderr, "\nand y=%li", y);
      fprintf (stderr, "\nand x*y=");
      mpc_out_str (stderr, 2, 0, z, MPC_RNDNN);
      fprintf (stderr, "\nmpc_mul_fr gives %i", inexact_z);
      fprintf (stderr, "\nmpc_mul_ui gives %i", inexact_t);
      fprintf (stderr, "\n");
      exit (1);
    }

  mpc_clear (z);
  mpc_clear (t);
  mpfr_clear (yf);
}

void cmpmulsi (mpc_srcptr x, long int y, mpc_rnd_t rnd)
     /* computes the product of x and y using mpc_mul_fr and mpc_mul_si      */
     /* using the rounding mode rnd and compares the results and return      */
     /* values.                                                              */
     /* In our current test suite, the real and imaginary parts of x have    */
     /* the same precision, and we use this precision also for the result.   */

{
  mpc_t z, t;
  mpfr_t yf;
  int   inexact_z, inexact_t;

  mpc_init2 (z, MPC_MAX_PREC (x));
  mpc_init2 (t, MPC_MAX_PREC (x));
  mpfr_init2 (yf, 8 * sizeof (long int));
  mpfr_set_si (yf, y, GMP_RNDN);

  inexact_z = mpc_mul_fr (z, x, yf, rnd);
  inexact_t = mpc_mul_si (t, x, y, rnd);

  if (mpc_cmp (z, t))
    {
      fprintf (stderr, "mul_fr and mul_si differ for rnd=(%s,%s) \nx=",
               mpfr_print_rnd_mode(MPC_RND_RE(rnd)),
               mpfr_print_rnd_mode(MPC_RND_IM(rnd)));
      mpc_out_str (stderr, 2, 0, x, MPC_RNDNN);
      fprintf (stderr, "\nand y=%li", y);
      fprintf (stderr, "\nmpc_mul_fr gives ");
      mpc_out_str (stderr, 2, 0, z, MPC_RNDNN);
      fprintf (stderr, "\nmpc_mul_si gives ");
      mpc_out_str (stderr, 2, 0, t, MPC_RNDNN);
      fprintf (stderr, "\n");
      exit (1);
    }
  if (inexact_z != inexact_t)
    {
      fprintf (stderr, "The return values of mul_fr and mul_si differ for ");
      fprintf (stderr, "rnd=(%s,%s) \nx=",
               mpfr_print_rnd_mode(MPC_RND_RE(rnd)),
               mpfr_print_rnd_mode(MPC_RND_IM(rnd)));
      mpc_out_str (stderr, 2, 0, x, MPC_RNDNN);
      fprintf (stderr, "\nand y=%li", y);
      fprintf (stderr, "\nand x*y=");
      mpc_out_str (stderr, 2, 0, z, MPC_RNDNN);
      fprintf (stderr, "\nmpc_mul_fr gives %i", inexact_z);
      fprintf (stderr, "\nmpc_mul_si gives %i", inexact_t);
      fprintf (stderr, "\n");
      exit (1);
    }

  mpc_clear (z);
  mpc_clear (t);
  mpfr_clear (yf);
}


void
testmul (long a, long b, long c, long d, mp_prec_t prec, mpc_rnd_t rnd)
{
  mpc_t x, y;

  mpc_init2 (x, prec);
  mpc_init2 (y, prec);

  mpc_set_si_si (x, a, b, rnd);
  mpc_set_si_si (y, c, d, rnd);

  cmpmul (x, y, rnd);

  mpc_clear (x);
  mpc_clear (y);
}


void
special ()
{
  mpc_t x, y, z, t;
  int inexact;

  mpc_init (x);
  mpc_init (y);
  mpc_init (z);
  mpc_init (t);

  mpc_set_prec (x, 8);
  mpc_set_prec (y, 8);
  mpc_set_prec (z, 8);
  mpc_set_si_si (x, 4, 3, MPC_RNDNN);
  mpc_set_si_si (y, 1, -2, MPC_RNDNN);
  inexact = mpc_mul (z, x, y, MPC_RNDNN);
  if (MPC_INEX_RE(inexact) || MPC_INEX_IM(inexact))
    {
      fprintf (stderr, "Error: (4+3*I)*(1-2*I) should be exact with prec=8\n");
      exit (1);
    }

  mpc_set_prec (x, 27);
  mpc_set_prec (y, 27);
  mpfr_set_str (MPC_RE(x), "1.11111011011000010101000000e-2", 2, GMP_RNDN);
  mpfr_set_str (MPC_IM(x), "1.11010001010110111001110001e-3", 2, GMP_RNDN);
  mpfr_set_str (MPC_RE(y), "1.10100101110110011011100100e-1", 2, GMP_RNDN);
  mpfr_set_str (MPC_IM(y), "1.10111100011000001100110011e-1", 2, GMP_RNDN);
  cmpmul (x, y, 0);

  mpc_clear (x);
  mpc_clear (y);
}

#ifdef TIMING
void
timemul ()
{
  /* measures the time needed with different precisions for naive and */
  /* Karatsuba multiplication                                         */

  mpc_t             x, y, z;
  unsigned long int i, j;
  const unsigned long int tests = 10000;
  struct tms        time_old, time_new;
  double            passed1, passed2;

  mpc_init (x);
  mpc_init (y);
  mpc_init_set_ui_ui (z, 1, 0, MPC_RNDNN);

  for (i = 1; i < 50; i++)
    {
      mpc_set_prec (x, i * BITS_PER_MP_LIMB);
      mpc_set_prec (y, i * BITS_PER_MP_LIMB);
      mpc_set_prec (z, i * BITS_PER_MP_LIMB);
      mpc_random (x);
      mpc_random (y);

      times (&time_old);
      for (j = 0; j < tests; j++)
        mpc_mul_naive (z, x, y, MPC_RNDNN);
      times (&time_new);
      passed1 = ((double) (time_new.tms_utime - time_old.tms_utime)) / 100;

      times (&time_old);
      for (j = 0; j < tests; j++)
        mpc_mul_karatsuba (z, x, y, MPC_RNDNN);
      times (&time_new);
      passed2 = ((double) (time_new.tms_utime - time_old.tms_utime)) / 100;

      printf ("Time for %3li limbs naive/Karatsuba: %5.2f %5.2f\n", i,
              passed1, passed2);
    }

  mpc_clear (x);
  mpc_clear (y);
  mpc_clear (z);
}
#endif

int
main()
{
  mpc_t x, y;
  mpc_rnd_t rnd_re, rnd_im;
  mp_prec_t prec;
  int i;
  long int ysi;

#ifdef TIMING
  timemul ();
#endif

  special ();

  testmul (247, -65, -223, 416, 8, 24);
  testmul (5, -896, 5, -32, 3, 2);
  testmul (-3, -512, -1, -1, 2, 16);
  testmul (266013312, 121990769, 110585572, 116491059, 27, 0);
  testmul (170, 9, 450, 251, 8, 0);
  testmul (768, 85, 169, 440, 8, 16);
  testmul (145, 1816, 848, 169, 8, 24);
  testmul (0, 1816, 848, 169, 8, 24);
  testmul (145, 0, 848, 169, 8, 24);
  testmul (145, 1816, 0, 169, 8, 24);
  testmul (145, 1816, 848, 0, 8, 24);

  mpc_init (x);
  mpc_init (y);

  for (prec = 2; prec < 1000; prec++) {

    mpc_set_prec (x, prec);
    mpc_set_prec (y, prec);

    for (i = 0; i < 1000/prec; i++)
      {

        mpc_random (x);
        mpc_random (y);
        ysi = rand ();

        for (rnd_re = 0; rnd_re < 4; rnd_re ++)
          for (rnd_im = 0; rnd_im < 4; rnd_im ++)
            {
              cmpmul (x, y, RNDC(rnd_re, rnd_im));
              cmpmului (x, (unsigned long int) ysi, RNDC(rnd_re, rnd_im));
              cmpmulsi (x, ysi, RNDC(rnd_re, rnd_im));
              cmpmulsi (x, -ysi, RNDC(rnd_re, rnd_im));
            }
      }
  }

  mpc_clear (x);
  mpc_clear (y);

  return 0;
}
