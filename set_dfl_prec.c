/* mpc_set_default_prec, mpc_get_default_prec -- set/get default precision

Copyright (C) 2002 Andreas Enge, Paul Zimmermann

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

#include "gmp.h"
#include "mpfr.h"
#include "mpc.h"

/* default is 53 bits */
mp_prec_t __mpc_default_fp_bit_precision = 53;

void
mpc_set_default_prec (mp_prec_t prec)
{
  __mpc_default_fp_bit_precision = prec;
}

mp_prec_t
mpc_get_default_prec (void)
{
  return __mpc_default_fp_bit_precision;
}
