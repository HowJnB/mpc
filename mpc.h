/* mpc.h -- Include file for mpc.

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

#ifndef __MPC_H
#define __MPC_H

/* check if stdio.h is included */
#if defined (FILE) || defined (H_STDIO) || defined (_H_STDIO) \
 || defined (_STDIO_H) || defined (_STDIO_H_) || defined (__STDIO_H__) \
 || defined (_STDIO_INCLUDED) || defined (__dj_include_stdio_h_)
#define _MPC_H_HAVE_FILE 1
#endif

/* Definition of rounding modes */

/* a complex rounding mode is just a pair of two real rounding modes
   we reserve three bits for a real rounding mode, to have space to
   add the "away from zero" rounding mode.
 */
#define RNDC(r1,r2) ((r1) + (r2 << 3))
#define MPC_RND_RE(x) ((x) & 0x07)
#define MPC_RND_IM(x) ((x) >> 3)

#define GMP_RNDA 4 /* round away from zero */

#define MPC_RNDNN RNDC(GMP_RNDN,GMP_RNDN)
#define MPC_RNDNZ RNDC(GMP_RNDN,GMP_RNDZ)
#define MPC_RNDNU RNDC(GMP_RNDN,GMP_RNDU)
#define MPC_RNDND RNDC(GMP_RNDN,GMP_RNDD)
#define MPC_RNDNA RNDC(GMP_RNDN,GMP_RNDA)

#define MPC_RNDZN RNDC(GMP_RNDZ,GMP_RNDN)
#define MPC_RNDZZ RNDC(GMP_RNDZ,GMP_RNDZ)
#define MPC_RNDZU RNDC(GMP_RNDZ,GMP_RNDU)
#define MPC_RNDZD RNDC(GMP_RNDZ,GMP_RNDD)
#define MPC_RNDZA RNDC(GMP_RNDZ,GMP_RNDA)

#define MPC_RNDUN RNDC(GMP_RNDU,GMP_RNDN)
#define MPC_RNDUZ RNDC(GMP_RNDU,GMP_RNDZ)
#define MPC_RNDUU RNDC(GMP_RNDU,GMP_RNDU)
#define MPC_RNDUD RNDC(GMP_RNDU,GMP_RNDD)
#define MPC_RNDUA RNDC(GMP_RNDU,GMP_RNDA)

#define MPC_RNDDN RNDC(GMP_RNDD,GMP_RNDN)
#define MPC_RNDDZ RNDC(GMP_RNDD,GMP_RNDZ)
#define MPC_RNDDU RNDC(GMP_RNDD,GMP_RNDU)
#define MPC_RNDDD RNDC(GMP_RNDD,GMP_RNDD)
#define MPC_RNDDA RNDC(GMP_RNDD,GMP_RNDA)

#define MPC_RNDAN RNDC(GMP_RNDA,GMP_RNDN)
#define MPC_RNDAZ RNDC(GMP_RNDA,GMP_RNDZ)
#define MPC_RNDAU RNDC(GMP_RNDA,GMP_RNDU)
#define MPC_RNDAD RNDC(GMP_RNDA,GMP_RNDD)
#define MPC_RNDAA RNDC(GMP_RNDA,GMP_RNDA)

/* return values */
/* transform negative to 2, positive to 1, leave 0 unchanged */
#define MPC_INEX_POS(inex) (((inex) < 0) ? 2 : ((inex) == 0) ? 0 : 1)
/* transform 2 to negative, 1 to positive, leave 0 unchanged */
#define MPC_INEX_NEG(inex) (((inex) == 2) ? -1 : ((inex) == 0) ? 0 : 1)

#define MPC_INEX(inex_re, inex_im) \
        (MPC_INEX_POS(inex_re) | (MPC_INEX_POS(inex_im) << 2))
#define MPC_INEX_RE(inex) MPC_INEX_NEG((inex) & 3)
#define MPC_INEX_IM(inex) MPC_INEX_NEG((inex) >> 2)

/* Definitions of types and their semantics */

/* typedef int mp_rnd_t; */                /* imported from mpfr.h,
                                              only 2*3 = 6 bits are used */

typedef struct {
  mpfr_t re;
  mpfr_t im;
}
__mpc_struct;

#define MPC_RE(x) ((x)->re)
#define MPC_IM(x) ((x)->im)

typedef __mpc_struct mpc_t[1]; 
typedef __mpc_struct *mpc_ptr; 
typedef __gmp_const __mpc_struct *mpc_srcptr;

/* Prototypes */

#ifndef _PROTO
#if defined (__STDC__) || defined (__cplusplus)
#define _PROTO(x) x
#else
#define _PROTO(x) ()
#endif
#endif

