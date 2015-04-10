/* exceptions -- test file for exceptions

Copyright (C) 2015 INRIA

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

#include "mpc-tests.h"

/* Return non-zero if 'rnd' rounds towards zero, for a number of sign 'sgn' */
#define MPC_IS_LIKE_RNDZ(rnd, sgn) \
  ((rnd==MPFR_RNDZ) || (sgn<0 && rnd==MPFR_RNDU) || (sgn>0 && rnd==MPFR_RNDD))

#define MPFR_SGN(x) (mpfr_signbit (x) ? -1 : 1)

static void
foo (int f(mpc_ptr, mpc_srcptr, mpc_rnd_t), char *s, mpfr_exp_t ex,
     mpfr_exp_t ey)
{
  mpc_t z, t;
  int rnd_re, rnd_im, rnd;

  printf ("Testing %s with ex=%ld ey=%ld\n", s, ex, ey);
  fflush (stdout);
  
  mpc_init2 (z, MPFR_PREC_MIN);
  mpc_init2 (t, MPFR_PREC_MIN);
  mpfr_set_ui_2exp (mpc_realref (z), 1, ex, MPFR_RNDN);
  mpfr_set_ui_2exp (mpc_imagref (z), 1, ey, MPFR_RNDN);

  f (t, z, MPC_RNDZZ);
  if (mpfr_inf_p (mpc_realref (t)))
    {
      fprintf (stderr, "Error, wrong real part with rounding towards zero\n");
      fprintf (stderr, "f = %s\n", s);
      fprintf (stderr, "z=");
      mpc_out_str (stderr, 2, 0, z, MPC_RNDNN);
      fprintf (stderr, "\nt=");
      mpc_out_str (stderr, 2, 0, t, MPC_RNDNN);
      fprintf (stderr, "\n");
      exit (1);
    }
  if (mpfr_inf_p (mpc_imagref (t)))
    {
      fprintf (stderr, "Error, wrong imag part with rounding towards zero\n");
      fprintf (stderr, "f = %s\n", s);
      fprintf (stderr, "z=");
      mpc_out_str (stderr, 2, 0, z, MPC_RNDNN);
      fprintf (stderr, "\nt=");
      mpc_out_str (stderr, 2, 0, t, MPC_RNDNN);
      fprintf (stderr, "\n");
      exit (1);
    }

  rnd_re = mpfr_signbit (mpc_realref (t)) == 0 ? MPFR_RNDU : MPFR_RNDD;
  rnd_im = mpfr_signbit (mpc_imagref (t)) == 0 ? MPFR_RNDU : MPFR_RNDD;
  rnd = MPC_RND(rnd_re,rnd_im); /* round away */
  f (t, z, rnd);
  if (mpfr_zero_p (mpc_realref (t)))
    {
      fprintf (stderr, "Error, wrong real part with rounding away from zero\n");
      fprintf (stderr, "f = %s\n", s);
      fprintf (stderr, "z=");
      mpc_out_str (stderr, 2, 0, z, MPC_RNDNN);
      fprintf (stderr, "\nt=");
      mpc_out_str (stderr, 2, 0, t, MPC_RNDNN);
      fprintf (stderr, "\n");
      fprintf (stderr, "rnd=%s\n", mpfr_print_rnd_mode (rnd_re));
      exit (1);
    }
  if (mpfr_zero_p (mpc_imagref (t)))
    {
      fprintf (stderr, "Error, wrong imag part with rounding away from zero\n");
      fprintf (stderr, "f = %s\n", s);
      fprintf (stderr, "z=");
      mpc_out_str (stderr, 2, 0, z, MPC_RNDNN);
      fprintf (stderr, "\nt=");
      mpc_out_str (stderr, 2, 0, t, MPC_RNDNN);
      fprintf (stderr, "\n");
      fprintf (stderr, "rnd=%s\n", mpfr_print_rnd_mode (rnd_im));
      exit (1);
    }

  mpc_clear (z);
  mpc_clear (t);
}

int
main (void)
{
  test_start ();

  foo (mpc_sqr, "sqr", 100, 800);
  foo (mpc_conj, "conj", -100, 100);
  foo (mpc_neg, "neg", -100, 100);
  foo (mpc_sqrt, "sqrt", -50, 50);
  foo (mpc_set, "set", -100, 100);
  foo (mpc_proj, "proj", -100, 100);
  foo (mpc_exp, "exp", 200, 800);
  foo (mpc_exp, "exp", 800, 200);
  foo (mpc_log, "log", -100, 100);
  foo (mpc_log10, "log10", -100, 100);
  foo (mpc_sin, "sin", -100, 100);
  foo (mpc_cos, "cos", -100, 100);
  foo (mpc_tan, "tan", -10, 1000);
  foo (mpc_sinh, "sinh", -100, 100);
  foo (mpc_cosh, "cosh", -100, 100);
  foo (mpc_tanh, "tanh", -100, 100);
  foo (mpc_asin, "asin", -100, 100);
  foo (mpc_acos, "acos", -100, 100);
  foo (mpc_atan, "atan", -100, 100);
  foo (mpc_asinh, "asinh", -100, 100);
  foo (mpc_acosh, "acosh", -100, 100);
  foo (mpc_atanh, "atanh", -100, 100);

  test_end ();

  return 0;
}
