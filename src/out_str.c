/* mpc_out_str -- Output a complex number on a given stream.

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
mpc_out_str (FILE *stream, int base, size_t n, mpc_srcptr op, mpc_rnd_t rnd)
{
  mpfr_t positive_imag;
  size_t size;

  if (stream == NULL)
    stream = stdout; /* fprintf does not allow NULL as first argument */

  size = mpfr_out_str (stream, base, n, MPC_RE(op), MPC_RND_RE(rnd));

  positive_imag[0] = MPC_IM (op)[0];
  if (mpfr_signbit (MPC_IM (op)))
    {
      size += fprintf (stream, " -I*");
      mpfr_setsign (positive_imag, positive_imag, 0, GMP_RNDN);
    }
  else
    size += fprintf (stream, " +I*");
  size += mpfr_out_str (stream, base, n, positive_imag, MPC_RND_IM(rnd));

  return size;
}
