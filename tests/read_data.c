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

static char *pathname;
static unsigned long line_number;
   /* file name with complete path and currently read line;
      kept globally to simplify parameter passing */
static int nextchar;
   /* character appearing next in the file, may be EOF */

#define __NOT_CHECKED 255
/* ternary value comparison */
#define MPC_INEX_CMP(r, i, c)                           \
  (((r) == __NOT_CHECKED || (r) == MPC_INEX_RE(c))      \
   && ((i) == __NOT_CHECKED || (i) == MPC_INEX_IM (c)))
#define MPFR_INEX_STR(inex)                     \
  (inex) == __NOT_CHECKED ? "x"                 \
    : (inex) == +1 ? "+1"                       \
    : (inex) == -1 ? "-1" : "0"
#define MPC_INEX_STR(inex)                      \
  (inex) == 0 ? "(0, 0)"                        \
    : (inex) == 1 ? "(+1, 0)"                   \
    : (inex) == 2 ? "(-1, 0)"                   \
    : (inex) == 4 ? "(0, +1)"                   \
    : (inex) == 5 ? "(+1, +1)"                  \
    : (inex) == 6 ? "(-1, +1)"                  \
    : (inex) == 8 ? "(0, -1)"                   \
    : (inex) == 9 ? "(-1, -1)"                  \
    : (inex) == 10 ? "(+1, -1)" : "unknown"


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
skip_whitespace (FILE *fp)
   /* skips over whitespace if any until reaching EOF */
   /* or non-whitespace                               */
{
   while (isspace (nextchar)) {
      if (nextchar == '\n')
         line_number++;
      nextchar = getc (fp);
   }
}

static void
skip_whitespace_comments (FILE *fp)
   /* skips over all whitespace and comments, if any */
{
   skip_whitespace (fp);
   while (nextchar == '#') {
      skip_line (fp);
      if (nextchar != EOF)
         skip_whitespace (fp);
   }
}

/* All following read routines skip over whitespace and comments; */
/* so after calling them, nextchar is either EOF or the beginning */
/* of a non-comment token.                                        */
static void
read_ternary (FILE *fp, int* ternary)
{
  switch (nextchar)
    {
    case '?':
      *ternary = __NOT_CHECKED;
      break;
    case '+':
      *ternary = +1;
      break;
    case '0':
      *ternary = 0;
      break;
    case '-':
      *ternary = -1;
      break;
    default:
      printf ("Error: Unexpected ternary value '%c' in file '%s' line %ld\n",
              nextchar, pathname, line_number);
      exit (1);
    }

  nextchar = getc (fp);
  skip_whitespace_comments (fp);
}

static void
read_mpfr_rounding_mode (FILE *fp, mpfr_rnd_t* rnd)
{
  switch (nextchar)
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
      printf ("Error: Unexpected rounding mode '%c' in file '%s' line %ld\n",
              nextchar, pathname, line_number);
      exit (1);
    }

    nextchar = getc (fp);
    if (nextchar != EOF && !isspace (nextchar)) {
      printf ("Error: Rounding mode not followed by white space in file "
              "'%s' line %ld\n",
              pathname, line_number);
      exit (1);
    }
    skip_whitespace_comments (fp);
}

static void
read_mpc_rounding_mode (FILE *fp, mpc_rnd_t* rnd)
{
   mpfr_rnd_t re, im;
   read_mpfr_rounding_mode (fp, &re);
   read_mpfr_rounding_mode (fp, &im);
   *rnd = RNDC (re, im);
}

static mpfr_prec_t
read_mpfr_prec (FILE *fp)
{
   unsigned long prec;
   int n;

   if (nextchar == EOF) {
      printf ("Error: Unexpected EOF when reading mpfr precision "
              "in file '%s' line %ld\n",
              pathname, line_number);
      exit (1);
   }
   ungetc (nextchar, fp);
   n = fscanf (fp, "%lu", &prec);
   if (ferror (fp)) /* then also n == EOF */
      perror ("Error when reading mpfr precision");
   if (n == 0 || n == EOF || prec < MPFR_PREC_MIN || prec > MPFR_PREC_MAX) {
      printf ("Error: Impossible mpfr precision in file '%s' line %ld\n",
              pathname, line_number);
      exit (1);
   }
   nextchar = getc (fp);
   skip_whitespace_comments (fp);
   return (mpfr_prec_t) prec;
}

