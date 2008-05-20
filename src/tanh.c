/* mpc_tanh -- hyperbolic tangent of a complex number.

Copyright (C) 2008 Philippe Th\'eveny.

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
mpc_tanh (mpc_ptr rop, mpc_srcptr op, mpc_rnd_t rnd)
{
  /* tanh(op) = -i*tan(i*op) = conj(-i*tan(conj(-i*op))) */
  mpc_t z;
  mpc_t tan_z;
  mpc_rnd_t rnd_z;

  rnd_z = RNDC (MPC_RND_IM (rnd), MPC_RND_RE (rnd));

  mpc_init3 (z, MPFR_PREC (MPC_IM (op)), MPFR_PREC (MPC_RE (op)));
  mpc_init3 (tan_z, MPFR_PREC (MPC_IM (rop)), MPFR_PREC (MPC_RE (rop)));

  /* z := conj(-i * op), note that the real and imaginary precisions
     may differ. */
  mpfr_set (MPC_RE (z), MPC_IM (op), GMP_RNDN); /* exact */
  mpfr_set (MPC_IM (z), MPC_RE (op), GMP_RNDN); /* exact */

  /* rop = conj(-i * tan(z)) */
  mpc_tan (tan_z, z, rnd_z);

  mpfr_set (MPC_RE (rop), MPC_IM (tan_z), GMP_RNDN); /* exact */
  mpfr_set (MPC_IM (rop), MPC_RE (tan_z), GMP_RNDN); /* exact */

  mpc_clear (z);
  mpc_clear (tan_z);
}
