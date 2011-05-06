/* tswap.c -- Test file for mpc_swap.

Copyright (C) INRIA, 2011

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

#include "mpc-tests.h"

int
main (void)
{
   mpc_t x, y, x2, y2;

   mpc_init2 (x, 50);
   mpc_init2 (x2, 50);
   mpc_init2 (y, 100);
   mpc_init2 (y2, 100);

   mpc_set_ui_ui (x,  1ul, 2ul, MPC_RNDNN);
   mpc_set_ui_ui (x2, 1ul, 2ul, MPC_RNDNN);
   mpc_set_ui_ui (y,  3ul, 4ul, MPC_RNDNN);
   mpc_set_ui_ui (y2, 3ul, 4ul, MPC_RNDNN);

   mpc_swap (x, y);

   if (   mpfr_get_prec (MPC_RE (x)) != mpfr_get_prec (MPC_RE (y2))
       || mpfr_get_prec (MPC_IM (x)) != mpfr_get_prec (MPC_IM (y2))
       || mpfr_get_prec (MPC_RE (y)) != mpfr_get_prec (MPC_RE (x2))
       || mpfr_get_prec (MPC_IM (y)) != mpfr_get_prec (MPC_IM (x2))
       || mpc_cmp (x, y2) != 0
       || mpc_cmp (y, x2) != 0)
      exit (1);

   mpc_clear (x);
   mpc_clear (x2);
   mpc_clear (y);
   mpc_clear (y2);

   return 0;
}