#if defined (__cplusplus)
extern "C" {
#endif

extern mp_prec_t __mpc_default_fp_bit_precision;

int  mpc_add   _PROTO ((mpc_ptr, mpc_srcptr, mpc_srcptr, mp_rnd_t));
int  mpc_add_fr _PROTO ((mpc_ptr, mpc_srcptr, mpfr_srcptr, mp_rnd_t));
int  mpc_add_ui _PROTO ((mpc_ptr, mpc_srcptr, unsigned long int, mp_rnd_t));
int  mpc_sub   _PROTO ((mpc_ptr, mpc_srcptr, mpc_srcptr, mp_rnd_t));
int  mpc_sub_ui _PROTO ((mpc_ptr, mpc_srcptr, unsigned long int, mp_rnd_t));
int  mpc_mul   _PROTO ((mpc_ptr, mpc_srcptr, mpc_srcptr, mp_rnd_t));
int  mpc_mul_naive _PROTO ((mpc_ptr, mpc_srcptr, mpc_srcptr, mp_rnd_t));
int  mpc_mul_karatsuba _PROTO ((mpc_ptr, mpc_srcptr, mpc_srcptr, mp_rnd_t));
int  mpc_mul_fr  _PROTO ((mpc_ptr, mpc_srcptr, mpfr_srcptr, mp_rnd_t));
int  mpc_mul_ui  _PROTO ((mpc_ptr, mpc_srcptr, unsigned long int, mp_rnd_t));
int  mpc_sqr   _PROTO ((mpc_ptr, mpc_srcptr, mp_rnd_t));
int  mpc_div    _PROTO ((mpc_ptr, mpc_srcptr, mpc_srcptr, mp_rnd_t));
int  mpc_div_fr _PROTO ((mpc_ptr, mpc_srcptr, mpfr_srcptr, mp_rnd_t));
int  mpc_div_ui _PROTO ((mpc_ptr, mpc_srcptr, unsigned long int, mp_rnd_t));
int  mpc_ui_div _PROTO ((mpc_ptr, unsigned long int, mpc_srcptr, mp_rnd_t));
int  mpc_div_2exp _PROTO ((mpc_ptr, mpc_srcptr, unsigned long int, mp_rnd_t));
int  mpc_mul_2exp _PROTO ((mpc_ptr, mpc_srcptr, unsigned long int, mp_rnd_t));
int  mpc_conj   _PROTO ((mpc_ptr, mpc_srcptr, mp_rnd_t));
int  mpc_neg    _PROTO ((mpc_ptr, mpc_srcptr, mp_rnd_t));
int  mpc_norm   _PROTO ((mpfr_ptr, mpc_srcptr, mp_rnd_t));
int  mpc_abs    _PROTO ((mpfr_ptr, mpc_srcptr, mp_rnd_t));
int  mpc_sqrt   _PROTO ((mpc_ptr, mpc_srcptr, mp_rnd_t));
int  mpc_set    _PROTO ((mpc_ptr, mpc_srcptr, mp_rnd_t));
int  mpc_set_d_d _PROTO ((mpc_ptr, double, double, mp_rnd_t));
int  mpc_set_ui_fr _PROTO ((mpc_ptr, unsigned long int, mpfr_srcptr, mp_rnd_t));
int  mpc_set_ui_ui _PROTO ((mpc_ptr, unsigned long int, unsigned long int, mp_rnd_t));
int  mpc_set_si_si _PROTO ((mpc_ptr, long int, long int, mp_rnd_t));
int  mpc_cmp _PROTO ((mpc_srcptr, mpc_srcptr));
void mpc_exp _PROTO ((mpc_ptr, mpc_srcptr, mp_rnd_t));
void mpc_clear  _PROTO ((mpc_ptr));
void mpc_init   _PROTO ((mpc_ptr));
void mpc_random _PROTO ((mpc_ptr));
void mpc_random2 _PROTO ((mpc_ptr, mp_size_t, mp_exp_t));
void mpc_init2 _PROTO ((mpc_ptr, mp_prec_t));
void mpc_init3 _PROTO ((mpc_ptr, mp_prec_t, mp_prec_t));
void mpc_set_prec _PROTO ((mpc_ptr, mp_prec_t));
void mpc_set_default_prec _PROTO ((mp_prec_t));
mp_prec_t mpc_get_default_prec _PROTO ((void));
#ifdef _MPC_H_HAVE_FILE
size_t mpc_inp_str _PROTO ((mpc_ptr, FILE *, int, mp_rnd_t));
size_t mpc_out_str _PROTO ((FILE *, int, size_t, mpc_srcptr, mp_rnd_t));
#endif

#if defined (__cplusplus)
}
#endif

#define mpc_set_d(x, y, rnd) mpc_set_d_d(x, y, 0.0, rnd)
#define mpc_set_ui(x, y, rnd) mpc_set_ui_ui(x, y, 0, rnd)
#define mpc_set_si(x, y, rnd) mpc_set_si_si(x, y, 0, rnd)
#define mpc_init_set(x, y, rnd) \
 ( mpc_init(x), mpc_set((x), (y), (rnd)) )
#define mpc_init_set_ui_fr(x, y, z, rnd) \
 ( mpc_init(x), mpc_set_ui_fr((x), (y), (z), (rnd)) )
#define mpc_init_set_ui_ui(x, y, z, rnd) \
 ( mpc_init(x), mpc_set_ui_ui((x), (y), (z), (rnd)) )
#define mpc_init_set_si_si(x, y, z, rnd) \
 ( mpc_init(x), mpc_set_si_si((x), (y), (z), (rnd)) )
#define mpc_add_si(x, y, z, rnd) \
 ( (z) >= 0 ? mpc_add_ui ((x), (y), (z), (rnd)) : mpc_sub_ui ((x), (y), -(z), (rnd)) )

#endif /* ifndef __MPC_H */ 
