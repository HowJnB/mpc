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
#define IS_IN(c, set) ((c) != '\0' && strchr ((set), tolower ((c))) != NULL)

/* Needs <locale.h> */
#ifdef HAVE_LOCALE_H
#include <locale.h>
#endif

static char *decimal_point;

static size_t
read_special (const char* p, const int base)
{
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
              for (q = p + 6; *q != ')' && *q != '\0'; ++q);
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
          if (p[5] == '(')
            {
              /* skip "(n-char-sequence)", see documentation of strtod */
              const char *q;
              for (q = p + 4; *q != ')' && *q != '\0'; ++q);
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
  const char all_digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";
  char ten_digits[] = "0123456789";
  char *digits = NULL;
  const char *ptr = str;
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

  if (true_base == 10)
    digits = ten_digits;
  else
    {
      /* set the set of digits in base BASE */
      digits = malloc ((true_base+1) * sizeof (char));
      if (digits == NULL) goto error;
      strncpy (digits, all_digits, true_base);
      digits[true_base] = '\0';
    }

  /* read decimal part */
  if (IS_IN (*ptr, decimal_point))
      {
        /* when a floating-point number start with the decimal point, it must be
           followed by at least one digit */
        for (++ptr; IS_IN (*ptr, digits); ++ptr);
        if (ptr == str + 1)
          goto error;
      }
    else if (IS_IN (*ptr, digits))
      {
        /* the number begins with at least one digit */
        for (++ptr; IS_IN (*ptr, digits); ++ptr);

        if (IS_IN (*ptr, decimal_point))
            for (++ptr; IS_IN (*ptr, digits); ++ptr);
      }
    else
      goto error;

  /* read exponent part */
  if (true_base != 10)
    {
      /* exponent number are always in base ten */
      free (digits);
      digits = ten_digits;
    }
  if (is_exponent_char (*ptr, true_base))
    {
      ++ptr;
      if (*ptr =='+' || *ptr == '-')
        ++ptr;
      if (IS_IN (*ptr, digits))
        for (++ptr; IS_IN (*ptr, digits); ++ptr);
      else
        goto error;
    }

  return ptr - str;

 error:
  if (digits != NULL && digits != ten_digits)
    free (digits);

  return 0;
}

static size_t
read_part (const char * const str, const int base, char **number, int* part_type)
/* Find in the string STR the first readable number written in base BASE.
   Return the number of characters in the part (spaces included).
   NUMBER is NULL if no number can be read, otherwise it points to a new
   allocated string where the sign and digits of the number are
   packed (without spaces).
   Try to determine to which part it belongs:
   *part_type == 0: real part
   *part_type == 1: imaginary part
   *part type == 2: cannot determine, an isolated 'I' is ambiguous
   in bases more than eighteen.
   *part_type == 3: unknown, when read_part failed

   WARNING: The user will have to free the string pointed by NUMBER.*/
{
  size_t n;
  const char *p;
  int type = 3; /* unknown */
  char sign = '\0';

  for (p = str; isspace (*p); ++p);

  if (*p == '+' || *p == '-')
    {
      sign = *p;
      for (++p; isspace (*p); ++p);
    }

  if (tolower (*p) == 'i')
    {
      if (base <= 18)
        {
          if ((n = read_special (p, base)) == 0)
            {
              /* it is not 'inf' or 'infinity' */
              for (++p; isspace (*p); ++p);

              switch (*p)
                {
                case '+': case '-': case '\0':
                  {
                    n = (sign != '\0') ? 3 : 2;
                    *number = malloc (n * sizeof (char));
                    if (*number == NULL) return 0;
                    (*number)[0] = sign;
                    (*number)[1] = '\0';
                    strncat (*number, "1", 1);
                  }
                  *part_type = 1;

                  return p - str;

                case '*':
                  for (++p; isspace (*p); ++p);
                  type = 1; /* imaginary part */
                  break;

                default:
                  /* error: characters other than +-* cannot follow an
                     isolated 'I' */
                  *part_type = 3;
                  *number = NULL;

                  return 0;
                }
            }
        }
      else
        {
          if (isspace (p[1]))
            {
              for (++p; isspace (*p); ++p);

              switch (*p)
                {
                case '+': case '-': case '\0':
                  {
                    n = (sign != '\0') ? 3 : 2;
                    *number = malloc (n * sizeof (char));
                    if (*number == NULL) return 0;
                    (*number)[0] = sign;
                    (*number)[1] = '\0';
                    strncat (*number, "I", 1);
                  }
                  *part_type = 2; /* isolated 'I' */

                  return p - str;

                case '*':
                  type = 1; /* imaginary part */
                  for (++p; isspace (*p); ++p);
                  break;

                default:
                  /* error: characters other than +-* cannot follow an
                     isolated 'I' */
                  *part_type = 3;
                  *number = NULL;

                  return 0;
                } 
            }
          else
            switch (p[1])
              {
              case '+': case '-': case '\0':
                {
                  n = (sign != '\0') ? 3 : 2;
                  *number = malloc (n * sizeof (char));
                  if (*number == NULL) return 0;
                  (*number)[0] = sign;
                  (*number)[1] = '\0';
                  strncat (*number, "I", 1);
                }
                *part_type = 2; /* isolated 'I' */
                ++p;            /* 'i' */
                return p - str;

              case '*':
                type = 1; /* imaginary part */
                ++p;      /* 'i' */
                ++p;      /* '*' */
                for (; isspace (*p); ++p);
              }
        }
    }

  n = read_number (p, base);
  if (n == 0)
    {
      if (type == 1)
        {
          n = (sign != '\0') ? 3 : 2;
          *number = malloc (n * sizeof (char));
          if (*number == NULL) return 0;
          (*number)[0] = sign;
          (*number)[1] = '\0';
          strncat (*number, "1", 1);
          *part_type = 1;
          return p - str;
        }
      else
        {
          *number = NULL;
          return 0;
        }
    }
  /* copy sign and number into a new string */
  if (sign != '\0')
    *number = malloc ((n+2) * sizeof (char));
  else
    *number = malloc ((n+1) * sizeof (char));
  if (*number == NULL) return 0;
  (*number)[0] = sign;
  (*number)[1] = '\0';
  strncat (*number, p, n);

  p += n;

  if (type != 1)
    {
      /* we have read a number without "I*" before it, let's see if we can
         find a "*I" after it.*/
      for (; isspace (*p); ++p);
      switch (*p)
        {
        case '+': case '-': case '\0':
          type = 0; /* real part */
          break;

        case '*':
          for (++p; isspace (*p); ++p);
          if (tolower (*p) == 'i')
            {
              for (++p; isspace (*p); ++p);
              type = 1; /* imaginary part */
            }
          else
            {
              /* error: no character other than 'I' can follow "number*" */
              *part_type = 3;
              *number = NULL;

              return 0;
            }
        }
    }
  *part_type = (type == 3 ? 0 : type);

  return p - str;
}

int
mpc_strtoc (mpc_ptr rop, char *nptr, char **endptr, int base, mpc_rnd_t rnd)
{
  char *p;
  char *end;

  char *p1 = NULL;
  int type1 = 3;
  size_t size1 = 0;
  char *p2 = NULL;
  int type2 = 3;
  size_t size2 = 0;

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
  if ((size1 = read_part (p, base, &p1, &type1)) == 0)
    goto error;
  p += size1;

  for (; isspace (*p); ++p);

  /* which caracter between the two parts ? */
  if (*p != '+' && *p != '-')
    type2 = 3; /* not read */
  else
    size2 = read_part (p, base, &p2, &type2);

  /* table used for determination of the complex value

     -------   type1=0      type=1     type1=2
     type2=0   p1+i*0      p2+i*p1   p2+s1*i*1
     type2=1   p1+i*p2      0+i*p1   p1+   i*p2
     type2=2   p1+s2*i*1   p2+i*p1      (A)
     type2=3   p1+i*0       0+i*p1      s1*i*1

     type=0: real, 1: imaginary, 2: isolated 'I', 3: unread
     s1: sign of p1, s2: sign of p2

     (A): if s1 == s2 then p1+s2*i*1 else error
  */

  /* set part2 */
  if (type2 == 3)
    /* no part2 */
    switch (type1)
      {
      case 0:
        inex_im = mpfr_set_ui (MPC_IM (rop), 0, GMP_RNDN);
        break;
      case 1:
        inex_re = mpfr_set_ui (MPC_RE (rop), 0, GMP_RNDN);
        break;
      case 2: /* part1 is considered as +-1*i */
        type1 = 1; /* imaginary part */
        p1[p1[0] == 'I' ? 0 : 1] = '1'; /* preserve sign */
        inex_re = mpfr_set_ui (MPC_RE (rop), 0, GMP_RNDN);
      }
  else if (type1 == type2)
    switch (type1)
      {
      case 0: /* both parts are real */
        /* keep the first one and forget the other one */
        inex_im = mpfr_set_ui (MPC_IM (rop), 0, MPC_RND_IM (rnd));
        free (p2);
        break;
      case 1: /* both parts are imaginary */
        /* keep the first one and forget the other one */
        inex_re = mpfr_set_ui (MPC_RE (rop), 0, MPC_RND_RE (rnd));
        free (p2);
        break;
      case 2: /* both parts are isolated I (base >= 19) */
        if ((p1[0] == '-' && p2[0] != '-')
            || (p1[0] != '-' && p2[0] == '-'))
          {
            /* the ambiguity cannot be solved: they do not share the same
               sign */
            free (p1);
            free (p2);
            goto error;
          }
        /* the second 'I' is considered as +-1*i */
        p2[p2[0] == 'I' ? 0 : 1] = '1'; /* preserve sign */
        type1 = 0; /* real part */
        inex_im =
          mpfr_strtofr (MPC_IM (rop), p2, &end, base, MPC_RND_IM (rnd));
        free (p2);
        if (end == p2)
          {
            /* mpfr cannot read the (modified) string p2 */
            free (p1);
            goto error;
          }
        p += size2;
      }
  else
    {
      /* real and imaginary parts can be identified */
      switch (type1)
        {
        case 0:
          if (type2 == 2)
            p2[p2[0] == 'I' ? 0 : 1] = '1'; /* preserve sign */
          inex_im =
            mpfr_strtofr (MPC_IM (rop), p2, &end, base, MPC_RND_IM (rnd));
          break;
        case 1:
          inex_re =
            mpfr_strtofr (MPC_RE (rop), p2, &end, base, MPC_RND_RE (rnd));
          break;
        case 2:
          if (type2 == 0)
            {
              type1 = 1; /* imaginary part */
              p1[p1[0] == 'I' ? 0 : 1] = '1'; /* preserve sign */
              inex_re =
                mpfr_strtofr (MPC_RE (rop), p2, &end, base, MPC_RND_RE (rnd));
            }
          else
            {
              type1 = 0; /* real part */
              inex_im =
                mpfr_strtofr (MPC_IM (rop), p2, &end, base, MPC_RND_IM (rnd));
            }
        }          

      free (p2);
      if (end == p2)
        {
          /* mpfr cannot read the string p2 */
          free (p1);
          goto error;
        }

      p += size2;
    }

  /* set part 1 */
  if (type1 == 0)
    inex_re = mpfr_strtofr (MPC_RE (rop), p1, &end, base, MPC_RND_RE (rnd));
  else
    inex_im = mpfr_strtofr (MPC_IM (rop), p1, &end, base, MPC_RND_IM (rnd));
  free (p1);
  if (end == p1)
    /* mpfr cannot read the string p1 */
    goto error;

  if (endptr != NULL)
    *endptr = p;
  return MPC_INEX (inex_re, inex_im);

 error:
  if (endptr != NULL)
    *endptr = nptr;
  return mpc_set_ui_ui (rop, 0, 0, rnd);
}
