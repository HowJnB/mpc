/* Read data file and check function.

Copyright (C) 2008 Andreas Enge, Philippe Th\'eveny

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

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "mpc-tests.h"
#include "mpc-impl.h"

#ifndef __SRCDIR
#define __SRCDIR .
#endif

unsigned long line_number;
char *pathname;

const char *mpfr_rnd_mode [] =
  { "GMP_RNDN", "GMP_RNDZ", "GMP_RNDU", "GMP_RNDD" };

const char *rnd_mode[] =
  { "MPC_RNDNN", "MPC_RNDZN", "MPC_RNDUN", "MPC_RNDDN",
    "undefined", "undefined", "undefined", "undefined", "undefined",
    "undefined", "undefined", "undefined", "undefined", "undefined",
    "undefined", "undefined",
    "MPC_RNDNZ", "MPC_RNDZZ", "MPC_RNDUZ", "MPC_RNDDZ",
    "undefined", "undefined", "undefined", "undefined", "undefined",
    "undefined", "undefined", "undefined", "undefined", "undefined",
    "undefined", "undefined",
    "MPC_RNDNU", "MPC_RNDZU", "MPC_RNDUU", "MPC_RNDDU",
    "undefined", "undefined", "undefined", "undefined", "undefined",
    "undefined", "undefined", "undefined", "undefined", "undefined",
    "undefined", "undefined",
    "MPC_RNDND", "MPC_RNDZD", "MPC_RNDUD", "MPC_RNDDD",
    "undefined", "undefined", "undefined", "undefined", "undefined",
    "undefined", "undefined", "undefined", "undefined", "undefined",
    "undefined", "undefined",
  };

/* read primitives */
/* skip white space and comments (from '#' to the end of the line) */
static int
skip_whitespace (FILE *fp)
{
  char c;

  while ((c = getc (fp)) != EOF)
    {
      if (c == '\n')
        ++line_number;

      if (isspace (c))
        continue;
      if (c == '#')
        {
          /* skip line of comment */
          while ((c = getc (fp)) != EOF && c != '\n')
            {}
          if (c == EOF)
            break;

          if (c == '\n')
            ++line_number;
        }
      else
        break;
    }

  if (c == EOF)
    return EOF;

  ungetc (c, fp);
  return 0;
}

static int
read_rounding_mode (FILE *fp, mpc_rnd_t* rnd)
{
  int r[2];
  int i;
  char c;

  for (i=0; i < 2; i++)
    {
      if (skip_whitespace (fp))
        return -1;

      if ((c = getc (fp)) == EOF)
        {
          perror ("data_check");
          exit (1);
        }

      switch (c)
        {
        case 'n': case 'N':
          r[i] = GMP_RNDN;
          break;
        case 'z': case 'Z':
          r[i] = GMP_RNDZ;
          break;
        case 'u': case 'U':
          r[i] = GMP_RNDU;
          break;
        case 'd': case 'D':
          r[i] = GMP_RNDD;
          break;
        default:
          return -1;
        }
    }
  *rnd = RNDC (r[0], r[1]);
  return 0;
}

static int
read_mpfr_rounding_mode (FILE *fp, mpfr_rnd_t* rnd)
{
  char c;

  if (skip_whitespace (fp))
    return -1;

  if ((c = getc (fp)) == EOF)
    {
      perror ("data_check");
      exit (1);
    }

  switch (c)
    {
    case 'n': case 'N':
      *rnd = GMP_RNDN;
      break;
    case 'z': case 'Z':
      *rnd = GMP_RNDZ;
      break;
    case 'u': case 'U':
      *rnd = GMP_RNDU;
      break;
    case 'd': case 'D':
      *rnd = GMP_RNDD;
      break;
    default:
      return -1;
    }

  return 0;
}