static void
read_mpfr_mantissa (FILE *fp, mpfr_ptr x)
{
   if (nextchar == EOF) {
      printf ("Error: Unexpected EOF when reading mpfr mantissa "
              "in file '%s' line %ld\n",
              pathname, line_number);
      exit (1);
   }
   ungetc (nextchar, fp);
   if (mpfr_inp_str (x, fp, 0, GMP_RNDN) == 0) {
      printf ("Error: Impossible to read mpfr mantissa "
              "in file '%s' line %ld\n",
              pathname, line_number);
      exit (1);
   }
   nextchar = getc (fp);
   skip_whitespace_comments (fp);
}

static void
read_mpfr (FILE *fp, mpfr_ptr x, int *known_sign)
{
   mpfr_set_prec (x, read_mpfr_prec (fp));
   read_mpfr_mantissa (fp, x);

   /* the sign always matters for regular values ('+' is implicit),
      but when no sign appears before 0 or Inf in the data file, it means
      that only absolute value must be checked. */
   if (known_sign != NULL)
      *known_sign =
         (!mpfr_zero_p (x) && !mpfr_inf_p (x))
          || nextchar == '+' || nextchar == '-';
}

static void
read_mpc (FILE *fp, mpc_ptr z, known_signs_t *ks)
{
  read_mpfr (fp, MPC_RE (z), ks == NULL ? NULL : &ks->re);
  read_mpfr (fp, MPC_IM (z), ks == NULL ? NULL : &ks->im);
}

/* read lines of data */
static void
read_cc (FILE *fp, int *inex_re, int *inex_im, mpc_ptr expected,
         known_signs_t *signs, mpc_ptr op, mpc_rnd_t *rnd)
{
  read_ternary (fp, inex_re);
  read_ternary (fp, inex_im);
  read_mpc (fp, expected, signs);
  read_mpc (fp, op, NULL);
  read_mpc_rounding_mode (fp, rnd);
}

static void
read_fc (FILE *fp, int *inex, mpfr_ptr expected, int *sign, mpc_ptr op,
         mpfr_rnd_t *rnd)
{
  read_ternary (fp, inex);
  read_mpfr (fp, expected, sign);
  read_mpc (fp, op, NULL);
  read_mpfr_rounding_mode (fp, rnd);
}

static void
read_ccc (FILE *fp, int *inex_re, int *inex_im, mpc_ptr expected,
          known_signs_t *signs, mpc_ptr op1, mpc_ptr op2, mpc_rnd_t *rnd)
{
  read_ternary (fp, inex_re);
  read_ternary (fp, inex_im);
  read_mpc (fp, expected, signs);
  read_mpc (fp, op1, NULL);
  read_mpc (fp, op2, NULL);
  read_mpc_rounding_mode (fp, rnd);
}

static void
read_cfc (FILE *fp, int *inex_re, int *inex_im, mpc_ptr expected,
          known_signs_t *signs, mpfr_ptr op1, mpc_ptr op2, mpc_rnd_t *rnd)
{

  read_ternary (fp, inex_re);
  read_ternary (fp, inex_im);
  read_mpc (fp, expected, signs);
  read_mpfr (fp, op1, NULL);
  read_mpc (fp, op2, NULL);
  read_mpc_rounding_mode (fp, rnd);
}

static void
read_ccf (FILE *fp, int *inex_re, int *inex_im, mpc_ptr expected,
          known_signs_t *signs, mpc_ptr op1, mpfr_ptr op2, mpc_rnd_t *rnd)
{
  read_ternary (fp, inex_re);
  read_ternary (fp, inex_im);
  read_mpc (fp, expected, signs);
  read_mpc (fp, op1, NULL);
  read_mpfr (fp, op2, NULL);
  read_mpc_rounding_mode (fp, rnd);
}

/* data_check (function, data_file_name) checks function results against
 precomputed data in a file.*/
