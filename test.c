/* test file for mpc.

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

#define PRINT(x) {}
#define OUT(x) { \
   mpfr_print_binary (MPC_RE(x)); \
   printf ("+I*"); \
   mpfr_print_binary (MPC_IM(x)); \
   printf ("\n"); \
}

int
main()
{
  mpc_t x, y, z;
  mp_prec_t prec;
  mpfr_t f, g;
  FILE *file;
  int nread;
  const char *filename = "mpc_test";

  mpc_init (x);
  mpc_init2 (y, 2);
  mpc_init3 (z, 2, 2);
  mpfr_init (f);
  mpfr_init (g);

  for (prec = 2; prec <= 1000; prec++)
    {
      mpc_set_prec (x, prec);
      mpc_set_prec (y, prec);
      mpc_set_prec (z, prec);
      mpfr_set_prec (f, prec);
      mpfr_set_prec (g, prec);

      mpc_set_ui (x, 1, MPC_RNDNN);
      mpc_mul_2exp (x, x, prec, MPC_RNDNN);
      mpc_set_ui (y, 1, MPC_RNDNN);
      if (mpc_add (z, x, y, MPC_RNDNN) == 0)
        {
          fprintf (stderr, "Error in mpc_add: 2^(-prec)+1 cannot be exact\n");
          exit (1);
        }

      mpc_random (x);

      mpc_random2 (y, 1 + (prec - 1) / mp_bits_per_limb, 0);

      PRINT ("Testing mpc_add\n");
      mpc_add (z, x, y, MPC_RNDNN);
      if (mpc_add (z, z, z, MPC_RNDNN))
        {
          fprintf (stderr, "Error in mpc_add: z+z should be exact\n");
          fprintf (stderr, "Precision %li\n", prec);
          exit (1);
        }

      PRINT ("Testing mpc_add_fr\n");
      mpfr_random (f);
      mpc_add_fr (z, x, f, MPC_RNDNZ);
      mpc_set_ui (z, 1, MPC_RNDNN);
      mpfr_set_ui (f, 1, GMP_RNDN);
      if (mpc_add_fr (z, z, f, MPC_RNDNZ))
        {
          fprintf (stderr, "Error in mpc_add_fr: 1+1 should be exact\n");
          exit (1);
        }
      mpc_set_ui (z, 1, MPC_RNDNN);
      mpc_mul_2exp (z, z, prec, MPC_RNDNN);
      if (mpc_add_fr (z, z, f, MPC_RNDNN) == 0)
        {
          fprintf (stderr, "Error in mpc_add_fr: 2^prec+1 cannot be exact\n");
          exit (1);
        }

      PRINT ("Testing mpc_add_ui\n");
      mpc_add_ui (z, x, 17, MPC_RNDNU);
      mpc_set_ui (z, 1, MPC_RNDNN);
      if (mpc_add_ui (z, z, 1, MPC_RNDNZ))
        {
          fprintf (stderr, "Error in mpc_add_ui: 1+1 should be exact\n");
          exit (1);
        }
      mpc_set_ui (z, 1, MPC_RNDNN);
      mpc_mul_2exp (z, z, prec, MPC_RNDNN);
      if (mpc_add_ui (z, z, 1, MPC_RNDNN) == 0)
        {
          fprintf (stderr, "Error in mpc_add_ui: 2^prec+1 cannot be exact\n");
          exit (1);
        }

      PRINT ("Testing mpc_conj\n");
      mpc_conj (z, z, MPC_RNDND);

      PRINT ("Testing mpc_div\n");
      mpc_div (z, x, y, MPC_RNDZN);

      PRINT ("Testing mpc_div_2exp\n");
      mpc_div_2exp (z, x, 1, MPC_RNDZZ);
      
      PRINT ("Testing mpc_div_fr\n");
      mpc_div_fr (z, x, f, MPC_RNDZU);

      PRINT ("Testing mpc_div_ui\n");
      mpc_div_ui (z, x, 17, MPC_RNDZD);

      PRINT ("Testing mpc_inp_str\n");
      if (!(file = fopen (filename, "w")))
      {
         fprintf (stderr, "Could not open file %s\n", filename);
         exit (1);
      };
      fprintf (file, "1+I*1\n");
      fclose (file);
      if (!(file = fopen (filename, "r")))
      {
         fprintf (stderr, "Could not open file %s\n", filename);
         exit (1);
      };
      nread = mpc_inp_str (z, file, 10, MPC_RNDUZ);
      fclose (file);
      mpc_set_si_si (x, 1, 1, MPC_RNDNN);
      if (nread && mpc_cmp (z, x))
        {
          fprintf (stderr, "inp_str o out_str <> Id\n");
          mpc_out_str (stderr, 10, 0, z, MPC_RNDNN);
          fprintf (stderr, "\n");
          exit (1);
        }

      PRINT ("Testing mpc_mul\n");
      mpc_mul (z, x, y, MPC_RNDUU);

      PRINT ("Testing mpc_mul_2exp\n");
      mpc_mul_2exp (z, x, 17, MPC_RNDUD);

      PRINT ("Testing mpc_mul_fr\n");
      mpc_mul_fr (z, x, f, MPC_RNDDN);

      PRINT ("Testing mpc_mul_ui\n");
      mpc_mul_ui (z, x, 17, MPC_RNDDZ);

      PRINT ("Testing mpc_neg\n");
      mpc_neg (z, x, MPC_RNDDU);

      PRINT ("Testing mpc_norm\n");
      mpc_norm (f, x, MPC_RNDDD);

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

      PRINT ("Testing mpc_sub\n");
      mpc_sub (z, x, y, MPC_RNDNN);

      PRINT ("Testing mpc_ui_div\n");
      mpc_ui_div (z, 17, x, MPC_RNDNN);
    }
  

  mpc_clear (x);
  mpc_clear (y);
  mpc_clear (z);
  mpfr_clear (f);
  mpfr_clear (g);

  return 0;
}
