/* mpc_mul -- Multiply two complex numbers.

Copyright (C) 2002, 2004 Andreas Enge, Paul Zimmermann

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

#include "gmp.h"
#include "mpfr.h"
#include "mpc.h"
#include "mpc-impl.h"

/* return inex such that MPC_INEX_RE(inex) = -1, 0, 1
                         MPC_INEX_IM(inex) = -1, 0, 1
   depending on the signs of the real/imaginary parts of the result
*/
int
mpc_mul (mpc_ptr a, mpc_srcptr b, mpc_srcptr c, mpc_rnd_t rnd)
{
   int inex_re, inex_im;
   int overlap;
   mpc_t result;
   
   /* first check for real multiplication */
   if (MPFR_IS_ZERO(MPC_IM(b))) /* b * (x+i*y) = b*x + i *(b*y) */
   {
      /* first start with imaginary part in case a=b */
      inex_im = mpfr_mul (MPC_IM(a), MPC_RE(b), MPC_IM(c), MPC_RND_IM(rnd));
      inex_re = mpfr_mul (MPC_RE(a), MPC_RE(b), MPC_RE(c), MPC_RND_RE(rnd));
      return MPC_INEX(inex_re, inex_im);
   }
   if (MPFR_IS_ZERO(MPC_IM(c)))
   {
      inex_im = mpfr_mul (MPC_IM(a), MPC_RE(c), MPC_IM(b), MPC_RND_IM(rnd));
      inex_re = mpfr_mul (MPC_RE(a), MPC_RE(c), MPC_RE(b), MPC_RND_RE(rnd));
      return MPC_INEX(inex_re, inex_im);
   }
   /* check for purely complex multiplication */
   if (MPFR_IS_ZERO(MPC_RE(b))) /* i*b * (x+i*y) = -b*y + i*(b*x) */
   {
      overlap = (a == b || a == c);
      if (overlap)
         mpc_init3 (result, MPFR_PREC (MPC_RE (a)),
               MPFR_PREC (MPC_IM (a)));
      else
         result [0] = a [0];
      inex_re = -mpfr_mul (MPC_RE(result), MPC_IM(b), MPC_IM(c),
                           INV_RND(MPC_RND_RE(rnd)));
      mpfr_neg (MPC_RE (result), MPC_RE (result), GMP_RNDN);
      inex_im = mpfr_mul (MPC_IM(result), MPC_IM(b), MPC_RE(c),
                          MPC_RND_IM(rnd));
      mpc_set (a, result, MPC_RNDNN);
      if (overlap)
         mpc_clear (result);
      return MPC_INEX(inex_re, inex_im);
   }
   if (MPFR_IS_ZERO(MPC_RE(c)))
   {
      overlap = (a == b || a == c);
      if (overlap)
         mpc_init3 (result, MPFR_PREC (MPC_RE (a)),
               MPFR_PREC (MPC_IM (a)));
      else
         result [0] = a [0];
      inex_re = -mpfr_mul (MPC_RE(result), MPC_IM(c), MPC_IM(b),
                           INV_RND(MPC_RND_RE(rnd)));
      mpfr_neg (MPC_RE (result), MPC_RE (result), GMP_RNDN);
      inex_im = mpfr_mul (MPC_IM(result), MPC_IM(c), MPC_RE(b),
                          MPC_RND_IM(rnd));
      mpc_set (a, result, MPC_RNDNN);
      if (overlap)
         mpc_clear (result);
      return MPC_INEX(inex_re, inex_im);
   }
   
   /* If the real and imaginary part of one argument have a very different */
   /* exponent, it is not reasonable to use Karatsuba multiplication.      */
   if (   SAFE_ABS (mp_exp_t, MPFR_EXP (MPC_RE (b)) - MPFR_EXP (MPC_IM (b)))
          > MPC_MAX_PREC (b) / 2
       || SAFE_ABS (mp_exp_t, MPFR_EXP (MPC_RE (c)) - MPFR_EXP (MPC_IM (c)))
          > MPC_MAX_PREC (c) / 2)
      return mpc_mul_naive (a, b, c, rnd);
   else
      return ((MPC_MAX_PREC(a)
              <= (mp_prec_t) MUL_KARATSUBA_THRESHOLD * BITS_PER_MP_LIMB)
              ? mpc_mul_naive : mpc_mul_karatsuba) (a, b, c, rnd);
}

