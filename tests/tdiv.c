/* tdiv -- test file for mpc_div.

Copyright (C) 2002 Andreas Enge, Paul Zimmermann

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
#include "mpc-impl.h"

#define ERR(x) { mpc_out_str (stderr, 2, 0, x, MPC_RNDNN); \
                 fprintf (stderr, "\n"); }

static int
mpc_div_ref (mpc_ptr a, mpc_srcptr b, mpc_srcptr c, mpc_rnd_t rnd)
{
  int ok = 0, inexact_q, inex_re, inex_im = 0, cancel = 0, sgn;
  mpfr_t u, v, q, t;
  mp_prec_t prec, err;
  mp_rnd_t rnd_re, rnd_im;

  if (mpfr_cmp_ui (MPC_IM(c), 0) == 0) /* c is real */
    {
      /* compute imaginary part first in case a=b or a=c */
      inex_im = mpfr_div (MPC_IM(a), MPC_IM(b), MPC_RE(c), MPC_RND_IM(rnd));
      inex_re = mpfr_div (MPC_RE(a), MPC_RE(b), MPC_RE(c), MPC_RND_RE(rnd));
      return MPC_INEX(inex_re, inex_im);
    }

  prec = MPC_MAX_PREC(a);

  /* try to guess the signs of the real and imaginary parts */
  sgn = mpfr_sgn (MPC_RE(b)) * mpfr_sgn (MPC_RE(c))
    + mpfr_sgn (MPC_IM(b)) * mpfr_sgn (MPC_IM(c));
  rnd_re = (sgn > 0) ? GMP_RNDU : GMP_RNDD;
  sgn = mpfr_sgn (MPC_RE(b)) * mpfr_sgn (MPC_IM(c))
    - mpfr_sgn (MPC_IM(b)) * mpfr_sgn (MPC_RE(c));
  rnd_im = (sgn > 0) ? GMP_RNDU : GMP_RNDD;

  /* first try with only one real division */
  prec += mpc_ceil_log2 (prec) + 3;

  mpfr_init2 (u, prec);
  mpfr_init2 (v, prec);
  mpfr_init2 (q, prec);
  mpfr_init2 (t, prec);

  /* first compute 1/norm(c)^2 rounded away */
  inexact_q = mpfr_mul (u, MPC_RE(c), MPC_RE(c), GMP_RNDZ);
  inexact_q |= mpfr_mul (v, MPC_IM(c), MPC_IM(c), GMP_RNDZ);
  inexact_q |= mpfr_add (q, u, v, GMP_RNDZ); /* 3 ulps */
  inexact_q |= mpfr_ui_div (q, 1, q, GMP_RNDU); /* 7 ulps */

  /* now compute b*conjugate(c) */

  /* real part */
  inex_re = mpfr_mul (u, MPC_RE(b), MPC_RE(c), rnd_re); /* 1 */
  inex_re |= mpfr_mul (v, MPC_IM(b), MPC_IM(c), rnd_re); /* 1 */
  inex_re |= mpfr_add (t, u, v, rnd_re);
  if (MPFR_IS_ZERO(u))
    {
      if (MPFR_IS_ZERO(v))
        cancel = 0;
      else
        cancel = MPFR_EXP(v) - MPFR_EXP(t);
    }
  else if (MPFR_IS_ZERO(v))
    cancel = MPFR_EXP(u) - MPFR_EXP(t);
  else
    {
       cancel = (MPFR_IS_ZERO(t)) ? (mp_exp_t) prec
        : MAX(MPFR_EXP(u), MPFR_EXP(v)) - MPFR_EXP(t);
    }
  /* err(t) <= [1 + 2*2^cancel] ulp(t) */
  inex_re |= mpfr_mul (t, t, q, rnd_re) || inexact_q;
  /* err(t) <= [1 + 3*(1 + 2*2^cancel) + 14] ulp(t) */
  err = (cancel <= 1) ? 5 : ((cancel == 2) ? 6 :
                             ((cancel <= 4) ? 7 : cancel + 3));
  ok = (inex_re == 0)
    || ((err < prec) && mpfr_can_round (t, prec - err, rnd_re,
                                        MPC_RND_RE (rnd),
                                        MPFR_PREC (MPC_RE (a))));
  if ((rnd_re == GMP_RNDU && mpfr_sgn (t) < 0)
      || (rnd_re == GMP_RNDD && mpfr_sgn (t) > 0))
    {
      ok = 0;
      rnd_re = INV_RND(rnd_re);
    }

  if (ok) /* compute imaginary part */
    {
      inex_im = mpfr_mul (u, MPC_RE(b), MPC_IM(c), INV_RND(rnd_im));
      inex_im |= mpfr_mul (v, MPC_IM(b), MPC_RE(c), rnd_im);
      if (MPFR_IS_ZERO(u))
        {
          if (MPFR_IS_ZERO(v))
            cancel = 0;
          else
            cancel = MPFR_EXP(v);
        }
      else if (MPFR_IS_ZERO(v))
        cancel = MPFR_EXP(u);
      else
        cancel = MAX(MPFR_EXP(u), MPFR_EXP(v));
      inex_im |= mpfr_sub (u, v, u, rnd_im);
      if (!MPFR_IS_ZERO(u))
        cancel = cancel - MPFR_EXP(u);
      inex_im |= mpfr_mul (u, u, q, rnd_im) || inexact_q;
      err = (cancel <= 1) ? 5 : ((cancel == 2) ? 6 :
                                 ((cancel <= 4) ? 7 : cancel + 3));
      ok = (inex_im == 0)
        || ((err < prec) && mpfr_can_round (u, prec - err, rnd_im,
                                            MPC_RND_IM (rnd),
                                            MPFR_PREC (MPC_IM (a))));
      if ((rnd_im == GMP_RNDU && mpfr_sgn (u) < 0)
          || (rnd_im == GMP_RNDD && mpfr_sgn (u) > 0))
        {
          ok = 0;
          rnd_im = INV_RND(rnd_im);
        }
    }

  /* now loop with two real divisions, to detect exact quotients */
  while (ok == 0)
    {
      prec += mpc_ceil_log2 (prec) + 3;

      mpfr_set_prec (u, prec);
      mpfr_set_prec (v, prec);
      mpfr_set_prec (q, prec);
      mpfr_set_prec (t, prec);

      /* first compute norm(c)^2 rounded towards zero */
      inexact_q = mpfr_mul (u, MPC_RE(c), MPC_RE(c), GMP_RNDZ);
      inexact_q |= mpfr_mul (v, MPC_IM(c), MPC_IM(c), GMP_RNDZ);
      inexact_q |= mpfr_add (q, u, v, GMP_RNDZ); /* 3 ulps */

      /* now compute b*conjugate(c) */

      /* real part */
      inex_re = mpfr_mul (u, MPC_RE(b), MPC_RE(c), rnd_re); /* 1 */
      inex_re |= mpfr_mul (v, MPC_IM(b), MPC_IM(c), rnd_re); /* 1 */
      inex_re |= mpfr_add (t, u, v, rnd_re);
      if (MPFR_IS_ZERO(u))
        {
          if (MPFR_IS_ZERO(v))
            cancel = 0;
          else
            cancel = MPFR_EXP(v) - MPFR_EXP(t);
        }
      else if (MPFR_IS_ZERO(v))
        cancel = MPFR_EXP(u) - MPFR_EXP(t);
      else
         cancel = (MPFR_IS_ZERO(t)) ? (mp_exp_t) prec
          : MAX(MPFR_EXP(u), MPFR_EXP(v)) - MPFR_EXP(t);
      /* err(t) <= [1 + 2*2^cancel] ulp(t) */
      inex_re |= mpfr_div (t, t, q, rnd_re) || inexact_q;
      /* err(t) <= [1 + 2*(1 + 2*2^cancel) + 6] ulp(t) */
      err = (cancel <= 0) ? 4 : cancel + 3; /* FIXME: cancel + 4 ? */
      ok = (inex_re == 0)
        || ((err < prec) && mpfr_can_round (t, prec - err, rnd_re,
                                            MPC_RND_RE (rnd),
                                            MPFR_PREC (MPC_RE (a))));
      if ((rnd_re == GMP_RNDU && mpfr_sgn (t) < 0)
          || (rnd_re == GMP_RNDD && mpfr_sgn (t) > 0))
        {
          ok = 0;
          rnd_re = INV_RND(rnd_re);
        }

      if (ok) /* compute imaginary part */
        {
          inex_im = mpfr_mul (u, MPC_RE(b), MPC_IM(c), INV_RND(rnd_im));
          inex_im |= mpfr_mul (v, MPC_IM(b), MPC_RE(c), rnd_im);
          if (MPFR_IS_ZERO(u))
            {
              if (MPFR_IS_ZERO(v))
                cancel = 0;
              else
                cancel = MPFR_EXP(v);
            }
          else if (MPFR_IS_ZERO(v))
            cancel = MPFR_EXP(u);
          else
            cancel = MAX(MPFR_EXP(u), MPFR_EXP(v));
          inex_im |= mpfr_sub (u, v, u, rnd_im);
          if (!MPFR_IS_ZERO(u))
            cancel = cancel - MPFR_EXP(u);
          inex_im |= mpfr_div (u, u, q, rnd_im) || inexact_q;
          err = (cancel <= 0) ? 4 : cancel + 3;
          ok = (inex_im == 0)
            || ((err < prec) && mpfr_can_round (u, prec - err, rnd_im,
                                                MPC_RND_IM (rnd),
                                                MPFR_PREC (MPC_IM (a))));
          if ((rnd_im == GMP_RNDU && mpfr_sgn (u) < 0)
              || (rnd_im == GMP_RNDD && mpfr_sgn (u) > 0))
            {
              ok = 0;
              rnd_im = INV_RND(rnd_im);
            }
        }
    }

  inex_re = mpfr_set (MPC_RE(a), t, MPC_RND_RE(rnd)) || inex_re;
  inex_im = mpfr_set (MPC_IM(a), u, MPC_RND_IM(rnd)) || inex_im;

  mpfr_clear (u);
  mpfr_clear (v);
  mpfr_clear (q);
  mpfr_clear (t);

  return MPC_INEX(inex_re, inex_im);
}

