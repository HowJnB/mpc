/* mpc_proj -- projection of a complex number onto the Riemann sphere.

Copyright (C) INRIA, 2008, 2009, 2011

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

#include "mpc-impl.h"

int
mpc_proj (mpc_ptr a, mpc_srcptr b, mpc_rnd_t rnd)
{
   if (mpc_inf_p (b)) {
      /* infinities project to +Inf +i* copysign(0.0, cimag(z)) */
      mpfr_set_inf (MPC_RE (a), +1);
      mpfr_set_zero (MPC_IM (a), (mpfr_signbit (MPC_IM (b)) ? -1 : 1));
      return MPC_INEX (0, 0);
   }
   else
      return mpc_set (a, b, rnd);
}