static int
read_mpfr (FILE *fp, mpfr_ptr x, int *known_sign)
{
  int n;
  char c;
  mpfr_prec_t prec;

  /* read prec */
  if (skip_whitespace (fp))
    return -1;
  n = fscanf (fp, "%lu", (unsigned long *)&prec);
  if (n == EOF && ferror (fp))
    {
      perror ("data_check");
      exit (1);
    }
  if (n == 0 || prec < MPFR_PREC_MIN || prec > MPFR_PREC_MAX)
    return -1;

  mpfr_set_prec (x, prec);

  /* read value */
  if (skip_whitespace (fp))
    return -1;

  /* Is the sign specified ? */
  if ((c = getc (fp)) == EOF)
    {
      perror ("data_check");
      exit (1);
    }
  ungetc (c, fp);

  if (mpfr_inp_str (x, fp, 0, GMP_RNDN) == 0)
    return -1;

  /* the sign always matters for regular values ('+' is implicit),
     but when no sign appears before 0 or Inf in the data file, it means
     that only absolute value must be checked. */
  if (known_sign != NULL)
    *known_sign =
      (!mpfr_zero_p (x) && !mpfr_inf_p (x)) || c == '+' || c == '-';

  return 0;
}

static int
read_mpc (FILE *fp, mpc_ptr z, known_signs_t *ks)
{
  if (read_mpfr (fp, MPC_RE (z), ks == NULL ? NULL : &ks->re))
    return -1;
  if (read_mpfr (fp, MPC_IM (z), ks == NULL ? NULL : &ks->im))
    return -1;
  return 0;
}

/* read lines of data */
static void
read_cc (FILE *fp, mpc_ptr expected, known_signs_t *signs, mpc_ptr op,
         mpc_rnd_t *rnd)
{
  if (read_mpc (fp, expected, signs))
    {
      printf ("Error: corrupted result in file '%s' line %ld\n",
              pathname, line_number);
      exit (1);
    }
  if (read_mpc (fp, op, NULL))
    {
      printf ("Error: corrupted argument in file '%s' line %ld\n",
              pathname, line_number);
      exit (1);
    }
  if (read_rounding_mode (fp, rnd))
    {
      printf ("Error: unexpected rounding mode in file '%s' line %ld\n",
              pathname, line_number);
      exit (1);
    }
}

static void
read_fc (FILE *fp, mpfr_ptr expected, int *sign, mpc_ptr op,
         mpfr_rnd_t *rnd)
{
  if (read_mpfr (fp, expected, sign))
    {
      printf ("Error: corrupted result in file '%s' line %ld\n",
              pathname, line_number);
      exit (1);
    }
  if (read_mpc (fp, op, NULL))
    {
      printf ("Error: corrupted argument in file '%s' line %ld\n",
              pathname, line_number);
      exit (1);
    }
  if (read_mpfr_rounding_mode (fp, rnd))
    {
      printf ("Error: unexpected rounding mode in file '%s' line %ld\n",
              pathname, line_number);
      exit (1);
    }
}

static void
read_ccc (FILE *fp, mpc_ptr expected, known_signs_t *signs,
          mpc_ptr op1, mpc_ptr op2, mpc_rnd_t *rnd)
{
  if (read_mpc (fp, expected, signs))
    {
      printf ("Error: corrupted result in file '%s' line %ld\n",
              pathname, line_number);
      exit (1);
    }
  if (read_mpc (fp, op1, NULL))
    {
      printf ("Error: corrupted argument in file '%s' line %ld\n",
              pathname, line_number);
      exit (1);
    }
  if (read_mpc (fp, op2, NULL))
    {
      printf ("Error: corrupted argument in file '%s' line %ld\n",
              pathname, line_number);
      exit (1);
    }
  if (read_rounding_mode (fp, rnd))
    {
      printf ("Error: unexpected rounding mode in file '%s' line %ld\n",
              pathname, line_number);
      exit (1);
    }
}

static void
read_cfc (FILE *fp, mpc_ptr expected, known_signs_t *signs, mpfr_ptr op1,
          mpc_ptr op2, mpc_rnd_t *rnd)
{

  if (read_mpc (fp, expected, signs))
    {
      printf ("Error: corrupted result in file '%s' line %ld\n",
              pathname, line_number);
      exit (1);
    }
  if (read_mpfr (fp, op1, NULL))
    {
      printf ("Error: corrupted argument in file '%s' line %ld\n",
              pathname, line_number);
      exit (1);
    }
  if (read_mpc (fp, op2, NULL))
    {
      printf ("5.Error: corrupted argument in file '%s' line %ld\n",
              pathname, line_number);
      exit (1);
    }
  if (read_rounding_mode (fp, rnd))
    {
      printf ("Error: unexpected rounding mode in file '%s' line %ld\n",
              pathname, line_number);
      exit (1);
    }
}

