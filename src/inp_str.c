/* mpc_inp_str -- Input a complex number from a given stream.

Copyright (C) 2002, 2009 Andreas Enge, Paul Zimmermann, Philippe Th\'eveny

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
#include "mpc-impl.h"

size_t
mpc_inp_str (mpc_ptr rop, FILE *stream, int base, mpc_rnd_t rnd_mode)
{
  size_t size, size_im;
  int c;
  int sign;

  if (stream == NULL)
    stream = stdin;

  /* input real part */
  size = mpfr_inp_str (MPC_RE(rop), stream, base, MPC_RND_RE(rnd_mode));
  if (size == 0) /* error while reading the real part */
    return 0;

  /* get 'i' or 'I' followed by '*' */
  do
    {
      c = getc (stream);
      size ++;
    }
  while (isspace (c));

  switch (c)
    {
    case '+':
      sign = 0;
      break;
    case '-':
      sign = 1;
      break;
    default:
      /* error */
      return 0;
    }

  c = getc (stream);
  size ++;
  if (c != 'I' && c != 'i')
    return 0; /* error */

  c = getc (stream);
  size ++;
  if (c != '*')
    return 0; /* error */

  size_im = mpfr_inp_str (MPC_IM(rop), stream, base, MPC_RND_IM(rnd_mode));
  if (size_im == 0) /* error while reading the imaginary part */
    return 0;

  if (sign)
    mpfr_setsign (MPC_IM (rop), MPC_IM (rop), sign, GMP_RNDN);
  
  return size + size_im;
}
