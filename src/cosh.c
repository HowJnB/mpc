/* mpc_cosh -- hyperbolic cosine of a complex number.

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

#include "mpfr.h"
#include "mpc.h"
#include "mpc-impl.h"

void
mpc_cosh (mpc_ptr rop, mpc_srcptr op, mpc_rnd_t rnd)
{
  /* cosh(op) = cos(i*op) */
  mpc_t z;

  mpc_init3 (z, MPFR_PREC (MPC_IM (op)), MPFR_PREC (MPC_RE (op)));

  mpc_mul_i (z, op, +1, MPC_RNDNN); /* exact */
  mpc_cos (rop, z, rnd);

  mpc_clear (z);
}
