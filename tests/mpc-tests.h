/* Tests helper functions.

Copyright (C) 2008 Philippe Th\'eveny.

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
#include <gmp.h>
#include <mpfr.h>
#include "mpc.h"

#define MPFR_OUT(X) \
  printf (#X" [%ld]=", MPFR_PREC (X));\
  mpfr_out_str (stdout, 2, 0, (X), GMP_RNDN);\
  printf ("\n");

#define QUOTE(X) NAME(X)
#define NAME(X) #X

/** RANDOM FUNCTIONS **/
/* the 3 following functions handle seed for random numbers. Usage:
   - add test_start at the beginning of your test function
   - use test_default_random (or use your random functions with
   gmp_randstate_t rands) in your tests
   - add test_end at the end the test function */
gmp_randstate_t  rands;
char             rands_initialized;

void test_start (void);
void test_end (void);

void test_default_random (mpc_ptr, mp_exp_t, mp_exp_t, int);


/** COMPARISON FUNCTIONS **/
/* some sign are unspecified in ISO C99, thus we record in struct known_signs_t
   whether the sign has to be checked */
typedef struct
{
  int re;  /* boolean value */
  int im;  /* boolean value */
} known_signs_t;

/* same_mpfr_value returns 1:
   - if got and ref have the same value and known_sign is true,
   or
   - if they have the same absolute value, got = 0 or got = inf, and known_sign is
   false.
   returns 0 in other cases.
   Unlike mpfr_cmp, same_mpfr_value(got, ref, x) return 1 when got and
   ref are both NaNs. */
int same_mpfr_value (mpfr_ptr got, mpfr_ptr ref, int known_sign);
int same_mpc_value (mpc_ptr got, mpc_ptr ref, known_signs_t known_signs);


/** GENERIC TESTS **/

typedef int (*CC_func_ptr) (mpc_t, mpc_srcptr, mpc_rnd_t);
typedef int (*CCC_func_ptr) (mpc_t, mpc_srcptr, mpc_srcptr, mpc_rnd_t);
typedef int (*CCU_func_ptr) (mpc_t, mpc_srcptr, unsigned long, mpc_rnd_t);
typedef int (*CCS_func_ptr) (mpc_t, mpc_srcptr, long, mpc_rnd_t);
typedef int (*CCF_func_ptr) (mpc_t, mpc_srcptr, mpfr_srcptr, mpc_rnd_t);
typedef int (*CFC_func_ptr) (mpc_t, mpfr_srcptr, mpc_srcptr, mpc_rnd_t);
typedef int (*CUC_func_ptr) (mpc_t, unsigned long, mpc_srcptr, mpc_rnd_t);
typedef int (*CUUC_func_ptr) (mpc_t, unsigned long, unsigned long, mpc_srcptr,
                              mpc_rnd_t);
typedef int (*FC_func_ptr) (mpfr_t, mpc_srcptr, mpfr_rnd_t);
typedef void (*V_CC_func_ptr) (mpc_t, mpc_srcptr, mpc_rnd_t);

typedef union
{
  FC_func_ptr FC;   /* output: mpfr_t, input: mpc_t */

  CC_func_ptr CC;   /* output: mpc_t, input: mpc_t */
  V_CC_func_ptr V_CC; /* no inexact value returned, output: mpc_t,
                         input: mpc_t */

  CCC_func_ptr CCC; /* output: mpc_t, inputs: (mpc_t, mpc_t) */
  CCU_func_ptr CCU; /* output: mpc_t, inputs: (mpc_t, unsigned long) */
  CCS_func_ptr CCS; /* output: mpc_t, inputs: (mpc_t, long) */
  CCF_func_ptr CCF; /* output: mpc_t, inputs: (mpc_t, mpfr_t) */
  CFC_func_ptr CFC; /* output: mpc_t, inputs: (mpfr_t, mpc_t) */
  CUC_func_ptr CUC; /* output: mpc_t, inputs: (unsigned long, mpc_t) */

  CUUC_func_ptr CUUC; /* output: mpc_t, inputs: (ulong, ulong, mpc_t) */
} func_ptr;

