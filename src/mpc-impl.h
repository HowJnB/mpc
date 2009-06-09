/* mpc-impl.h -- Internal include file for mpc.

Copyright (C) 2002, 2004, 2005, 2008, 2009 Andreas Enge, Philippe Th\'eveny, Paul Zimmermann

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

#include <stdlib.h>

#include "config.h" /* for MPC_USE_LOGGING */

#ifndef __MPC_TESTS_H
#define __MPC_WITHIN_MPC 1
#endif

#include "mpc.h"

#define MPC_RE(x) ((x)->re)
#define MPC_IM(x) ((x)->im)

/*
 * Miscelaneous useful macros
 */

#define MPC_MAX(h,i) ((h) > (i) ? (h) : (i))

/* Safe absolute value (to avoid possible integer overflow) */
/* type is the target (unsigned) type (copied from mpfr-impl.h) */
#ifdef SAFE_ABS
#undef SAFE_ABS
#endif
#define SAFE_ABS(type,x) ((x) >= 0 ? (type)(x) : -(type)(x))


/*
 * MPFR constants and macros
 */

#ifndef BITS_PER_MP_LIMB
#define BITS_PER_MP_LIMB mp_bits_per_limb
#endif

#define MPFR_PREC(x) mpfr_get_prec(x)
#define MPFR_EXP(x)  mpfr_get_exp(x)

#define MPFR_CHANGE_SIGN(x) mpfr_neg(x,x,GMP_RNDN)
#define MPFR_IS_SINGULAR(x) (mpfr_nan_p(x) || mpfr_inf_p(x) || mpfr_zero_p(x))

#define MPFR_SWAP(a,b) { mpfr_srcptr tmp; tmp = a; a = b; b = tmp; }


/*
 * MPC macros
 */

#define MPC_PREC_RE(x) (MPFR_PREC(MPC_RE(x)))
#define MPC_PREC_IM(x) (MPFR_PREC(MPC_IM(x)))
#define MPC_MAX_PREC(x) MPC_MAX(MPC_PREC_RE(x), MPC_PREC_IM(x))

#define INV_RND(r) \
   (((r) == GMP_RNDU) ? GMP_RNDD : (((r) == GMP_RNDD) ? GMP_RNDU : (r)))

/* Convention in C99 (G.3): z is regarded as an infinity if at least one of
   its parts is infinite */
#define MPC_IS_INF(z) (mpfr_inf_p(MPC_RE(z))||mpfr_inf_p(MPC_IM(z)))
/* Convention in C99 (G.3): z is regarded as a zero if each of its parts is
   a zero */
#define MPC_IS_ZERO(z) (mpfr_zero_p(MPC_RE(z))&&mpfr_zero_p(MPC_IM(z)))

#define OUT(x)                                                  \
do {                                                            \
  printf (#x "[%lu,%lu]=", (unsigned long int) MPC_PREC_RE (x), \
      (unsigned long int) MPC_PREC_IM (x));                     \
  mpc_out_str (stdout, 2, 0, x, MPC_RNDNN);                     \
  printf ("\n");                                                \
} while (0)


/*
 * ASSERT macros
 */

#define MPC_ASSERT(expr)                                        \
  do {                                                          \
    if (!(expr))                                                \
      {                                                         \
        fprintf (stderr, "%s:%d: MPC assertion failed: %s\n",   \
                 __FILE__, __LINE__, #expr);                    \
        abort();                                                \
      }                                                         \
  } while (0)


/*
 * Constants
 */

#ifndef MUL_KARATSUBA_THRESHOLD
#define MUL_KARATSUBA_THRESHOLD 23
#endif


/*
 * Define internal functions
 */

#if defined (__cplusplus)
extern "C" {
#endif

__MPC_DECLSPEC int  mpc_mul_naive     __MPC_PROTO ((mpc_ptr, mpc_srcptr, mpc_srcptr, mpc_rnd_t));
__MPC_DECLSPEC int  mpc_mul_karatsuba __MPC_PROTO ((mpc_ptr, mpc_srcptr, mpc_srcptr, mpc_rnd_t));
__MPC_DECLSPEC char* mpc_alloc_str __MPC_PROTO ((size_t));
__MPC_DECLSPEC char* mpc_realloc_str __MPC_PROTO ((char*, size_t, size_t));
__MPC_DECLSPEC void mpc_free_str __MPC_PROTO ((char*));
__MPC_DECLSPEC unsigned long mpc_ceil_log2 __MPC_PROTO ((unsigned long));

#if defined (__cplusplus)
}
#endif


#endif