static void
read_ccf (FILE *fp, mpc_ptr expected, known_signs_t *signs, mpc_ptr op1,
          mpfr_ptr op2, mpc_rnd_t *rnd)
{
  if (read_mpc (fp, expected, signs))
    {
      printf ("Error: corrupted result in file '%s' line %ld\n",
              pathname, line_number);
      exit (1);
    }
  if (read_mpc (fp, op1, NULL))
    {
      printf ("Error: corrupted argument in file '%s' line %ld\n",
              pathname, line_number);
      exit (1);
    }
  if (read_mpfr (fp, op2, NULL))
    {
      printf ("Error: corrupted argument in file '%s' line %ld\n",
              pathname, line_number);
      exit (1);
    }
  if (read_rounding_mode (fp, rnd))
    {
      printf ("Error: unexpected rounding mode in file '%s' line %ld\n",
              pathname, line_number);
      exit (1);
    }
}

/* data_check (function, data_file_name) checks function results against
 precomputed data in a file.*/
void
data_check (mpc_function function, const char *file_name)
{
  FILE *fp;
  char c;

  mpfr_t x1, x2;
  mpfr_rnd_t mpfr_rnd = GMP_RNDN;
  int sign_real;
  mpc_t z1, z2, z3, z4;
  mpc_rnd_t rnd = MPC_RNDNN;
  known_signs_t signs;

  /* 1. open data file */
  pathname = (char *)malloc ((strlen (QUOTE (__SRCDIR)) + strlen (file_name)
                              + 2) * sizeof (char));
  if (pathname == NULL)
    {
      printf ("Cannot allocate memory\n");
      exit (1);
    }
  sprintf (pathname, QUOTE (__SRCDIR)"/%s", file_name);
  fp = fopen (pathname, "r");
  if (fp == NULL)
    {
      fprintf (stderr, "Unable to open %s\n", pathname);
      exit (1);
    }

  /* 2. init needed variables */
  mpc_init (z1);
  switch (function.type)
    {
    case FC:
      mpfr_init (x1);
      mpfr_init (x2);
    case CC: case V_CC:
      mpc_init (z2);
      mpc_init (z3);
      break;
    case CCC:
      mpc_init (z2);
      mpc_init (z3);
      mpc_init (z4);
      break;
    case CFC: case CCF:
      mpfr_init (x1);
      mpc_init (z2);
      mpc_init (z3);
      break;
    default:
      ;
    }

  /* 3. read data file */
  line_number = 1;
  while (1)
    {
      c = skip_whitespace (fp);
      if (c)
        {
          if (c == EOF)
            break;
          printf ("Error: corrupted argument in file '%s' line %ld\n",
                  pathname, line_number);
          exit (1);
        }

      /* for each kind of function prototype: */
      /* 3.1 read a line of data: expected result, parameters, rounding mode */
      /* 3.2 compute function at the same precision as the expected result */
      /* 3.3 compare this result with the expected one */
      switch (function.type)
        {
        case FC:
          read_fc (fp, x1, &sign_real, z1, &mpfr_rnd);
          mpfr_set_prec (x2, MPFR_PREC (x1));
          function.pointer.FC (x2, z1, mpfr_rnd);
          if (!same_mpfr_value (x1, x2, sign_real))
            {
              mpfr_t got, expected;
              mpc_t op;
              printf ("%s(op) failed (%s:%lu)\nwith rounding mode %s\n",
                      function.name, file_name, line_number,
                      mpfr_rnd_mode[mpfr_rnd]);
              OUT (op);
              printf ("     ");
              MPFR_OUT (got);
              MPFR_OUT (expected);
              
              exit (1);
            }
          break;

        case CC:
          read_cc (fp, z1, &signs, z2, &rnd);
          mpfr_set_prec (MPC_RE (z3), MPC_PREC_RE (z1));
          mpfr_set_prec (MPC_IM (z3), MPC_PREC_IM (z1));
          function.pointer.CC (z3, z2, rnd);
          if (!same_mpc_value (z3, z1, signs))
            {
              mpc_t op, got, expected; /* display sensible variable names */
              op[0] = z2[0];
              expected[0]= z1[0];
              got[0] = z3[0];
              printf ("%s(op) failed (line %lu)\nwith rounding mode %s\n",
                      function.name, line_number, rnd_mode[rnd]);
              OUT (op);
              printf ("     ");
              OUT (got);
              OUT (expected);
              
              exit (1);
            }
          break;

        case V_CC:
          read_cc (fp, z1, &signs, z2, &rnd);
          mpfr_set_prec (MPC_RE (z3), MPC_PREC_RE (z1));
          mpfr_set_prec (MPC_IM (z3), MPC_PREC_IM (z1));
          function.pointer.V_CC (z3, z2, rnd);
          if (!same_mpc_value (z3, z1, signs))
            {
              mpc_t op, got, expected; /* display sensible variable names */
              op[0] = z2[0];
              expected[0]= z1[0];
              got[0] = z3[0];
              printf ("%s(op) failed (line %lu)\nwith rounding mode %s\n      ",
                      function.name, line_number, rnd_mode[rnd]);
              OUT (op);
              printf ("     ");
              OUT (got);
              OUT (expected);
              
              exit (1);
            }
          break;

        case CCC:
          read_ccc (fp, z1, &signs, z2, z3, &rnd);
          mpfr_set_prec (MPC_RE(z4), MPC_PREC_RE (z1));
          mpfr_set_prec (MPC_IM(z4), MPC_PREC_IM (z1));
          function.pointer.CCC (z4, z2, z3, rnd);
          if (!same_mpc_value (z4, z1, signs))
            {
              /* display sensible variable names */
              mpc_t op1, op2, got, expected;
              op1[0] = z2[0];
              op2[0] = z3[0];
              expected[0]= z1[0];
              got[0] = z4[0];
              printf ("%s(op) failed (line %lu)\nwith rounding mode %s\n",
                      function.name, line_number, rnd_mode[rnd]);
              OUT (op1);
              OUT (op2);
              printf ("     ");
              OUT (got);
              OUT (expected);
              
              exit (1);
            }
          break;

        case CFC:
          read_cfc (fp, z1, &signs, x1, z2, &rnd);
          mpfr_set_prec (MPC_RE(z3), MPC_PREC_RE (z1));
          mpfr_set_prec (MPC_IM(z3), MPC_PREC_IM (z1));
          function.pointer.CFC (z3, x1, z2, rnd);
          if (!same_mpc_value (z3, z1, signs))
            {
              /* display sensible variable names */
              mpc_t op2, got, expected;
              mpfr_t op1;
              op1[0] = x1[0];
              op2[0] = z2[0];
              expected[0]= z1[0];
              got[0] = z3[0];
              printf ("%s(op) failed (line %lu)\nwith rounding mode %s\n",
                      function.name, line_number, rnd_mode[rnd]);
              MPFR_OUT (op1);
              OUT (op2);
              printf ("     ");
              OUT (got);
              OUT (expected);
              
              exit (1);
            }
          break;

        case CCF:
          read_ccf (fp, z1, &signs, z2, x1, &rnd);
          mpfr_set_prec (MPC_RE(z3), MPC_PREC_RE (z1));
          mpfr_set_prec (MPC_IM(z3), MPC_PREC_IM (z1));
          function.pointer.CCF (z3, z2, x1, rnd);
          if (!same_mpc_value (z3, z1, signs))
            {
              /* display sensible variable names */
              mpc_t op1, got, expected;
              mpfr_t op2;
              op1[0] = z2[0];
              op2[0] = x1[0];
              expected[0]= z1[0];
              got[0] = z3[0];
              printf ("%s(op) failed (line %lu)\nwith rounding mode %s\n",
                      function.name, line_number, rnd_mode[rnd]);
              OUT (op1);
              MPFR_OUT (op2);
              printf ("     ");
              OUT (got);
              OUT (expected);
              
              exit (1);
            }
          break;

        default:
          ;
        }
    }

  /* 4. Clear used variables */
  mpc_clear (z1);
  switch (function.type)
    {
    case FC:
      mpfr_clear (x1);
      mpfr_clear (x2);
    case CC: case V_CC:
      mpc_clear (z2);
      mpc_clear (z3);
      break;
    case CCC:
      mpc_clear (z2);
      mpc_clear (z3);
      mpc_clear (z4);
      break;
    case CFC: case CCF:
      mpfr_clear (x1);
      mpc_clear (z2);
      mpc_clear (z3);
      break;
    default:
      ;
    }

  fclose (fp);
  free (pathname);
}