static void
check_regular (void)
{
  mpc_t b, c, q;
  int inex;

  mpc_init (b);
  mpc_init (c);
  mpc_init (q);

  mpc_set_prec (b, 10);
  mpc_set_prec (c, 10);
  mpc_set_prec (q, 10);

  /* inexact result */
  mpc_set_ui_ui (b, 973, 964, MPC_RNDNN);
  mpc_set_ui_ui (c, 725, 745, MPC_RNDNN);
  inex = mpc_div (q, b, c, MPC_RNDZZ);
  mpc_set_si_si (b, 43136, -787, MPC_RNDNN);
  mpc_div_2exp (b, b, 15, MPC_RNDNN);
  if (mpc_cmp (q, b) || MPC_INEX_RE (inex) == 0 || MPC_INEX_IM (inex) == 0)
    {
      printf ("mpc_div failed for (973+I*964)/(725+I*745)\n");
      exit (1);
    }

  /* exact result */
  mpc_set_si_si (b, -837, 637, MPC_RNDNN);
  mpc_set_si_si (c, 63, -5, MPC_RNDNN);
  inex = mpc_div (q, b, c, MPC_RNDZN);
  mpc_set_si_si (b, -14, 9, MPC_RNDNN);
  if (mpc_cmp (q, b) || inex != 0)
    {
      printf ("mpc_div failed for (-837+I*637)/(63-I*5)\n");
      exit (1);
    }

  mpc_set_prec (b, 2);
  mpc_set_prec (c, 2);
  mpc_set_prec (q, 2);

  /* exact result */
  mpc_set_ui_ui (b, 4, 3, MPC_RNDNN);
  mpc_set_ui_ui (c, 1, 2, MPC_RNDNN);
  inex = mpc_div (q, b, c, MPC_RNDNN);
  mpc_set_si_si (b, 2, -1, MPC_RNDNN);
  if (mpc_cmp (q, b) || inex != 0)
    {
      printf ("mpc_div failed for (4+I*3)/(1+I*2)\n");
      exit (1);
    }

  /* pure real dividend BUG-20080923 */
  mpc_set_prec (b, 4);
  mpc_set_prec (c, 4);
  mpc_set_prec (q, 4);

  mpc_set_si_si (b, -3, 0, MPC_RNDNN);
  mpc_div_2exp (b, b, 206, MPC_RNDNN);
  mpc_set_si_si (c, -1, -5, MPC_RNDNN);
  mpfr_div_2ui (MPC_RE (c), MPC_RE (c), 733, GMP_RNDN);
  mpfr_div_2ui (MPC_IM (c), MPC_IM (c), 1750, GMP_RNDN);
  inex = mpc_div (q, b, c, MPC_RNDNZ);
  mpc_set_si_si (b, 3, -7, MPC_RNDNN);
  mpfr_mul_2ui (MPC_RE (b), MPC_RE (b), 527, GMP_RNDN);
  mpfr_div_2ui (MPC_IM (b), MPC_IM (b), 489, GMP_RNDN);

  if (mpc_cmp (q, b))
    {
      printf ("mpc_div failed for -3p-206/(-1p-733 -I* 5p-1750)\n");
      exit (1);
    }

  mpc_clear (b);
  mpc_clear (c);
  mpc_clear (q);
}


