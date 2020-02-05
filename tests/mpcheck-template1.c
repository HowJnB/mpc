/* template for complex -> complex function, for example acos

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
  mpc_t x, z, t;
  TYPE complex xx, zz;
  int inex;
  unsigned long errors = 0, max_err_re = 0, max_err_im = 0;

  gmp_randseed_ui (state, seed);

  mpc_init2 (x, p);
  mpc_init2 (z, p);
  mpc_init2 (t, p);
  for (i = 0; i < n; i++)
    {
      mpc_urandom (x, state);
      inex = MPC_FOO (z, x, MPC_RNDNN);
      mpfr_subnormalize (mpc_realref (z), MPC_INEX_RE(inex), MPFR_RNDN);
      mpfr_subnormalize (mpc_imagref (z), MPC_INEX_IM(inex), MPFR_RNDN);
      xx = mpc_get_type (x, MPC_RNDNN);
      zz = CFOO (xx);
      mpc_set_type (t, zz, MPFR_RNDN);
      if (mpc_cmp (z, t) != 0)
        {
          unsigned long err_re = ulp_error (mpc_realref (t), mpc_realref (z));
          unsigned long err_im = ulp_error (mpc_imagref (t), mpc_imagref (z));
          if (verbose > 0 && (err_re > max_err_re || err_im > max_err_im))
            {
              mpfr_printf ("   mpc_%s and c%s differ by (%lu,%lu) ulp(s)\n      for x=(%Re,%Re)\n",
                           BAR, BAR, err_re, err_im,
                           mpc_realref (x), mpc_imagref (x));
              mpfr_printf ("   mpc_%s gives (%Re,%Re)\n", BAR,
                           mpc_realref (z), mpc_imagref (z));
              mpfr_printf ("      c%s gives (%Re,%Re)\n", BAR,
                           mpc_realref (t), mpc_imagref (t));
            }
          errors ++;
          if (err_re > max_err_re)
            max_err_re = err_re;
          if (err_im > max_err_im)
            max_err_im = err_im;
        }
    }
  mpc_clear (x);
  mpc_clear (z);
  mpc_clear (t);
  printf ("Number of errors for %s: %lu/%lu (max %lu,%lu)\n", BAR,
          errors, n, max_err_re, max_err_im);
}

#undef FOO
#undef CFOO
