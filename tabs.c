/* tsqr -- test file for mpc_sqr.

Copyright (C) 2005 Andreas Enge

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

#define SIGN(x) (((x) == 0 ? 0 : ((x) > 0 ? 1 : -1)))

void cmpabs (mpc_srcptr, mpfr_rnd_t);
void testabs (mpc_ptr);


void cmpabs (mpc_srcptr x, mpfr_rnd_t rnd)
   /* computes the absolute value of x with the two functions defined in   */
   /* abs.c with rounding mode rnd and compares the results and return     */
   /* values. We only consider cases where all precisions are identical.   */
   /* We also compute the result with four times the precision and check   */
   /* whether the rounding is correct. Error reports in this part of the   */
   /* algorithm might still be wrong, though, since there are two          */
   /* consecutive roundings.                                               */
{
   mpfr_t res1, res2, res3;
   int    inexact1, inexact2;

   mpfr_init2 (res1, MPC_MAX_PREC (x));
   mpfr_init2 (res2, MPC_MAX_PREC (x));
   mpfr_init2 (res3, 4 * MPC_MAX_PREC (x));

   inexact1 = mpc_abs (res1, x, rnd);
   inexact2 = mpc_abs_basic (res2, x, rnd);

   if (mpfr_cmp (res1, res2))
   {
      fprintf (stderr, "abs and abs_basic differ for rnd=%s \nx=",
               mpfr_print_rnd_mode(rnd));
      mpc_out_str (stderr, 2, 0, x, MPC_RNDNN);
      fprintf (stderr, "\nabs       gives ");
      mpfr_out_str (stderr, 2, 0, res1, GMP_RNDN);
      fprintf (stderr, "\nabs_basic gives ");
      mpfr_out_str (stderr, 2, 0, res2, GMP_RNDN);
      fprintf (stderr, "\n");
      exit (1);
   }
   if (SIGN (inexact1) != SIGN (inexact2))
   {
     fprintf (stderr, "The return values of abs and abs_basic differ for rnd=%s \nx=  ",
              mpfr_print_rnd_mode(rnd));
     mpc_out_str (stderr, 2, 0, x, MPC_RNDNN);
     fprintf (stderr, "\n|x| = ");
     mpfr_out_str (stderr, 2, 0, res1, GMP_RNDN);
     fprintf (stderr, "\nabs       gives %i", inexact1);
     fprintf (stderr, "\nabs_basic gives %i", inexact2);
     fprintf (stderr, "\n");
     exit (1);
   }
   if (   (rnd == GMP_RNDU && (inexact1 < 0 || inexact2 < 0))
       || ((rnd == GMP_RNDD || rnd == GMP_RNDZ) 
            && (inexact1 > 0 || inexact2 > 0)))
      {
        fprintf (stderr, "Incorrect return value in abs or abs_basic for rnd=%s \nx=  ",
                 mpfr_print_rnd_mode(rnd));
        mpc_out_str (stderr, 2, 0, x, MPC_RNDNN);
        fprintf (stderr, "\n|x| = ");
        mpfr_out_str (stderr, 2, 0, res1, GMP_RNDN);
        fprintf (stderr, "\nabs       gives %i", inexact1);
        fprintf (stderr, "\nabs_basic gives %i", inexact2);
        fprintf (stderr, "\n");
        exit (1);
     }
      

   mpc_abs (res3, x, rnd);
   mpfr_set (res2, res3, rnd);
   if (mpfr_cmp (res1, res2))
   {
      fprintf (stderr, "rounding in abs might be incorrect for rnd=%s \nx=",
               mpfr_print_rnd_mode(rnd));
      mpc_out_str (stderr, 2, 0, x, MPC_RNDNN);
      fprintf (stderr, "\nabs                     gives ");
      mpfr_out_str (stderr, 2, 0, res1, GMP_RNDN);
      fprintf (stderr, "\nabs quadruple precision gives ");
      mpfr_out_str (stderr, 2, 0, res3, GMP_RNDN);
      fprintf (stderr, "\nand is rounded to                 ");
      mpfr_out_str (stderr, 2, 0, res2, GMP_RNDN);
      fprintf (stderr, "\n");
      exit (1);
  }

   mpfr_clear (res1);
   mpfr_clear (res2);
   mpfr_clear (res3);
}


void testabs (mpc_ptr x)
   /* executes the test for all rounding modes and all possible signs for */
   /* the real and the imaginary part                                     */
      
{
   mpfr_rnd_t rnd;
   int i, j;
   
   for (i = 0; i < 2; i++)
   {
      mpfr_neg (MPC_RE (x), MPC_RE (x), GMP_RNDN);
      for (j = 0; j < 2; j++)
      {
         mpfr_neg (MPC_IM (x), MPC_IM (x), GMP_RNDN);
         for (rnd = 0; rnd < 4; rnd++)
            cmpabs (x, rnd);
      }
   }
}


int main()
{
  mpc_t  x;
  mpfr_t eps;
  mp_prec_t prec;

  mpc_init2 (x, 1000);
  mpfr_init2 (eps, 10);

  /* The most interesting cases arise when real and imaginary part differ */
  /* much, since only then abs and abs_basic actually differ.             */
  /* Check 1, 1+epsilon, (1+epsilon) + epsilon*i and the same values with */
  /* real and imaginary part swapped for epsilon small powers of 2.       */
  mpfr_set_ui (eps, 1, GMP_RNDN);
  mpfr_div_2ui (eps, eps, 200, GMP_RNDN);
  for (prec = 2; prec < 1000; prec += 222)
  {
     mpc_set_prec (x, prec);
     mpc_set_ui_ui (x, 1, 0, MPC_RNDNN);
     testabs (x);
     mpfr_set (MPC_IM (x), eps, GMP_RNDN);
     testabs (x);
     mpfr_add_ui (MPC_RE (x), eps, 1, GMP_RNDN);
     testabs (x);
     mpc_set_ui_ui (x, 0, 1, MPC_RNDNN);
     testabs (x);
     mpfr_set (MPC_RE (x), eps, GMP_RNDN);
     testabs (x);
     mpfr_add_ui (MPC_IM (x), eps, 1, GMP_RNDN);
     testabs (x);
  }
  
  mpc_clear (x);

  return 0;
}
