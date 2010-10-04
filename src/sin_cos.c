/* mpc_sin_cos -- sine and cosine of a complex number.

Copyright (C) 2010 Andreas Enge

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

#include "mpc-impl.h"

int
mpc_sin_cos (mpc_ptr rop_sin, mpc_ptr rop_cos, mpc_srcptr op,
   mpc_rnd_t rnd_sin, mpc_rnd_t rnd_cos)
{
   int inex_sin, inex_cos;

   if (!mpc_fin_p (op)
       || mpfr_zero_p (MPC_RE (op)) || mpfr_zero_p (MPC_IM (op))) {
      inex_sin = mpc_sin (rop_sin, op, rnd_sin);
      inex_cos = mpc_cos (rop_cos, op, rnd_cos);
   }
   else {
      mpfr_t s, c, sh, ch, sch, csh;
      mpfr_prec_t prec;
      int ok = 0;
      int inex_re, inex_im;

      prec = MPC_MAX (MPC_MAX_PREC (rop_sin), MPC_MAX_PREC (rop_cos));

      mpfr_init2 (s, 2);
      mpfr_init2 (c, 2);
      mpfr_init2 (sh, 2);
      mpfr_init2 (ch, 2);
      mpfr_init2 (sch, 2);
      mpfr_init2 (csh, 2);

      do {
         prec += mpc_ceil_log2 (prec) + 5;

         mpfr_set_prec (s, prec);
         mpfr_set_prec (c, prec);
         mpfr_set_prec (sh, prec);
         mpfr_set_prec (ch, prec);
         mpfr_set_prec (sch, prec);
         mpfr_set_prec (csh, prec);

         mpfr_sin_cos (s, c, MPC_RE(op), GMP_RNDN);
         mpfr_sinh_cosh (sh, ch, MPC_IM(op), GMP_RNDN);

         /* real part of sine */
         mpfr_mul (sch, s, ch, GMP_RNDN);
         ok = (!mpfr_number_p (sch))
               || mpfr_can_round (sch, prec - 2, GMP_RNDN, GMP_RNDZ,
                     MPC_PREC_RE (rop_sin)
                     + (MPC_RND_RE (rnd_sin) == GMP_RNDN));

         if (ok) {
            /* imaginary part of sine */
            mpfr_mul (csh, c, sh, GMP_RNDN);
            ok = (!mpfr_number_p (csh))
                  || mpfr_can_round (csh, prec - 2, GMP_RNDN, GMP_RNDZ,
                        MPC_PREC_IM (rop_sin)
                        + (MPC_RND_IM (rnd_sin) == GMP_RNDN));

            if (ok) {
               /* real part of cosine */
               mpfr_mul (c, c, ch, GMP_RNDN);
               ok = (!mpfr_number_p (c))
                     || mpfr_can_round (c, prec - 2, GMP_RNDN, GMP_RNDZ,
                           MPC_PREC_RE (rop_cos)
                           + (MPC_RND_RE (rnd_cos) == GMP_RNDN));

               if (ok) {
                  /* imaginary part of cosine */
                  mpfr_mul (s, s, sh, GMP_RNDN);
                  mpfr_neg (s, s, GMP_RNDN);
                  ok = (!mpfr_number_p (s))
                        || mpfr_can_round (s, prec - 2, GMP_RNDN, GMP_RNDZ,
                              MPC_PREC_IM (rop_cos)
                              + (MPC_RND_IM (rnd_cos) == GMP_RNDN));
               }
            }
         }
      } while (ok == 0);

      inex_re = mpfr_set (MPC_RE (rop_sin), sch, MPC_RND_RE (rnd_sin));
      if (mpfr_inf_p (sch))
         inex_re = mpfr_sgn (sch);
      inex_im = mpfr_set (MPC_IM (rop_sin), csh, MPC_RND_IM (rnd_sin));
      if (mpfr_inf_p (csh))
         inex_im = mpfr_sgn (csh);
      inex_sin = MPC_INEX (inex_re, inex_im);
      inex_re = mpfr_set (MPC_RE (rop_cos), c, MPC_RND_RE (rnd_cos));
      if (mpfr_inf_p (c))
         inex_re = mpfr_sgn (c);
      inex_im = mpfr_set (MPC_IM (rop_cos), s, MPC_RND_IM (rnd_cos));
      if (mpfr_inf_p (s))
         inex_im = mpfr_sgn (s);
      inex_cos = MPC_INEX (inex_re, inex_im);

      mpfr_clear (s);
      mpfr_clear (c);
      mpfr_clear (sh);
      mpfr_clear (ch);
      mpfr_clear (sch);
      mpfr_clear (csh);
   }

   return (inex_sin == 0 && inex_cos == 0 ? 0 : 1);
}
