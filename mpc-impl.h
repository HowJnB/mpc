/* mpc-impl.h -- Internal include file for mpc.

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

/* stolen from gmp-impl.h */
#define MAX(h,i) ((h) > (i) ? (h) : (i))

#ifndef MUL_KARATSUBA_THRESHOLD
#define MUL_KARATSUBA_THRESHOLD 23
#endif

#ifndef BITS_PER_MP_LIMB
#define BITS_PER_MP_LIMB mp_bits_per_limb
#endif


/* avoids to include mpfr-impl.h */
/* This unsigned type must correspond to the signed one defined in gmp.h */
#if defined (_CRAY) && ! defined (_CRAYMPP)
typedef unsigned int            mp_size_unsigned_t;
#else
typedef unsigned long int       mp_size_unsigned_t;
#endif
#define MPFR_PREC(x) ((x)->_mpfr_prec)
#define MPFR_SIZE(x) ((x)->_mpfr_size)
#define MPFR_MANT(x) ((x)->_mpfr_d)
#define MPFR_EXP(x) ((x)->_mpfr_exp)
#define MPFR_CHANGE_SIGN(x) (MPFR_SIZE(x) ^= (((mp_size_unsigned_t) 1) << 31))
#define MPFR_IS_ZERO(x) \
  (MPFR_MANT(x)[(MPFR_PREC(x)-1)/BITS_PER_MP_LIMB] == (mp_limb_t) 0)
extern long _mpfr_ceil_log2 _PROTO((double));

#define MPC_MAX_PREC(x) MAX(MPFR_PREC(MPC_RE(x)), MPFR_PREC(MPC_IM(x)))

/* forgotten in mpfr.h from GMP 4.1 */
int mpfr_cmp_abs _PROTO ((mpfr_srcptr, mpfr_srcptr));