int
main (void)
{
  mpc_t b, c, q, q_ref;
  int inex, i;
  mp_prec_t prec;
  mp_rnd_t rnd_re, rnd_im;
  mpc_rnd_t rnd;

  check_regular ();

  mpc_init (b);
  mpc_init (c);
  mpc_init (q);
  mpc_init (q_ref);

  for (prec = 2; prec < 1000; prec++)
    {
      const size_t s = 1 + (prec-1)/BITS_PER_MP_LIMB;

      mpc_set_prec (b, prec);
      mpc_set_prec (c, prec);
      mpc_set_prec (q, prec);
      mpc_set_prec (q_ref, prec);

      for (i = 0; i < (int) (1000/prec); i++)
        {
          mpc_random2 (b, s, 0);
          /* generate a non-zero divisor */
          do
            {
              mpc_random2 (c, s, 0);
            }
          while (mpfr_sgn (MPC_RE(c)) == 0 && mpfr_sgn (MPC_IM(c)) == 0);

          for (rnd_re = (mp_rnd_t)0; rnd_re < (mp_rnd_t)4; ++rnd_re)
            for (rnd_im = (mp_rnd_t)0; rnd_im < (mp_rnd_t)4; ++rnd_im)
              {
                rnd = RNDC(rnd_re, rnd_im);
                mpc_div     (q,     b, c, rnd);
                mpc_div_ref (q_ref, b, c, rnd);
                if (mpc_cmp (q, q_ref))
                  {
                    fprintf (stderr, "mpc_div and mpc_div_ref differ"
                             "for prec=%lu rnd=(%s,%s)\n", prec,
                             mpfr_print_rnd_mode (rnd_re),
                             mpfr_print_rnd_mode (rnd_im));
                    fprintf (stderr, "b=");
                    ERR(b);
                    fprintf (stderr, "c=");
                    ERR(c);
                    fprintf (stderr, "q=");
                    ERR(q);
                    fprintf (stderr, "q_ref=");
                    ERR(q_ref);
                    exit (1);
                  }
              }
        }

    }

  mpc_clear (b);
  mpc_clear (c);
  mpc_clear (q);
  mpc_clear (q_ref);

  return 0;
}
