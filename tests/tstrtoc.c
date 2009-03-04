/* test file for mpc_strtoc.

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

#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "mpc.h"
#include "mpc-impl.h"
#include "mpc-tests.h"

#ifndef __SRCDIR
#define __SRCDIR .
#endif

extern unsigned long line_number;
extern int nextchar;
extern char *pathname;

static const char *rnd_mode[] =
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

static void
read_int (FILE *fp, int *nread, const char *name)
{
  int n = 0;

  if (nextchar == EOF)
    {
      printf ("Error: Unexpected EOF when reading mpfr precision "
              "in file '%s' line %ld\n",
              pathname, line_number);
      exit (1);
    }
  ungetc (nextchar, fp);
  n = fscanf (fp, "%i", nread);
  if (ferror (fp) || n == 0 || n == EOF)
    {
      printf ("Error: Cannot read %s in file '%s' line %ld\n",
              name, pathname, line_number);
      exit (1);
    }
  nextchar = getc (fp);
  skip_whitespace_comments (fp);
}

static size_t
read_string (FILE *fp, char **buffer_ptr, size_t buffer_length, const char *name)
{
  size_t pos;
  char *buffer;

  pos = 0;
  buffer = *buffer_ptr;

  if (nextchar == '"')
    nextchar = getc (fp);
  else
    goto error;

  while (nextchar != EOF && nextchar != '"')
    {
      if (nextchar == '\n')
        line_number++;
      if (pos + 1 > buffer_length)
        {
          buffer = realloc (buffer, 2 * buffer_length);
          if (buffer == NULL)
            {
              printf ("Cannot allocate memory\n");
              exit (1);
            }
          buffer_length *= 2;
        }
      buffer[pos++] = nextchar;
      nextchar = getc (fp);
    }

  if (nextchar != '"')
    goto error;

  if (pos + 1 > buffer_length)
    {
      buffer = realloc (buffer, buffer_length + 1);
      if (buffer == NULL)
        {
          printf ("Cannot allocate memory\n");
          exit (1);
        }
      buffer_length *= 2;
    }
  buffer[pos] = '\0';

  nextchar = getc (fp);
  skip_whitespace_comments (fp);

  buffer_ptr = &buffer;

  return buffer_length;

 error:
  printf ("Error: Unable to read %s in file '%s' line '%ld'\n",
          name, pathname, line_number);
  exit (1);
}

static void
check_file (const char* file_name)
{
  FILE *fp;

  size_t str_len = 255;
  char *str = NULL;
  size_t rstr_len = 255;
  char *rstr = NULL;
  char *end = NULL;

  int base;
  int inex_re;
  int inex_im;
  mpc_t expected, got;
  mpc_rnd_t rnd = MPC_RNDNN;
  int inex = 0;
  known_signs_t ks = {1, 1};


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
  str = (char *) malloc (str_len * sizeof (char));
  if (str == NULL)
    {
      printf ("Cannot allocate memory\n");
      exit (1);
    }
  rstr = (char *) malloc (rstr_len * sizeof (char));
  if (rstr == NULL)
    {
      printf ("Cannot allocate memory\n");
      exit (1);
    }
  mpc_init2 (expected, 53);
  mpc_init2 (got, 53);

  /* 3. read data file */
  line_number = 1;
  nextchar = getc (fp);
  while (nextchar != EOF)
    {
      skip_whitespace_comments (fp);

      /* 3.1 read a line of data: expected result, base, rounding mode */
      read_ternary (fp, &inex_re);
      read_ternary (fp, &inex_im);
      read_mpc (fp, expected, NULL);

      str_len = read_string (fp, &str, str_len, "number string");
      rstr_len = read_string (fp, &rstr, rstr_len, "string remainder");
      read_int (fp, &base, "base");
      read_mpc_rounding_mode (fp, &rnd);

      /* 3.2 convert string at the same precision as the expected result */
      mpfr_set_prec (MPC_RE (got), MPC_PREC_RE (expected));
      mpfr_set_prec (MPC_IM (got), MPC_PREC_IM (expected));
      inex = mpc_strtoc (got, str, &end, base, rnd);

      /* 3.3 compare this result with the expected one */
      if (inex != MPC_INEX (inex_re, inex_im)
          || !same_mpc_value (got, expected, ks)
          || strcmp (end, rstr) != 0)
        {
          printf ("mpc_strtoc(str) failed (line %lu)\nwith base=%d and "
                  "rounding mode %s\n", line_number-1, base, rnd_mode[rnd]);
          if (inex != MPC_INEX (inex_re, inex_im))
            printf ("ternary value: got %s, expected (%s, %s)\n",
                    MPC_INEX_STR (inex),
                    (inex_re == +1 ? "+1" : (inex_re == -1 ? "-1" : "0")),
                    (inex_im == +1 ? "+1" : (inex_im == -1 ? "-1" : "0")));
          printf ("str = \"%s\"\n", str);
          if (strcmp (end, rstr) != 0)
            printf ("string remainder expected \"%s\"\n"
                    "                 got      \"%s\"\n",
                    rstr, end);
          else
            {
              printf ("     ");
              OUT (got);
              OUT (expected);
            }
          exit (1);
        }

      end = NULL;
    }

  fclose (fp);

  /* 4. Clear used variables */
  mpc_clear (expected);
  mpc_clear (got);
  if (str != NULL)
    free (str);
  if (rstr != NULL)
    free (rstr);
  free (pathname);
}

static void
check_null (void)
{
  int inex;
  char *end;
  mpc_t z;

  mpc_init2 (z, 53);

  inex = mpc_strtoc (z, NULL, &end, 10, MPC_RNDNN);
  if (end != NULL || inex != 0 || mpfr_nan_p (MPC_RE (z)) == 0
      || mpfr_nan_p (MPC_IM (z)) == 0)
    {
      printf ("Error: mpc_strtoc(z, NULL) with an NULL pointer should fail"
              " and the z value should be set to NaN +I*NaN\ngot ");
      OUT (z);
      exit (1);
    }

  inex = mpc_strtoc (z, "", &end, 10, MPC_RNDNN);
  if (inex != 0 || mpfr_nan_p (MPC_RE (z)) == 0
      || mpfr_nan_p (MPC_IM (z)) == 0)
    {
      printf ("Error: mpc_strtoc(z, "") with an empty string should fail"
              " and the z value should be set to NaN +I*NaN\ngot ");
      OUT (z);
      exit (1);
    }

  mpc_clear (z);
}

int
main (void)
{
  check_null ();
  check_file ("strtoc.dat");
  return 0;
}