/* the rounding mode is implicit */
typedef enum
  {
    FC,   /* output: mpfr_t, input: mpc_t */
    CC,   /* output: mpc_t, input: mpc_t */
    V_CC, /* no inexact value returned, output: mpc_t, input: mpc_t */

    CCC,  /* output: mpc_t, inputs: (mpc_t, mpc_t) */
    CCU,  /* output: mpc_t, inputs: (mpc_t, unsigned long) */
    CCS,  /* output: mpc_t, inputs: (mpc_t, long) */
    CCF,  /* output: mpc_t, inputs: (mpc_t, mpfr_t) */
    CFC,  /* output: mpc_t, inputs: (mpfr_t, mpc_t) */
    CUC,  /* output: mpc_t, inputs: (unsigned long, mpc_t) */

    CUUC  /* output: mpc_t, inputs: (ulong, ulong, mpc_t) */
  } func_type;

/* properties */
#define FUNC_PROP_NONE     0
#define FUNC_PROP_SYMETRIC 1

typedef struct
{
  func_ptr  pointer;
  func_type type;
  char *    name;
  int       properties;
} mpc_function;

#define DECL_FUNC(_ftype, _fvar, _func)         \
  mpc_function _fvar;                           \
  _fvar.pointer._ftype = _func;                 \
  _fvar.type = _ftype;                          \
  _fvar.name = QUOTE (_func);                   \
  _fvar.properties = FUNC_PROP_NONE;

#define DECL_CC_FUNC(fvar, func)                \
  mpc_function (fvar);                          \
  (fvar).pointer.CC = (func);                   \
  (fvar).type = CC;                             \
  (fvar).name = QUOTE (func);                   \
  (fvar).properties = 0;

#define DECL_V_CC_FUNC(fvar, func)              \
  mpc_function (fvar);                          \
  (fvar).pointer.V_CC = (func);                 \
  (fvar).type = V_CC;                           \
  (fvar).name = QUOTE (func);                   \
  (fvar).properties = 0;

#define DECL_FC_FUNC(fvar, func)                \
  mpc_function (fvar);                          \
  (fvar).pointer.FC = (func);                   \
  (fvar).type = FC;                             \
  (fvar).name = QUOTE (func);                   \
  (fvar).properties = 0;

#define DECL_CFC_FUNC(fvar, func)               \
  mpc_function (fvar);                          \
  (fvar).pointer.CFC = (func);                  \
  (fvar).type = CFC;                            \
  (fvar).name = QUOTE (func);                   \
  (fvar).properties = 0;

#define DECL_CCF_FUNC(fvar, func)               \
  mpc_function (fvar);                          \
  (fvar).pointer.CCF = (func);                  \
  (fvar).type = CCF;                            \
  (fvar).name = QUOTE (func);                   \
  (fvar).properties = 0;


#define DECL_CCC_FUNC(fvar, func)               \
  mpc_function (fvar);                          \
  (fvar).pointer.CCC = (func);                  \
  (fvar).type = CCC;                            \
  (fvar).name = QUOTE (func);                   \
  (fvar).properties = 0;


#define DECL_CCU_FUNC(fvar, func)               \
  mpc_function (fvar);                          \
  (fvar).pointer.CCU = (func);                  \
  (fvar).type = CCU;                            \
  (fvar).name = QUOTE (func);                   \
  (fvar).properties = 0;


#define DECL_CUC_FUNC(fvar, func)               \
  mpc_function (fvar);                          \
  (fvar).pointer.CUC = (func);                  \
  (fvar).type = CUC;                            \
  (fvar).name = QUOTE (func);                   \
  (fvar).properties = 0;


#define DECL_CCS_FUNC(fvar, func)               \
  mpc_function (fvar);                          \
  (fvar).pointer.CCS = (func);                  \
  (fvar).type = CCS;                            \
  (fvar).name = QUOTE (func);                   \
  (fvar).properties = 0;


#define DECL_CUUC_FUNC(fvar, func)              \
  mpc_function (fvar);                          \
  (fvar).pointer.CUUC = (func);                 \
  (fvar).type = CUUC;                           \
  (fvar).name = QUOTE (func);                   \
  (fvar).properties = 0;


/* tgeneric(mpc_function, prec_min, prec_max, step, exp_max) checks rounding
 with random numbers:
 - with precision ranging from prec_min to prec_max with an increment of
   step,
 - with exponent between -prec_max and prec_max.

 It also checks parameter reuse (it is assumed here that either two mpc_t
 variables are equal or they are different, in the sense that the real part of
 one of them cannot be the imaginary part of the other). */
void tgeneric (mpc_function, mpfr_prec_t, mpfr_prec_t, mpfr_prec_t, mp_exp_t);


/** READ FILE WITH TEST DATA SET **/
/* data_check (function, "data_file_name") checks function results against
   precomputed data in a file.*/
void data_check (mpc_function, const char *);
