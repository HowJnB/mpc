/* mpc-impl.h -- Internal include file for mpc.

Copyright (C) 2002, 2004 Andreas Enge, Paul Zimmermann

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

/* Check version of MPFR:
   + 2.0.1:
          Define mpfr_cmp_abs and check for zero.
	  Format.
	  ceil_log2
	  IS_ZERO
	  Change_sign
   + 2.0.2 / 2.0.3:
          Format.
	  set_str_raw
	  ceil_log2
	  IS_ZERO
	  Change_sign
   + 2.1.0: 
          Format 
	  set_str_raw
	  ceil_log2
	  IS_ZERO
	  Change_sign
   + Generic:
   
*/
#if !defined(mpfr_round_prec) /* MPFR 2.0.1 */

#define MPFR_CMP_ABS(a,b) \
      ((MPFR_IS_ZERO (a)) ? 0 : ((MPFR_IS_ZERO (b)) ? 1 : mpfr_cmp_abs(a, b)))
#define NEED_CMP_ABS_PROTO
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

#elif !defined(MPFR_VERSION)  /* MPFR 2.0.2 / 2.0.3 */

#if defined (_CRAY) && ! defined (_CRAYMPP)
typedef unsigned int            mp_size_unsigned_t;
#else
typedef unsigned long int       mp_size_unsigned_t;
#endif
#define mpfr_set_str_raw(x, s) mpfr_set_str(x,s,2,GMP_RNDN)
#define MPFR_CMP_ABS mpfr_cmp_abs
#define MPFR_PREC(x) ((x)->_mpfr_prec)
#define MPFR_SIZE(x) ((x)->_mpfr_size)
#define MPFR_MANT(x) ((x)->_mpfr_d)
#define MPFR_EXP(x) ((x)->_mpfr_exp)
#define MPFR_CHANGE_SIGN(x) (MPFR_SIZE(x) ^= (((mp_size_unsigned_t) 1) << 31))
#define MPFR_IS_ZERO(x) \
  (MPFR_MANT(x)[(MPFR_PREC(x)-1)/BITS_PER_MP_LIMB] == (mp_limb_t) 0)

#elif (MPFR_VERSION == MPFR_VERSION_NUM(2,1,0)) /* MPFR 2.1.0 */

#define mpfr_set_str_raw(x, s) mpfr_set_str(x,s,2,GMP_RNDN)
#define MPFR_CMP_ABS mpfr_cmp_abs
#define MPFR_PREC(x) ((x)->_mpfr_prec)
#define MPFR_EXP(x) ((x)->_mpfr_exp)
#define MPFR_CHANGE_SIGN(x) (MPFR_SIGN (x) = - MPFR_SIGN (x))
#if __GMP_MP_SIZE_T_INT
# define MPFR_EXP_ZERO ((mp_exp_t)((~((~(unsigned int)0)>>1))+1))
#else
# define MPFR_EXP_ZERO ((mp_exp_t)((~((~(unsigned long)0)>>1))+1))
#endif
#define MPFR_IS_ZERO(x) (MPFR_EXP(x) == MPFR_EXP_ZERO)

#else /* Generic MPFR code */

#define mpfr_set_str_raw(x, s) mpfr_set_str(x,s,2,GMP_RNDN)
#define MPFR_CMP_ABS mpfr_cmp_abs
#define MPFR_PREC(x) mpfr_get_prec(x)
#define MPFR_EXP(x)  mpfr_get_exp(x)
#define MPFR_CHANGE_SIGN(x) mpfr_neg(x,x,GMP_RNDN)
#define MPFR_IS_ZERO(x) mpfr_cmp_ui(x,0)

#endif

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

/* Define internal functions */

#if defined (__cplusplus)
extern "C" {
#endif

  int  mpc_mul_naive     __MPC_PROTO ((mpc_ptr, mpc_srcptr, mpc_srcptr, mpc_rnd_t));
  int  mpc_mul_karatsuba __MPC_PROTO ((mpc_ptr, mpc_srcptr, mpc_srcptr, mpc_rnd_t));
  unsigned long  mpc_ceil_log2 __MPC_PROTO ((unsigned long));

  /* forgotten in mpfr.h from GMP 4.1 */
#ifdef NEED_CMP_ABS_PROTO
  extern int mpfr_cmp_abs __MPC_PROTO ((mpfr_srcptr, mpfr_srcptr));
#endif

#if defined (__cplusplus)
}
#endif

#endif