int
mpc_mul_naive (mpc_ptr a, mpc_srcptr b, mpc_srcptr c, mpc_rnd_t rnd)
{
  int overlap, inex_re, inex_im;
  mpfr_t u, v, t;
  mp_prec_t prec;

  overlap = (a == b) || (a == c);

  prec = MPC_MAX_PREC(b) + MPC_MAX_PREC(c);

  mpfr_init2 (u, prec);
  mpfr_init2 (v, prec);

  /* Re(a) = Re(b)*Re(c) - Im(b)*Im(c) */
  mpfr_mul (u, MPC_RE(b), MPC_RE(c), GMP_RNDN); /* exact */
  mpfr_mul (v, MPC_IM(b), MPC_IM(c), GMP_RNDN); /* exact */

  if (overlap)
    {
      mpfr_init2 (t, MPFR_PREC(MPC_RE(a)));
      inex_re = mpfr_sub (t, u, v, MPC_RND_RE(rnd));
    }
  else
    inex_re = mpfr_sub (MPC_RE(a), u, v, MPC_RND_RE(rnd));

  /* Im(a) = Re(b)*Im(c) + Im(b)*Re(c) */
  mpfr_mul (u, MPC_RE(b), MPC_IM(c), GMP_RNDN); /* exact */
  if (b == c) /* square case */
    inex_im = mpfr_mul_2exp (MPC_IM(a), u, 1, MPC_RND_IM(rnd));
  else
    {
      mpfr_mul (v, MPC_IM(b), MPC_RE(c), GMP_RNDN); /* exact */
      inex_im = mpfr_add (MPC_IM(a), u, v, MPC_RND_IM(rnd));
    }

  mpfr_clear (u);
  mpfr_clear (v);

  if (overlap)
    {
      mpfr_set (MPC_RE(a), t, GMP_RNDN); /* exact */
      mpfr_clear (t);
    }

  return MPC_INEX(inex_re, inex_im);
}


