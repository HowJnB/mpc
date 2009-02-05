/* tset.c -- Test file for mpc_set_x and mpc_set_x_x functions.

Copyright (C) 2009 Philippe Th\'eveny

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

#include "config.h"

#if HAVE_INTTYPES_H
# include <inttypes.h> /* for intmax_t */
#else
# if HAVE_STDINT_H
#  include <stdint.h>
# endif
#endif

#include <stdio.h>
#include "mpc.h"
#include "mpc-impl.h"

#define PRINT_ERROR(function_name, precision, a)                \
  do {                                                          \
    printf ("Error in "function_name" for prec = %lu\n",        \
            (unsigned long int) precision);                     \
    OUT(a);                                                     \
    exit (1);                                                   \
  } while (0)
  
int
main (void)
{
  long int lo;
  mpz_t mpz;
  mpq_t mpq;
  mpf_t mpf;
  mpfr_t fr;
  mpc_t x, z;
  mp_prec_t prec;

  mpz_init (mpz);
  mpq_init (mpq);
  mpf_init2 (mpf, 1000);
  mpfr_init2 (fr, 1000);
  mpc_init2 (x, 1000);
  mpc_init2 (z, 1000);

  mpz_set_ui (mpz, 0x4217);
  mpq_set_si (mpq, -1, 0x4321);
  mpf_set_q (mpf, mpq);

  for (prec = 2; prec <= 1000; prec++)
    {
      mpc_set_prec (z, prec);
      mpfr_set_prec (fr, prec);

      lo = -prec;
      
      mpfr_set_d (fr, 1.23456789, GMP_RNDN);

      mpc_set_d (z, 1.23456789, MPC_RNDNN);
      if (mpfr_cmp (MPC_RE(z), fr) != 0 || mpfr_cmp_si (MPC_IM(z), 0) != 0)
        PRINT_ERROR ("mpc_set", prec, z);

      mpc_set_ui (z, prec, MPC_RNDNN);
      if (mpfr_cmp_ui (MPC_RE(z), prec) != 0
          || mpfr_cmp_ui (MPC_IM(z), 0) != 0)
        PRINT_ERROR ("mpc_set_ui", prec, z);

      mpc_set_d_d (z, 1.23456789, 1.23456789, MPC_RNDNN);
      if (mpfr_cmp (MPC_RE(z), fr) != 0 || mpfr_cmp (MPC_IM(z), fr) != 0)
        PRINT_ERROR ("mpc_set_d_d", prec, z);

      mpc_set_si (z, lo, MPC_RNDNN);
      if (mpfr_cmp_si (MPC_RE(z), lo) != 0 || mpfr_cmp_ui (MPC_IM(z), 0) != 0)
        PRINT_ERROR ("mpc_set_si", prec, z);

      mpfr_set_ld (fr, 1.23456789L, GMP_RNDN);

      mpc_set_ld_ld (z, 1.23456789L, 1.23456789L, MPC_RNDNN);
      if (mpfr_cmp (MPC_RE(z), fr) != 0 || mpfr_cmp (MPC_IM(z), fr) != 0)
        PRINT_ERROR ("mpc_set_ld_ld", prec, z);

      mpc_set_ui_ui (z, prec, prec, MPC_RNDNN);
      if (mpfr_cmp_ui (MPC_RE(z), prec) != 0
          || mpfr_cmp_ui (MPC_IM(z), prec) != 0)
        PRINT_ERROR ("mpc_set_ui_ui", prec, z);

      mpc_set_ld (z, 1.23456789L, MPC_RNDNN);
      mpfr_clear_flags ();
      if (mpfr_cmp (MPC_RE(z), fr) != 0
          || mpfr_cmp_ui (MPC_IM(z), 0) != 0
          || mpfr_erangeflag_p())
        PRINT_ERROR ("mpc_set_ld", prec, z);

      mpc_set_prec (x, prec);
      mpfr_set_ui(fr, 1, GMP_RNDN);
      mpfr_div_ui(fr, fr, 3, GMP_RNDN);
      mpfr_set(MPC_RE(x), fr, GMP_RNDN);
      mpfr_set(MPC_IM(x), fr, GMP_RNDN);

      mpc_set (z, x, MPC_RNDNN);
      mpfr_clear_flags (); /* mpc_cmp set erange flag when an operand is a
                              NaN */
      if (mpc_cmp (z, x) != 0 || mpfr_erangeflag_p())
        {
          printf ("Error in mpc_set for prec = %lu\n",
                  (unsigned long int) prec);
          OUT(z);
          OUT(x);
          exit (1);
        }

      mpc_set_si_si (z, lo, lo, MPC_RNDNN);
      if (mpfr_cmp_si (MPC_RE(z), lo) != 0
          || mpfr_cmp_si (MPC_IM(z), lo) != 0)
        PRINT_ERROR ("mpc_set_si_si", prec, z);

      mpc_set_fr (z, fr, MPC_RNDNN);
      mpfr_clear_flags ();
      if (mpfr_cmp (MPC_RE(z), fr) != 0
          || mpfr_cmp_ui (MPC_IM(z), 0) != 0
          || mpfr_erangeflag_p())
        PRINT_ERROR ("mpc_set_fr", prec, z);

      mpfr_set_z (fr, mpz, GMP_RNDN);      
      mpc_set_z_z (z, mpz, mpz, MPC_RNDNN);
      mpfr_clear_flags ();
      if (mpfr_cmp (MPC_RE(z), fr) != 0
          || mpfr_cmp (MPC_IM(z), fr) != 0
          || mpfr_erangeflag_p())
        PRINT_ERROR ("mpc_set_z_z", prec, z);

      mpc_set_fr_fr (z, fr, fr, MPC_RNDNN);
      mpfr_clear_flags ();
      if (mpfr_cmp (MPC_RE(z), fr) != 0
          || mpfr_cmp (MPC_IM(z), fr) != 0
          || mpfr_erangeflag_p())
        PRINT_ERROR ("mpc_set_fr_fr", prec, z);

      mpc_set_z (z, mpz, MPC_RNDNN);
      mpfr_clear_flags ();
      if (mpfr_cmp (MPC_RE(z), fr) != 0
          || mpfr_cmp_ui (MPC_IM(z), 0) != 0
          || mpfr_erangeflag_p())
        PRINT_ERROR ("mpc_set_z", prec, z);

      mpfr_set_q (fr, mpq, GMP_RNDN);
      mpc_set_q_q (z, mpq, mpq, MPC_RNDNN);
      mpfr_clear_flags ();
      if (mpfr_cmp (MPC_RE(z), fr) != 0
          || mpfr_cmp (MPC_IM(z), fr) != 0
          || mpfr_erangeflag_p())
        PRINT_ERROR ("mpc_set_q_q", prec, z);

      mpc_set_ui_fr (z, prec, fr, MPC_RNDNN);
      mpfr_clear_flags ();
      if (mpfr_cmp_ui (MPC_RE(z), prec) != 0
          || mpfr_cmp (MPC_IM(z), fr) != 0
          || mpfr_erangeflag_p())
        PRINT_ERROR ("mpc_set_fr_fr", prec, z);

      mpc_set_q (z, mpq, MPC_RNDNN);
      mpfr_clear_flags ();
      if (mpfr_cmp (MPC_RE(z), fr) != 0
          || mpfr_cmp_ui (MPC_IM(z), 0) != 0
          || mpfr_erangeflag_p())
        PRINT_ERROR ("mpc_set_q", prec, z);

      mpfr_set_f (fr, mpf, GMP_RNDN);
      mpc_set_f_f (z, mpf, mpf, MPC_RNDNN);
      mpfr_clear_flags ();
      if (mpfr_cmp (MPC_RE(z), fr) != 0
          || mpfr_cmp (MPC_IM(z), fr) != 0
          || mpfr_erangeflag_p())
        PRINT_ERROR ("mpc_set_f_f", prec, z);      

      mpc_set_f (z, mpf, MPC_RNDNN);
      mpfr_clear_flags ();
      if (mpfr_cmp (MPC_RE(z), fr) != 0
          || mpfr_cmp_ui (MPC_IM(z), 0) != 0
          || mpfr_erangeflag_p())
        PRINT_ERROR ("mpc_set_f", prec, z);      

#ifdef _MPC_H_HAVE_INTMAX_T
      {
        uintmax_t uim = prec;
        intmax_t im = prec;

        mpc_set_uj_uj (z, uim, uim, MPC_RNDNN);
        if (mpfr_cmp_ui (MPC_RE(z), prec) != 0
            || mpfr_cmp_ui (MPC_IM(z), prec) != 0)
          PRINT_ERROR ("mpc_set_uj_uj", prec, z);

        mpc_set_sj_sj (z, im, im, MPC_RNDNN);
        if (mpfr_cmp_ui (MPC_RE(z), prec) != 0
            || mpfr_cmp_ui (MPC_IM(z), prec) != 0)
          PRINT_ERROR ("mpc_set_sj_sj", prec, z);
      }
#endif /* _MPC_H_HAVE_INTMAX_T */
    }

  mpz_clear (mpz);
  mpq_clear (mpq);
  mpf_clear (mpf);
  mpfr_clear (fr);
  mpc_clear (x);
  mpc_clear (z);

  return 0;
}