void
data_check (mpc_function function, const char *file_name)
{
  FILE *fp;

  int inex_re;
  mpfr_t x1, x2;
  mpfr_rnd_t mpfr_rnd = GMP_RNDN;
  int sign_real;

  int inex_im;
  mpc_t z1, z2, z3, z4;
  mpc_rnd_t rnd = MPC_RNDNN;
  known_signs_t signs;
  int inex = 0;

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
      break;
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
  nextchar = getc (fp);
  skip_whitespace_comments (fp);
  while (nextchar != EOF) {
      /* for each kind of function prototype: */
      /* 3.1 read a line of data: expected result, parameters, rounding mode */
      /* 3.2 compute function at the same precision as the expected result */
      /* 3.3 compare this result with the expected one */
      switch (function.type)
        {
        case FC:
          read_fc (fp, &inex_re, x1, &sign_real, z1, &mpfr_rnd);
          mpfr_set_prec (x2, MPFR_PREC (x1));
          inex = function.pointer.FC (x2, z1, mpfr_rnd);
          if (!MPC_INEX_CMP (inex_re, __NOT_CHECKED, inex)
              || !same_mpfr_value (x1, x2, sign_real))
            {
              mpfr_t got, expected;
              mpc_t op;
              op[0] = z1[0];
              got[0] = x2[0];
              expected[0] = x1[0];
              printf ("%s(op) failed (%s:%lu)\nwith rounding mode %s\n",
                      function.name, file_name, line_number-1,
                      mpfr_rnd_mode[mpfr_rnd]);
              if (!MPC_INEX_CMP (inex_re, __NOT_CHECKED, inex))
                printf("ternary value: got %s, expected %s\n",
                       MPFR_INEX_STR (inex), MPFR_INEX_STR (inex_re));
              OUT (op);
              printf ("     ");
              MPFR_OUT (got);
              MPFR_OUT (expected);

              exit (1);
            }
          break;

        case CC:
          read_cc (fp, &inex_re, &inex_im, z1, &signs, z2, &rnd);
          mpfr_set_prec (MPC_RE (z3), MPC_PREC_RE (z1));
          mpfr_set_prec (MPC_IM (z3), MPC_PREC_IM (z1));
          inex = function.pointer.CC (z3, z2, rnd);
          if (!MPC_INEX_CMP (inex_re, inex_im, inex)
              || !same_mpc_value (z3, z1, signs))
            {
              mpc_t op, got, expected; /* display sensible variable names */
              op[0] = z2[0];
              expected[0]= z1[0];
              got[0] = z3[0];
              printf ("%s(op) failed (line %lu)\nwith rounding mode %s\n",
                      function.name, line_number-1, rnd_mode[rnd]);
              if (!MPC_INEX_CMP (inex_re, inex_im, inex))
                printf("ternary value: got %s, expected (%s, %s)\n",
                       MPC_INEX_STR (inex),
                       MPFR_INEX_STR (inex_re), MPFR_INEX_STR (inex_im));
              OUT (op);
              printf ("     ");
              OUT (got);
              OUT (expected);

              exit (1);
            }
          break;

        case V_CC:
          read_cc (fp, &inex_re, &inex_im, z1, &signs, z2, &rnd);
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
                      function.name, line_number-1, rnd_mode[rnd]);
              OUT (op);
              printf ("     ");
              OUT (got);
              OUT (expected);

              exit (1);
            }
          break;

        case CCC:
          read_ccc (fp, &inex_re, &inex_im, z1, &signs, z2, z3, &rnd);
          mpfr_set_prec (MPC_RE(z4), MPC_PREC_RE (z1));
          mpfr_set_prec (MPC_IM(z4), MPC_PREC_IM (z1));
          inex = function.pointer.CCC (z4, z2, z3, rnd);
          if (!MPC_INEX_CMP (inex_re, inex_im, inex)
              || !same_mpc_value (z4, z1, signs))
            {
              /* display sensible variable names */
              mpc_t op1, op2, got, expected;
              op1[0] = z2[0];
              op2[0] = z3[0];
              expected[0]= z1[0];
              got[0] = z4[0];
              printf ("%s(op) failed (line %lu)\nwith rounding mode %s\n",
                      function.name, line_number-1, rnd_mode[rnd]);
              if (!MPC_INEX_CMP (inex_re, inex_im, inex))
                printf("ternary value: got %s, expected (%s, %s)\n",
                       MPC_INEX_STR (inex),
                       MPFR_INEX_STR (inex_re), MPFR_INEX_STR (inex_im));
              OUT (op1);
              OUT (op2);
              printf ("     ");
              OUT (got);
              OUT (expected);

              exit (1);
            }
          if (function.properties & FUNC_PROP_SYMETRIC)
            {
              inex = function.pointer.CCC (z4, z3, z2, rnd);
              if (!MPC_INEX_CMP (inex_re, inex_im, inex)
              || !same_mpc_value (z4, z1, signs))
                {
                  /* display sensible variable names */
                  mpc_t op1, op2, got, expected;
                  op1[0] = z3[0];
                  op2[0] = z2[0];
                  expected[0]= z1[0];
                  got[0] = z4[0];
                  printf ("%s(op) failed (line %lu/symetric test)\n"
                          "with rounding mode %s\n",
                          function.name, line_number-1, rnd_mode[rnd]);
                  if (!MPC_INEX_CMP (inex_re, inex_im, inex))
                    printf("ternary value: got %s, expected (%s, %s)\n",
                           MPC_INEX_STR (inex),
                           MPFR_INEX_STR (inex_re), MPFR_INEX_STR (inex_im));
                  OUT (op1);
                  OUT (op2);
                  printf ("     ");
                  OUT (got);
                  OUT (expected);

                  exit (1);
                }
            }
          break;

        case CFC:
          read_cfc (fp, &inex_re, &inex_im, z1, &signs, x1, z2, &rnd);
          mpfr_set_prec (MPC_RE(z3), MPC_PREC_RE (z1));
          mpfr_set_prec (MPC_IM(z3), MPC_PREC_IM (z1));
          inex = function.pointer.CFC (z3, x1, z2, rnd);
          if (!MPC_INEX_CMP (inex_re, inex_im, inex)
              || !same_mpc_value (z3, z1, signs))
            {
              /* display sensible variable names */
              mpc_t op2, got, expected;
              mpfr_t op1;
              op1[0] = x1[0];
              op2[0] = z2[0];
              expected[0]= z1[0];
              got[0] = z3[0];
              printf ("%s(op) failed (line %lu)\nwith rounding mode %s\n",
                      function.name, line_number-1, rnd_mode[rnd]);
              if (!MPC_INEX_CMP (inex_re, inex_im, inex))
                printf("ternary value: got %s, expected (%s, %s)\n",
                       MPC_INEX_STR (inex),
                       MPFR_INEX_STR (inex_re), MPFR_INEX_STR (inex_im));
              MPFR_OUT (op1);
              OUT (op2);
              printf ("     ");
              OUT (got);
              OUT (expected);

              exit (1);
            }
          break;

        case CCF:
          read_ccf (fp, &inex_re, &inex_im, z1, &signs, z2, x1, &rnd);
          mpfr_set_prec (MPC_RE(z3), MPC_PREC_RE (z1));
          mpfr_set_prec (MPC_IM(z3), MPC_PREC_IM (z1));
          inex = function.pointer.CCF (z3, z2, x1, rnd);
          if (!MPC_INEX_CMP (inex_re, inex_im, inex)
              || !same_mpc_value (z3, z1, signs))
            {
              /* display sensible variable names */
              mpc_t op1, got, expected;
              mpfr_t op2;
              op1[0] = z2[0];
              op2[0] = x1[0];
              expected[0]= z1[0];
              got[0] = z3[0];
              printf ("%s(op) failed (line %lu)\nwith rounding mode %s\n",
                      function.name, line_number-1, rnd_mode[rnd]);
              if (!MPC_INEX_CMP (inex_re, inex_im, inex))
                printf("ternary value: got %s, expected (%s, %s)\n",
                       MPC_INEX_STR (inex),
                       MPFR_INEX_STR (inex_re), MPFR_INEX_STR (inex_im));
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
      break;
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
