/* mpc_get_str -- Convert a complex number into a string.

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

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "mpc.h"
#include "mpc-impl.h"

/* The output format is "(real imag)" */

static char *
prettify (const char *str, const mp_exp_t expo, int base, int special)
{
  size_t sz;
  char *pretty;
  char *p;
  const char *s;

  sz = strlen (str) + 1; /* + terminal '\0' */

  if (special)
    {
      /* special number: nan or inf or zero */
      pretty = (char *)malloc (sz * sizeof(char));
      strcpy (pretty, str);

      return pretty;
    }

  /* regular number */

  ++sz; /* + decimal point */

  if (expo != 1)
    {
      /* augment sz with the size needed for exponent in base ten */
      mp_exp_t x;

      sz += 3; /* + exponent char + sign + 1 digit */

      x = expo - 1; /* expo is the exponent value with decimal point BEFORE
                       the first digit, we wants decimal point AFTER the first
                       digit */

      if (x < 0)
        {
          /* avoid overflow when changing sign (assuming that, for the
             mp_exp_t type, (max value) is greater than (- min value / 10)) */
          if (x < -10)
            {
              x = - (x / 10);
              sz++;
            }
          else
            x = -x;
        }

      /* compute sz += floor(log(expo)/log(10)) without using libm
         functions */
      while (x > 9)
        {
          sz++;
          x /= 10;
        }
    }

  pretty = (char *)malloc (sz * sizeof(char));
  p = pretty;

  /* optional sign plus first digit */
  s = str;
  *p++ = *s++;
  if (str[0] == '-' || str[0] == '+')
    *p++ = *s++;

  /* decimal point */
  *p++ = '.';
  *p = '\0';

  /* other significant digits */
  strcat (pretty, s);

  /* exponent (in base ten) */
  if (expo == 1)
    return pretty;

  p = pretty + strlen (str) + 1;

  switch (base)
    {
    case 10:
      *p++ = 'e';
      break;
    case 2:
    case 16:
      *p++ = 'p';
      break;
    default:
      *p++ = '@';
    }

  *p = '\0';

  sprintf (p, "%+ld", expo - 1); /* TODO: mp_exp_t type is not fixed */

  return pretty;
}

char *
mpc_get_str (int base, size_t n, mpc_srcptr op, mpc_rnd_t rnd)
{
  size_t needed_size;
  mp_exp_t expo;
  char *uggly;
  int special;
  char *real_str;
  char *imag_str;
  char *complex_str = NULL;

  uggly = mpfr_get_str (NULL, &expo, base, n, MPC_RE (op), MPC_RND_RE (rnd));
  if (uggly == NULL)
    return NULL;
  special = !mpfr_number_p (MPC_RE (op)) || mpfr_zero_p (MPC_RE (op));
  real_str = prettify (uggly, expo, base, special);
  mpfr_free_str (uggly);
    
  uggly = mpfr_get_str (NULL, &expo, base, n, MPC_IM (op), MPC_RND_IM (rnd));
  if (uggly == NULL)
    {
      free (real_str); /* TODO: use gmp_free_func */
      return NULL;
    }
  special = !mpfr_number_p (MPC_IM (op)) || mpfr_zero_p (MPC_IM (op));
  imag_str = prettify (uggly, expo, base, special);
  mpfr_free_str (uggly);

  needed_size = strlen (real_str) + strlen (imag_str) + 4;
 
  /* TODO: use gmp_alloc_func */
  complex_str = (char *)malloc (needed_size * sizeof (char));
  if (complex_str == NULL)
    goto exit;

  strcpy (complex_str, "(");
  strcat (complex_str, real_str);
  strcat (complex_str, " ");
  strcat (complex_str, imag_str);
  strcat (complex_str, ")");

 exit:
  free (real_str); /* TODO: use gmp_free_func */
  free (imag_str);

  return complex_str;
}

void
mpc_free_str (char *str)
{
  /* TODO: use gmp_free_func */
  free (str);
}
