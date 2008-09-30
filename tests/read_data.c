/* Read data file and check function.

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
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifndef TEST_FUNCTION
#error "define TEST_FUNCTION macro before including read_data.c"
#endif

#ifndef __SRCDIR
#define __SRCDIR .
#endif

#define _QUOTEME(x) #x
#define QUOTEME(x) _QUOTEME (x)

/* some sign are unspecified in ISO C99, thus we record in struct known_signs_t
   whether the sign has to be checked */
typedef struct {
  int re;
  int im;
}
known_signs_t;

unsigned long line_number;
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
    "MPC_RNDND", "MPC_RNDZD", "MPC_RNDUD", "MPC_RNDDD"
    "undefined", "undefined", "undefined", "undefined", "undefined",
    "undefined", "undefined", "undefined", "undefined", "undefined",
    "undefined", "undefined",
  };

static int
same_value (mpc_ptr got, mpc_ptr ref, known_signs_t known_signs)
{
  int cmp_re, cmp_im;

  cmp_re = (mpfr_nan_p (MPC_RE (got)) && mpfr_nan_p (MPC_RE (ref)))
    || (!known_signs.re
        && ((mpfr_inf_p (MPC_RE (got)) && mpfr_inf_p (MPC_RE (ref)))
            || (mpfr_zero_p (MPC_RE (got)) && mpfr_zero_p (MPC_RE (ref)))))
    || mpfr_cmp (MPC_RE (got), MPC_RE (ref)) == 0;

  cmp_im = (mpfr_nan_p (MPC_IM (got)) && mpfr_nan_p (MPC_IM (ref)))
    || (!known_signs.im
        && ((mpfr_inf_p (MPC_IM (got)) && mpfr_inf_p (MPC_IM (ref)))
            || (mpfr_zero_p (MPC_IM (got)) && mpfr_zero_p (MPC_IM (ref)))))
    || mpfr_cmp (MPC_IM (got), MPC_IM (ref)) == 0;

  return cmp_re && cmp_im;
}

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
read_mpfr (FILE *fp, mpfr_ptr x, int *known_sign)
{
  int n;
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
  if (known_sign != NULL)
    /* Is the sign specified ? */
    {
      char c;

      if ((c = getc (fp)) == EOF)
        {
          perror ("data_check");
          exit (1);
        }
      *known_sign = (c == '+' || c == '-');
      ungetc (c, fp);
    }
  if (mpfr_inp_str (x, fp, 0, GMP_RNDN) == 0)
    return -1;

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

static void
data_check (const char *file_name)
{
  FILE *fp;
  mpc_rnd_t rnd = MPC_RNDNN;
  mpc_t op, got, expected;
  known_signs_t signs;
  char c;
  char *full_name;

  full_name = (char *)malloc ((strlen (QUOTEME (__SRCDIR))
                               + strlen (file_name) + 2) * sizeof (char));
  if (full_name == NULL)
    {
      printf ("Cannot allocate memory\n");
      exit (1);
    }

  sprintf (full_name, QUOTEME (__SRCDIR)"/%s", file_name);

  fp = fopen (full_name, "r");
  if (fp == NULL)
    {
      fprintf (stderr, "Unable to open %s\n", full_name);
      exit (1);
    }

  mpc_init (op);
  mpc_init (expected);
  line_number = 1;

  while (1)
    {
      c = skip_whitespace (fp);
      if (c)
        {
          if (c == EOF)
            break;
          printf ("Error: corrupted argument in file '%s' line %ld\n",
                  full_name, line_number);
          exit (1);
        }

      if (read_mpc (fp, op, NULL))
        {
          printf ("Error: corrupted argument in file '%s' line %ld\n",
                  full_name, line_number);
          exit (1);
        }
      if (read_mpc (fp, expected, &signs))
        {
          printf ("Error: corrupted result in file '%s' line %ld\n",
                  full_name, line_number);
          exit (1);
        }
      if (read_rounding_mode (fp, &rnd))
        {
          printf ("Error: unexpected rounding mode in file '%s' line %ld\n",
                  full_name, line_number);
          exit (1);
        }

      mpc_init3 (got, MPC_PREC_RE (expected), MPC_PREC_IM (expected));
      TEST_FUNCTION (got, op, rnd);

      if (!same_value (got, expected, signs))
        {
          printf ("%s(op) failed (line %lu)\nwith rounding mode %s\n      ",
                  QUOTEME (TEST_FUNCTION), line_number, rnd_mode[rnd]);
          OUT (op);
          printf ("     ");
          OUT (got);
          OUT (expected);

          exit (1);
        }
    }

  fclose (fp);
  free (full_name);
  mpc_clear (op);
  mpc_clear (got);
  mpc_clear (expected);
}
