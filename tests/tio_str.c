/* tio_str.c -- Test file for mpc_inp_str and mpc_out_str.

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

#define  _XOPEN_SOURCE /* for fileno */
#include <stdio.h>
#include <string.h>

#include "mpc-tests.h"

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifndef __SRCDIR
#define __SRCDIR .
#endif

static unsigned long line_number;
/* character appearing next in the file, may be EOF */
static int nextchar;

/* read primitives */
static void
skip_line (FILE *fp)
   /* skips characters until reaching '\n' or EOF; */
   /* '\n' is skipped as well                      */
{
   while (nextchar != EOF && nextchar != '\n')
      nextchar = getc (fp);
   if (nextchar != EOF) {
      line_number++;
      nextchar = getc (fp);
   }
}

static void
skip_comments (FILE *fp)
   /* skips over all whitespace and comments, if any */
{
   while (nextchar == '#') {
      skip_line (fp);
   }
  ungetc (nextchar, fp);
}

/* Try reading valid and invalid complexes from file */
static void
check_data_file (mpc_ptr read_number, char *file_name)
{
  FILE *fp;
  char *pathname;

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

  /* 2. read data file */
  line_number = 1;
  nextchar = getc (fp);
  /* skips over all comments, if any */
  skip_comments (fp);

  /* 2.1 regular number with I */
  if (mpc_inp_str (read_number, fp, NULL, 10, MPC_RNDUZ) == -1)
    {
      printf ("Error: mpc_inp_str cannot correctly read number line %lu in file %s\n",
              line_number, file_name);
      OUT (read_number);

      exit (1);
    }

  mpfr_clear_flags (); /* mpc_cmp set erange flag when an operand is
                          a NaN */
  if (mpc_cmp_si_si (read_number, 1, 1) != 0 || mpfr_erangeflag_p())
    {
      printf ("Error: mpc_inp_str do not correctly re-read number "
              "line %lu in file %s\n", line_number, file_name);
      OUT (read_number);
      printf ("expected: 1 +I*1\n");

      exit (1);
    }

  /* 2.2 with i instead of I */
  skip_line (fp);
  skip_comments (fp);

  if (mpc_inp_str (read_number, fp, NULL, 10, MPC_RNDUZ) == -1)
    {
      printf ("Error: mpc_inp_str cannot correctly re-read number line %lu "
              "in file %s\n", line_number, file_name);

      exit (1);
    }

  mpfr_clear_flags ();
  if (mpc_cmp_si_si (read_number, 1, -1) != 0 || mpfr_erangeflag_p())
    {
      printf ("Error: mpc_inp_str do not correctly re-read number line %lu "
              "in file %s\n", line_number, file_name);
      OUT (read_number);
      printf ("expected: 1 -I* 1\n");

      exit (1);
    }

  /* 2.3 with invalid real part */
  skip_line (fp);
  skip_comments (fp);

  if (mpc_inp_str (read_number, fp, NULL, 10, MPC_RNDUZ) != -1)
    goto read_error;

  /* 2.4 with valid real part but invalid imaginary part */
  skip_line (fp);
  skip_comments (fp);

  if (mpc_inp_str (read_number, fp, NULL, 10, MPC_RNDUZ) != -1)
    goto read_error;

  /* 2.5 with valid real part and + but invalid rest */
  skip_line (fp);
  skip_comments (fp);

  if (mpc_inp_str (read_number, fp, NULL, 10, MPC_RNDUZ) != -1)
    goto read_error;

  /* 2.6 with valid real part and +I but invalid rest */
  skip_line (fp);
  skip_comments (fp);

  if (mpc_inp_str (read_number, fp, NULL, 10, MPC_RNDUZ) != -1)
    goto read_error;

  /* 2.7 with valid real part and +I* but invalid rest */
  skip_line (fp);
  skip_comments (fp);

  if (mpc_inp_str (read_number, fp, NULL, 10, MPC_RNDUZ) != -1)
    goto read_error;

  fclose (fp);

  return ;

 read_error:
  printf ("Error: mpc_inp_str should return -1 but does not (line %lu "
          "in file %s)\n", line_number, file_name);

  exit (1);
}

