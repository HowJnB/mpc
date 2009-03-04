/* mpc_strtoc -- Read a complex number from a stream.

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

#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include "mpc.h"
#include "mpc-impl.h"

#include "config.h"

/* Check if c is in the set (case insensitive) */
#define IS_IN_CI(c, set) ((c) != '\0' && strchr ((set), tolower ((c))) != NULL)
/* Check if c is in the set (case sensitive) */
#define IS_IN(c, set) ((c) != '\0' && strchr ((set), (c)) != NULL)

/* Needs <locale.h> */
#ifdef HAVE_LOCALE_H
#include <locale.h>
#endif

static char *decimal_point;

static size_t
read_special (const char* p, const int base)
{
  char seq_char[] = "0123456789abcdefghijklmnopqrstuvwxyz_";

  if (*p == '@')
    {
      /* all bases: accept "@nan@" and "@inf@" (case insensitive) */
      if (tolower (p[1]) == 'i'
          && tolower (p[2]) == 'n'
          && tolower (p[3]) == 'f'
          && p[4] == '@')
        return 5;
      else if (tolower (p[1]) == 'n'
               && tolower (p[2]) == 'a'
               && tolower (p[3]) == 'n'
               && p[4] == '@')
        {
          if (p[5] == '(')
            {
              /* skip "(n-char-sequence)", see documentation of strtod */
              const char *q;
              for (q = p + 6; IS_IN_CI (*q, seq_char) ; ++q);
              if (*q == ')')
                return q + 1 - p;
              else
                return 5;
            }
          else
            return 5;
        }
    }

  if (base <= 10)
    {
      /* accept "nan", "inf" and "infinity" (case insensitive) */
      if (tolower (p[0]) == 'n'
          && tolower (p[1]) == 'a'
          && tolower (p[2]) == 'n')
        {
          if (p[3] == '(')
            {
              /* skip "(n-char-sequence)", see documentation of strtod */
              const char *q;
              for (q = p + 4; IS_IN_CI (*q, seq_char); ++q);
              if (*q == ')')
                return q + 1 - p;
              else
                return 3;
            }
          else
            return 3;
        }

      if (tolower (p[0]) == 'i'
          && tolower (p[1]) == 'n'
          && tolower (p[2]) == 'f')
        {
          if (tolower (p[3])== 'i'
              && tolower (p[4]) == 'n'
              && tolower (p[5]) == 'i'
              && tolower (p[6]) == 't'
              && tolower (p[7]) == 'y')
            return 8;
          else
            return 3;
        }
    }

  return 0;
}

static int
is_exponent_char (const char c, const int base)
{
  switch (tolower (c))
    {
    case 'e':
      return base <= 10;
    case 'p':
      return base == 2 || base == 16;
    }
  return c == '@';
}

