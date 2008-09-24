/* tfr_div -- test file for mpc_fr_sub.

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

#include <stdio.h>
#include <stdlib.h>
#include "gmp.h"
#include "mpfr.h"
#include "mpc.h"

static int
mpc_fr_sub_ref (mpc_t q, mpfr_t x, mpc_t z, mpc_rnd_t rnd);

#include "random.c"
#define TEST_FUNCTION mpc_fr_sub
#define REFERENCE_FUNCTION mpc_fr_sub_ref
#include "tgeneric_cfc.c"

static int
mpc_fr_sub_ref (mpc_t q, mpfr_t x, mpc_t z, mpc_rnd_t rnd)
{
  int inexact;
  mpc_t y;
  mpc_init2 (y, MPFR_PREC (x));
  mpc_set_fr (y, x, MPC_RNDNN);

  inexact = mpc_sub (q, y, z, rnd);

  mpc_clear (y);
  return inexact;
}

int
main (void)
{
  test_start();

  tgeneric (2, 4096, 7, 0);

  test_end ();
  return 0;
}
