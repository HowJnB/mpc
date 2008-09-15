/* tfr_div -- test file for mpc_div.

Copyright (C) 2008 Philippe Th\'eveny

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
#include "random.c"

static void
random (void)
{
  mpc_t b, c, q, q_ref;
  mp_prec_t prec;
  mp_rnd_t rnd_re, rnd_im;
  mpc_rnd_t rnd;

  mpc_init (b);
  mpc_init (c);
  mpc_init (q);
  mpc_init (q_ref);

  for (prec = 2; prec < 1024; prec++)
    {
      mpc_set_prec (b, prec);
      mpc_set_prec (c, prec);
      mpc_set_prec (q, prec);
      mpc_set_prec (q_ref, prec);

      test_default_random (c, -8192, 8192, 1);
      test_default_random (b, -65535, 65535, 1);
      mpfr_set_ui (MPC_IM (b), 0, GMP_RNDN);

      for (rnd_re = (mp_rnd_t)0; rnd_re < (mp_rnd_t)4; ++rnd_re)
        for (rnd_im = (mp_rnd_t)0; rnd_im < (mp_rnd_t)4; ++rnd_im)
          {
            rnd = RNDC(rnd_re, rnd_im);
            mpc_fr_div  (q, MPC_RE (b), c, rnd);
            mpc_div (q_ref, b, c, rnd);
            if (mpc_cmp (q, q_ref))
              {
                printf ("mpc_fr_div and mpc_div differ for "
                        "prec=%lu rnd=(%s,%s)\n", prec,
                        mpfr_print_rnd_mode (rnd_re),
                        mpfr_print_rnd_mode (rnd_im));
                OUT (b);
                OUT (c);
                OUT (q);
                OUT (q_ref);
                exit (1);
              }
          }
    }

  mpc_clear (b);
  mpc_clear (c);
  mpc_clear (q);
  mpc_clear (q_ref);
}

int
main (void)
{
  test_start();

  random ();

  test_end ();
  return 0;
}
