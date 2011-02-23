/* mpc_get_version -- MPC version

Copyright (C) INRIA, 2008, 2009, 2010, 2011

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

#if MPFR_VERSION_MAJOR < 3
/* The following are functions defined for compatibility with mpfr < 3;
   logically, they should be defined in a separate file, but then gcc
   complains about an empty translation unit with mpfr >= 3.            */

void
mpfr_set_zero (mpfr_ptr z, int s)
{
   mpfr_set_ui (z, 0ul, GMP_RNDN);
   if (s < 0)
      mpfr_neg (z, z, GMP_RNDN);
}

int
mpfr_regular_p (mpfr_srcptr z)
{
   return (mpfr_number_p (z) && !mpfr_zero_p (z));
}
#endif /* mpfr < 3 */


const char *
mpc_get_version (void)
{
  return "0.9.1-dev";
}
