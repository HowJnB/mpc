/* mpc-impl.h -- Internal include file for mpc.

Copyright (C) 2002, 2004, 2005, 2008 Andreas Enge, Paul Zimmermann

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

#ifndef __MPC_IMPL_H
#define __MPC_IMPL_H

#define MPFR_PREC(x) mpfr_get_prec(x)
#define MPFR_EXP(x)  mpfr_get_exp(x)
#define MPFR_CHANGE_SIGN(x) mpfr_neg(x,x,GMP_RNDN)
#define MPFR_IS_ZERO(x) (mpfr_cmp_ui(x,0) == 0)

#define MAX(h,i) ((h) > (i) ? (h) : (i))

#ifndef MUL_KARATSUBA_THRESHOLD
#define MUL_KARATSUBA_THRESHOLD 23
#endif

#ifndef BITS_PER_MP_LIMB
#define BITS_PER_MP_LIMB mp_bits_per_limb
#endif

#define MPC_MAX_PREC(x) MAX(MPFR_PREC(MPC_RE(x)), MPFR_PREC(MPC_IM(x)))

#define INV_RND(r) \
   (((r) == GMP_RNDU) ? GMP_RNDD : (((r) == GMP_RNDD) ? GMP_RNDU : (r)))
#define SWAP(a,b) { mpfr_srcptr tmp; tmp = a; a = b; b = tmp; }
/* Safe absolute value (to avoid possible integer overflow) */
/* type is the target (unsigned) type (copied from mpfr-impl.h */
#define SAFE_ABS(type,x) ((x) >= 0 ? (type)(x) : -(type)(x))

/* Define internal functions */

#if defined (__cplusplus)
extern "C" {
#endif

  int  mpc_mul_naive     __MPC_PROTO ((mpc_ptr, mpc_srcptr, mpc_srcptr, mpc_rnd_t));
  int  mpc_mul_karatsuba __MPC_PROTO ((mpc_ptr, mpc_srcptr, mpc_srcptr, mpc_rnd_t));
  unsigned long  mpc_ceil_log2 __MPC_PROTO ((unsigned long));

#if defined (__cplusplus)
}
#endif

#endif