static void
check_io_str (mpc_ptr read_number, mpc_ptr expected)
{
  char tmp_file[] = "mpc_test";
  FILE *fp;

  if (!(fp = fopen (tmp_file, "w")))
    {
      printf ("Error: Could not open file %s\n", tmp_file);

      exit (1);
    }

  mpc_out_str (fp, 10, 0, expected, MPC_RNDNN);
  fclose (fp);

  if (!(fp = fopen (tmp_file, "r")))
    {
      printf ("Error: Could not open file %s\n", tmp_file);

      exit (1);
    };
  if (mpc_inp_str (read_number, fp, NULL, 10, MPC_RNDNN) == -1)
    {
      printf ("Error: mpc_inp_str cannot correctly re-read number "
              "in file %s\n", tmp_file);

      exit (1);
    }
  fclose (fp);

  /* mpc_cmp set erange flag when an operand is a NaN */
  mpfr_clear_flags ();
  if (mpc_cmp (read_number, expected) != 0 || mpfr_erangeflag_p())
    {
      printf ("Error: inp_str o out_str <> Id\n");
      OUT (read_number);
      OUT (expected);

      exit (1);
    }
}

#ifndef NO_STREAM_REDIRECTION
/* test out_str with stream=NULL */
static void
check_stdout (mpc_ptr read_number, mpc_ptr expected)
{
  char tmp_file[] = "mpc_test";
  int fd;

  fflush(stdout);
  fd = dup(fileno(stdout));
  freopen(tmp_file, "w", stdout);
  mpc_out_str (NULL, 2, 0, expected, MPC_RNDNN);
  fflush(stdout);
  dup2(fd, fileno(stdout));
  close(fd);
  clearerr(stdout);

  fflush(stdin);
  fd = dup(fileno(stdin));
  freopen(tmp_file, "r", stdin);
  if (mpc_inp_str (read_number, NULL, NULL, 2, MPC_RNDNN) == -1)
    {
      printf ("mpc_inp_str cannot correctly re-read number "
              "in file %s\n", tmp_file);
      exit (1);
    }
  mpfr_clear_flags (); /* mpc_cmp set erange flag when an operand is
                          a NaN */
  if (mpc_cmp (read_number, expected) != 0 || mpfr_erangeflag_p())
    {
      printf ("mpc_inp_str did not read the number which was written by "
              "mpc_out_str\n");
      OUT (read_number);
      OUT (expected);
      exit (1);
    }
  fflush(stdin);
  dup2(fd, fileno(stdin));
  close(fd);
  clearerr(stdin);
}
#endif /* NO_STREAM_REDIRECTION */

int
main (void)
{
  mpc_t z, x;
  mp_prec_t prec;

  mpc_init2 (z, 1000);
  mpc_init2 (x, 1000);

  for (prec = 2; prec <= 1000; prec+=7)
    {
      mpc_set_prec (z, prec);
      check_data_file (z, "inp_str.dat");

      mpc_set_prec (x, prec);
      mpc_set_si_si (x, 1, 1, MPC_RNDNN);
      check_io_str (z, x);
      mpc_set_si_si (x, -1, 1, MPC_RNDNN);
      check_io_str (z, x);
      mpfr_set_inf (MPC_RE(x), -1);
      mpfr_set_inf (MPC_IM(x), +1);
      check_io_str (z, x);
    }

#ifndef NO_STREAM_REDIRECTION
  mpc_set_si_si (x, 1, -4, MPC_RNDNN);
  mpc_div_ui (x, x, 3, MPC_RNDDU);

  check_stdout(z, x);
#endif

  mpc_clear (z);
  mpc_clear (x);

  return 0;
}