static size_t
read_number (const char * const str, const int base)
/* Try to read a floating-point number written in base BASE starting from
   the very begining of STR (no space allowed).  Return the number of
   characters making up that number. */
{
  char digits[63] = "0123456789";
  const char *ptr = str;
  const char *end_fp;
  int true_base = base;

  /* special number ? */
  switch (*ptr)
    {
    case '@':
      return read_special (ptr, (base == 0 ? 10 : base));
    case 'i': case 'I':
    case 'n': case 'N':
      if (base <= 10)
        return read_special (ptr, (base == 0 ? 10 : base));
      break;
    case '\0':
      return 0;
    }

  /* skip optional prefix */
  if (ptr[0] == '0')
    switch (base)
      {
      case 0:
        switch (tolower (ptr[1]))
          {
          case 'x':
            true_base = 16;
            ptr += 2;
            break;
          case 'b':
            true_base = 2;
            ptr += 2;
          }
        break;
      case 2:
        if (tolower (ptr[1]) == 'b')
          ptr += 2;
        break;
      case 16:
        if (tolower (ptr[1]) == 'x')
          ptr += 2;
      }

  if (true_base != 10)
    {
      /* set the set of digits in base BASE */
      const char all_digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
      int i;

      for (i = 0; i < true_base; ++i)
        digits[i] = all_digits[i];

      if (true_base > 10 && true_base < 37)
        {
          /* digits can be in lower case */
          for (i = 0; i < true_base - 10; ++i)
            digits[true_base + i] = all_digits[36 + i];

          digits[2*true_base - 10] = '\0';
        }
      else
        digits[true_base] = '\0';
    }

  /* read decimal part */
  if (IS_IN (*ptr, decimal_point))
    {
      /* when a floating-point number start with the decimal point, it must be
         followed by at least one digit */
      for (++ptr; IS_IN (*ptr, digits); ++ptr);
      if (ptr == str + 1)
        return 0;
    }
  else if (IS_IN (*ptr, digits))
    {
      /* the number begins with at least one digit */
      for (++ptr; IS_IN (*ptr, digits); ++ptr);

      if (IS_IN (*ptr, decimal_point))
        for (++ptr; IS_IN (*ptr, digits); ++ptr);
    }
  else
    return 0;

  /* read exponent part */
  end_fp = ptr;

  if (is_exponent_char (*ptr, true_base))
    {
      ++ptr;
      /* exponent number are always in base ten */
      if (true_base < 10)
        {
          digits[2] = '2';
          digits[3] = '3';
          digits[4] = '4';
          digits[5] = '5';
          digits[6] = '6';
          digits[7] = '7';
          digits[8] = '8';
          digits[9] = '9';
          digits[10] = '\0';
        }
      else if (true_base > 10)
        digits[10] = '\0';

      if (*ptr =='+' || *ptr == '-')
        ++ptr;
      if (IS_IN (*ptr, digits))
        for (++ptr; IS_IN (*ptr, digits); ++ptr);
      else
        return end_fp - str;
    }

  return ptr - str;
}

static size_t
read_part (const char * const str, const int base)
/* Find in the string STR the first readable number written in base BASE.
   Return the number of characters in the part (spaces not included)
   or zero if STR doesn't point to a floating-point number. */
{
  size_t n;
  const char *p;

  p = str;
  if (*p == '+' || *p == '-')
      p++;

  if ((n = read_number (p, base)) == 0)
    return 0;

  p += n;

  return p - str;
}

int
mpc_strtoc (mpc_ptr rop, char *nptr, char **endptr, int base, mpc_rnd_t rnd)
{
  char *p;
  char *end;

  char *real_str = NULL;
  size_t real_size = 0;
  char *imag_str = NULL;
  size_t imag_size = 0;
  int bracketed = 0;

  int inex_re = 0;
  int inex_im = 0;

  if (base != 0 && (base < 2 || base > 62))
    goto error;

#ifdef HAVE_LOCALE_H
  decimal_point = localeconv ()->decimal_point;
#else
  decimal_point = ".";
#endif

  p = nptr;
  for (p = nptr; isspace (*p); ++p);

  if (*p == '(')
    {
      bracketed = 1;
      ++p;
    }

  real_str = p;
  real_size = read_part (p, base);
  if (real_size == 0)
    goto error;
  p += real_size;

  if (bracketed)
    {
      if (!isspace (*p))
        goto error;

      for (++p; isspace (*p); ++p);

      imag_str = p;
      imag_size = read_part (p, base);
      if (imag_size == 0)
        goto error;
      p += imag_size;

      if (*p != ')')
        goto error;

      p++;

      inex_im = mpfr_strtofr (MPC_IM (rop), imag_str, &end, base, MPC_RND_IM (rnd));
      if (imag_str + imag_size != end)
        goto error;
    }
  else
    inex_im = mpfr_set_ui (MPC_IM (rop), 0, MPC_RND_IM(rnd));

  for (; isspace (*p); ++p);

  inex_re = mpfr_strtofr (MPC_RE(rop), real_str, &end, base, MPC_RND_RE (rnd));
  if (real_str + real_size != end)
    goto error;

  if (endptr != NULL)
    *endptr = p;
  return MPC_INEX (inex_re, inex_im);

 error:
  if (endptr != NULL)
    *endptr = nptr;
  mpfr_set_nan (MPC_RE (rop));
  mpfr_set_nan (MPC_IM (rop));
  return 0;
}
