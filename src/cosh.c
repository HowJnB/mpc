/* mpc_cosh -- hyperbolic cosine of a complex number.

Copyright (C)  2008, 2009 INRIA

This file is part of GNU MPC.

GNU MPC is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the
Free Software Foundation; either version 3 of the License, or (at your
option) any later version.

GNU MPC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for
more details.

You should have received a copy of the GNU Lesser General Public License
along with this program. If not, see http://www.gnu.org/licenses/ .
*/

#include "mpc-impl.h"

int
mpc_cosh (mpc_ptr rop, mpc_srcptr op, mpc_rnd_t rnd)
{
  /* cosh(op) = cos(i*op) */
  mpc_t z;

  /* z = i*op without copying significand */
  MPC_RE (z)[0] = MPC_IM (op)[0];
  MPC_IM (z)[0] = MPC_RE (op)[0];
  MPFR_CHANGE_SIGN (MPC_RE (z));

  return mpc_cos (rop, z, rnd);
}
