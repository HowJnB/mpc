/* template for complex -> real function, for example abs

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

#define CAT1(X,Y) X ## Y
#define CAT2(X,Y) CAT1(X,Y)
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#define FUN CAT2(test_,FOO)
#define MPC_FOO CAT2(mpc_,FOO)
#define CFOO CAT2(CAT2(c,FOO),SUFFIX)
#define BAR TOSTRING(FOO)

static void
FUN (mpfr_prec_t p, unsigned long n)
{
  unsigned long i = 0;
  mpc_t x;
  mpfr_t z, t;
  TYPE complex xx;
  TYPE zz;
  int inex;
  unsigned long errors = 0, max_err = 0;

  printf ("Testing function %s\n", BAR);

  gmp_randseed_ui (state, seed);

  mpc_init2 (x, p);
  mpfr_init2 (z, p);
  mpfr_init2 (t, p);
  for (i = 0; i < n; i++)
    {
      mpcheck_random (x);
      inex = MPC_FOO (z, x, MPC_RNDNN);
      mpfr_subnormalize (z, inex, MPFR_RNDN);
      xx = mpc_get_type (x, MPC_RNDNN);
      zz = CFOO (xx);
      mpfr_set_type (t, zz, MPFR_RNDN);
      if (mpcheck_mpfr_cmp (t, z) != 0)
        {
          unsigned long err = ulp_error (t, z);
          if (verbose > 0 && err > max_err)
            {
              mpfr_printf ("   mpc_%s and c%s differ by %lu ulp(s)\n      for x=(%Re,%Re)\n",
                           BAR, BAR, err,
                           mpc_realref (x), mpc_imagref (x));
              mpfr_printf ("   mpc_%s gives %Re\n", BAR, z);
              mpfr_printf ("      c%s gives %Re\n", BAR, t);
            }
          errors++;
          if (err > max_err)
            max_err = err;
        }
    }
  mpc_clear (x);
  mpfr_clear (z);
  mpfr_clear (t);
  printf ("Errors for %s: %lu/%lu (max %lu) [seed %lu]\n",
          BAR, errors, n, max_err, seed);
}

#undef FOO
#undef CFOO