/* Karatsuba multiplication, with 3 real multiplies */
int
mpc_mul_karatsuba (mpc_ptr rop, mpc_srcptr op1, mpc_srcptr op2, mpc_rnd_t rnd)
{
  mpfr_srcptr a, b, c, d;
  int mul_i, ok, inexact, mul_a, mul_c, inex_re, inex_im, sign_x, sign_u;
  mpfr_t u, v, w, x;
  mp_prec_t prec, prec_re, prec_u, prec_v, prec_w;
  mp_rnd_t rnd_re, rnd_u, rnd_x;
  int overlap;
     /* true if rop == op1 or rop == op2 */
  mpc_t result;
     /* overlap is quite difficult to handle, because we have to tentatively
        round the variable u in the end to either the real or the imaginary
        part of rop (it is not possible to tell now whether the real or
        imaginary part is used). If this fails, we have to start again and
        need the correct values of op1 and op2.
        So we just create a new variable for the result in this case. */

  overlap = (rop == op1) || (rop == op2);
  if (overlap)
     mpc_init3 (result, MPFR_PREC (MPC_RE (rop)),
                        MPFR_PREC (MPC_IM (rop)));
  else
     result [0] = rop [0];
  
  a = MPC_RE(op1);
  b = MPC_IM(op1);
  c = MPC_RE(op2);
  d = MPC_IM(op2);

  /* (a + i*b) * (c + i*d) = [ac - bd] + i*[ad + bc] */

  mul_i = 0; /* number of multiplications by i */
  mul_a = 1; /* implicit factor for a */
  mul_c = 1; /* implicit factor for c */

  if (MPFR_CMP_ABS (a, b) < 0)
    {
      SWAP(a, b);
      mul_i ++;
      mul_a = -1; /* consider i * (a+i*b) = -b + i*a */
    }

  if (MPFR_CMP_ABS (c, d) < 0)
    {
      SWAP(c, d);
      mul_i ++;
      mul_c = -1; /* consider -d + i*c instead of c + i*d */
    }

  /* find the precision and rounding mode for the new real part.
   */
  if (mul_i % 2)
    {
      prec_re = MPFR_PREC(MPC_IM(rop));
      rnd_re = MPC_RND_IM(rnd);
    }
  else /* mul_i = 0 or 2 */
    {
      prec_re = MPFR_PREC(MPC_RE(rop));
      rnd_re = MPC_RND_RE(rnd);
    }

  if (mul_i)
    rnd_re = INV_RND(rnd_re);

  /* now |a| >= |b| and |c| >= |d| */
  prec = MPC_MAX_PREC(rop);

  mpfr_init2 (u, 2);
  mpfr_init2 (v, prec_v = MPFR_PREC(a) + MPFR_PREC(d));
  mpfr_init2 (w, prec_w = MPFR_PREC(b) + MPFR_PREC(c));
  mpfr_init2 (x, 2);

  mpfr_mul (v, a, d, GMP_RNDN); /* exact */
  if (mul_a == -1)
    mpfr_neg (v, v, GMP_RNDN);

  mpfr_mul (w, b, c, GMP_RNDN); /* exact */
  if (mul_c == -1)
    mpfr_neg (w, w, GMP_RNDN);

  /* compute sign(v-w) */
  sign_x = MPFR_CMP_ABS (v, w);
  if (sign_x > 0)
    sign_x = 2 * mpfr_sgn (v) - mpfr_sgn (w);
  else if (sign_x == 0)
    sign_x = mpfr_sgn (v) - mpfr_sgn (w);
  else
    sign_x = mpfr_sgn (v) - 2 * mpfr_sgn (w);

   sign_u = mul_a * mpfr_sgn (a) * mul_c * mpfr_sgn (c);

   if (sign_x * sign_u < 0)
    {  /* swap inputs */
      SWAP (a, c);
      SWAP (b, d);
      mpfr_swap (v, w);
      { int tmp; tmp = mul_a; mul_a = mul_c; mul_c = tmp; }
      sign_x = - sign_x;
    }

   /* now sign_x * sign_u >= 0 */
   do
   {
      do
      {
         /* the following should give failures with prob. <= 1/prec */
         prec += mpc_ceil_log2 (prec) + 3;

         mpfr_set_prec (u, prec_u = prec);
         mpfr_set_prec (x, prec);

         /* first compute away(b +/- a) and store it in u */
         rnd_u = (mpfr_sgn (a) > 0) ? GMP_RNDU : GMP_RNDD;
         if (mul_a == -1)
           rnd_u = INV_RND(rnd_u);
         inexact = ((mul_a == -1) ? mpfr_sub : mpfr_add) (u, b, a, rnd_u);

         /* then compute away(+/-c - d) and store it in x */
         rnd_x = (mpfr_sgn (c) > 0) ? GMP_RNDU : GMP_RNDD;
         inexact |= ((mul_c == -1) ? mpfr_add : mpfr_sub) (x, c, d, rnd_x);
         if (mul_c == -1)
           mpfr_neg (x, x, GMP_RNDN);

	 if (inexact == 0)
	   mpfr_prec_round (u, prec_u = 2 * prec, GMP_RNDN);

         /* compute away(u*x) and store it in u */
         rnd_u = (sign_u > 0) ? GMP_RNDU : GMP_RNDD;
         inexact |= mpfr_mul (u, u, x, rnd_u); /* (a+b)*(c-d) */

	 /* if all computations are exact up to here, it may be that
	    the real part is exact, thus we need if possible to 
	    compute v - w exactly */
	 if (inexact == 0)
	   {
	     mp_prec_t prec_x;
             if (MPFR_IS_ZERO(v))
               prec_x = prec_w;
             else if (MPFR_IS_ZERO(w))
               prec_x = prec_v;
             else
               {
                 prec_x = (MPFR_EXP(v) > MPFR_EXP(w)) ? MPFR_EXP(v) - MPFR_EXP(w)
                   : MPFR_EXP(w) - MPFR_EXP(v);
                 prec_x += MAX(prec_v, prec_w) + 1;
               }
           /* +1 is necessary for a potential carry */
	     /* ensure we do not use a too large precision */
	     if (prec_x > prec_u)
	       prec_x = prec_u;
	     if (prec_x > prec)
	       mpfr_prec_round (x, prec_x, GMP_RNDN);
	   }

         inexact |= mpfr_sub (x, v, w, rnd_u); /* ad - bc */

         /* in case u=0, ensure that rnd_u rounds x away from zero */
         if (mpfr_sgn (u) == 0)
           rnd_u = (mpfr_sgn (x) > 0) ? GMP_RNDU : GMP_RNDD;
         inexact |= mpfr_add (u, u, x, rnd_u); /* ac - bd */

         ok = inexact == 0 ||
           mpfr_can_round (u, prec_u - 3, rnd_u, rnd_re, prec_re);
      }
      while (ok == 0);

      if (mul_i == 0)
      {
         inex_re = mpfr_set (MPC_RE(result), u, MPC_RND_RE(rnd));
         if (inex_re == 0)
         {
            inex_re = inexact; /* u is rounded away from 0 */
            inex_im = mpfr_add (MPC_IM(result), v, w, MPC_RND_IM(rnd));
         }
         else if (MPC_RND_RE (rnd) == GMP_RNDN && inexact != 0
            && MPC_INEX_POS (inex_re) == MPC_INEX_POS (-MPFR_SIGN (u))
            && !(MPFR_SIGN (u) > 0
                 ? mpfr_can_round (u, prec - 3, rnd_u, GMP_RNDU, prec_re)
                 : mpfr_can_round (u, prec - 3, rnd_u, GMP_RNDD, prec_re)))
            /* rounding did work, but we do not know whether we are larger
               or smaller than the correct result */
         {
            inex_im = 0; /* just to avoid the gcc warning message */
            ok = 0;
         }
         else
            inex_im = mpfr_add (MPC_IM(result), v, w, MPC_RND_IM(rnd));
      }
      else if (mul_i == 1) /* (x+i*y)/i = y - i*x */
      {
         inex_im = mpfr_neg (MPC_IM(result), u, MPC_RND_IM(rnd));
         if (inex_im == 0)
         {
            inex_im = -inexact; /* u is rounded away from 0 */
            inex_re = mpfr_add (MPC_RE(result), v, w, MPC_RND_RE(rnd));
         }
         else if (MPC_RND_IM (rnd) == GMP_RNDN && inexact != 0
            && MPC_INEX_POS (inex_im) == MPC_INEX_POS (MPFR_SIGN (u))
            && !(MPFR_SIGN (u) > 0
                 ? mpfr_can_round (u, prec - 3, rnd_u, GMP_RNDU, prec_re)
                 : mpfr_can_round (u, prec - 3, rnd_u, GMP_RNDD, prec_re)))
            /* rounding did work, but we do not know whether we are larger
               or smaller than the correct result */
         {
            inex_re = 0; /* just to avoid the gcc warning message */
            ok = 0;
         }
         else
            inex_re = mpfr_add (MPC_RE(result), v, w, MPC_RND_RE(rnd));

      }
      else /* mul_i = 2, z/i^2 = -z */
      {
         inex_re = mpfr_neg (MPC_RE(result), u, MPC_RND_RE(rnd));
         if (inex_re == 0)
         {
            inex_re = -inexact; /* u is rounded away from 0 */
            inex_im = -mpfr_add (MPC_IM(result), v, w,
                                 INV_RND(MPC_RND_IM(rnd)));
            mpfr_neg (MPC_IM(result), MPC_IM(result), MPC_RND_IM(rnd));
         }
         else if (MPC_RND_RE (rnd) == GMP_RNDN && inexact != 0
            && MPC_INEX_POS (inex_re) == MPC_INEX_POS (MPFR_SIGN (u))
            && !(MPFR_SIGN (u) > 0
                 ? mpfr_can_round (u, prec - 3, rnd_u, GMP_RNDU, prec_re)
                 : mpfr_can_round (u, prec - 3, rnd_u, GMP_RNDD, prec_re)))
            /* rounding did work, but we do not know whether we are larger
               or smaller than the correct result */
         {
            inex_im = 0; /* just to avoid the gcc warning message */
            ok = 0;
         }
         else
         {
            inex_im = -mpfr_add (MPC_IM(result), v, w,
                                 INV_RND(MPC_RND_IM(rnd)));
            mpfr_neg (MPC_IM(result), MPC_IM(result), MPC_RND_IM(rnd));
         }
      }
   }
   while (ok == 0);
   
   mpc_set (rop, result, MPC_RNDNN);

   mpfr_clear (u);
   mpfr_clear (v);
   mpfr_clear (w);
   mpfr_clear (x);
   if (overlap)
      mpc_clear (result);
     
   return MPC_INEX(inex_re, inex_im);
}
