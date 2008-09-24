/* test file for mpc.

Copyright (C) 2002, 2004, 2007, 2008 Andreas Enge, Paul Zimmermann, Philippe Th\'eveny

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

#define PRINT(x) {}

/* Crude tests for some functions. */
/* TODO: use tgeneric instead and remove this. */
int
main (void)
{
  mpc_t x, z;
  mp_prec_t prec, pr, pi;
  mpfr_t f, g;
  FILE *file;
  const char *filename = "mpc_test";

  mpc_init (x);
  mpc_init3 (z, 2, 2);
  mpfr_init (f);
  mpfr_init (g);

  for (prec = 2; prec <= 1000; prec++)
    {
      mpc_set_prec (x, prec);
      mpfr_set_prec (f, prec);
      mpfr_set_prec (g, prec);

      PRINT ("Testing mpc_get_prec and mpc_get_prec2\n");
      mpfr_set_prec (MPC_RE (z), prec);
      mpfr_set_prec (MPC_IM (z), prec + 1);
      if (mpc_get_prec (z) != 0)
        {
          fprintf (stderr, "Error in mpc_get_prec for prec (re) = %li, prec (im) = %li\n",
                   prec, prec + 1);
          exit (1);
        }
      mpc_get_prec2 (&pr, &pi, z);
      if (pr != prec || pi != prec + 1)
        {
          fprintf (stderr, "Error in mpc_get_prec2 for prec (re) = %li, prec (im) = %li\n",
                   prec, prec + 1);
          exit (1);
        }
      mpc_set_prec (z, prec);
      if (mpc_get_prec (z) != prec)
        {
          fprintf (stderr, "Error in mpc_get_prec for prec = %li\n", prec);
          exit (1);
        }

      PRINT ("Testing mpc_inp_str\n");
      if (!(file = fopen (filename, "w")))
        {
          fprintf (stderr, "Could not open file %s\n", filename);
          exit (1);
        };
      fprintf (file, "1 +I*1\n");
      fclose (file);
      if (!(file = fopen (filename, "r")))
        {
          fprintf (stderr, "Could not open file %s\n", filename);
          exit (1);
        };
      if (mpc_inp_str (z, file, 10, MPC_RNDUZ) == 0)
        {
          fprintf (stderr, "mpc_inp_str cannot correctly re-read number "
                   "in file %s\n", filename);
          exit (1);
        }
      fclose (file);
      mpc_set_si_si (x, 1, 1, MPC_RNDNN);
      mpfr_clear_flags (); /* mpc_cmp set erange flag when an operand is
                              a NaN */
      if (mpc_cmp (z, x) != 0 || mpfr_erangeflag_p())
        {
          fprintf (stderr, "inp_str o out_str <> Id\n");
          mpc_out_str (stderr, 10, 0, z, MPC_RNDNN);
          fprintf (stderr, "\n");
          exit (1);
        }

      PRINT ("Testing mpc_out_str\n");
      if (!(file = fopen (filename, "w")))
        {
          fprintf (stderr, "Could not open file %s\n", filename);
          exit (1);
        };
      mpc_out_str (file, 10, 0, z, MPC_RNDNN);
      fclose (file);

      PRINT ("Testing mpc_set\n");
      mpc_set (z, x, MPC_RNDNN);

      PRINT ("Testing mpc_set_d\n");
      mpc_set_d (z, 1.23456789, MPC_RNDNN);

      PRINT ("Testing mpc_set_d_d\n");
      mpc_set_d_d (z, 1.23456789, 1.23456789, MPC_RNDNN);

      PRINT ("Testing mpc_set_default_prec\n");
      mpc_set_default_prec (prec);

      PRINT ("Testing mpc_get_default_prec\n");
      if (mpc_get_default_prec () != prec)
        abort ();

      PRINT ("Testing mpc_set_ui\n");
      mpc_set_ui (z, 17, MPC_RNDNN);

      PRINT ("Testing mpc_set_ui_fr\n");
      mpc_set_ui_fr (z, 17, f, MPC_RNDNN);

      PRINT ("Testing mpc_set_ui_ui\n");
      mpc_set_ui_ui (z, 17, 17, MPC_RNDNN);
    }


  mpc_clear (x);
  mpc_clear (z);
  mpfr_clear (f);
  mpfr_clear (g);

  return 0;
}
