/* tsin_cos -- test file for mpc_sin_cos.

Copyright (C) 2011, 2013 INRIA

This file is part of GNU MPC.

GNU MPC is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the
Free Software Foundation; either version 3 of the License, or (at your
option) any later version.

GNU MPC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for
more details.

You should have received a copy of the GNU Lesser General Public License
along with this program. If not, see http://www.gnu.org/licenses/ .
*/

#include "mpc-tests.h"

static void random_params        (mpc_fun_param_t *params,
                                  mpfr_exp_t exp_min, mpfr_exp_t exp_max,
                                  int special);
static void check_against_quadruple_precision (mpc_fun_param_t *params,
                                               mpc_fun_param_t *params_sin,
                                               mpc_fun_param_t *params_cos,
                                               mpfr_prec_t prec,
                                               mpfr_exp_t exp_min,
                                               mpfr_exp_t exp_max,
                                               int special);

/* tgeneric(desc, prec_min, prec_max, step, exp_max) checks rounding with
   random numbers:
   - with precision ranging from prec_min to prec_max with an increment of
   step,
   - with exponent between -exp_max and exp_max.
   - for pure real, pure imaginary and infinite random parameters.

   It also checks parameter reuse.
*/
static void
tgeneric_custom (mpfr_prec_t prec_min, mpfr_prec_t prec_max, mpfr_prec_t step,
                 mpfr_exp_t exp_max)
{
  int special = 0;
  const int last_special = 8;
  mpfr_prec_t prec;
  mpfr_exp_t exp_min;
  mpc_fun_param_t params;
  mpc_fun_param_t params_sin;
  mpc_fun_param_t params_cos;

  read_description (&params, "sin_cos.dsc");
  init_parameters (&params);
  read_description (&params_sin, "sin.dsc");
  init_parameters (&params_sin);
  read_description (&params_cos, "cos.dsc");
  init_parameters (&params_cos);

  /* ask for enough memory */
  set_output_precision (&params, 4 * prec_max);
  set_input_precision (&params, prec_max);
  set_reference_precision (&params, prec_max);
  set_output_precision (&params_sin, 4 * prec_max);
  set_input_precision (&params_sin, prec_max);
  set_reference_precision (&params_sin, prec_max);
  set_output_precision (&params_cos, 4 * prec_max);
  set_input_precision (&params_cos, prec_max);
  set_reference_precision (&params_cos, prec_max);

  /* sanity checks */
  exp_min = mpfr_get_emin ();
  if (exp_max <= 0 || exp_max > mpfr_get_emax ())
    exp_max = mpfr_get_emax();
  if (-exp_max > exp_min)
    exp_min = - exp_max;
  if (step < 1)
    step = 1;

  /* check consistency with quadruple precision for random parameters */
  for (prec = prec_min; prec <= prec_max; prec += step)
    check_against_quadruple_precision (&params, &params_sin, &params_cos,
                                       prec, exp_min, exp_max, -1);

  /* check consistency with quadruple precision for special values:
     pure real, pure imaginary, or infinite arguments */
  for (special = 0; special < last_special ; special++)
    check_against_quadruple_precision (&params, &params_sin, &params_cos,
                                       prec_max, exp_min, exp_max,
                                       special);

  clear_parameters (&params);
  clear_parameters (&params_sin);
  clear_parameters (&params_cos);
}

static void
filter_params (mpc_fun_param_t *params_sin_cos,
               mpc_fun_param_t *params,
               int index)
{
  /* inex */
  params->P[0].mpc_inex = (index == 0) ?
    MPC_INEX1 (params_sin_cos->P[0].mpc_inex)
    : MPC_INEX2 (params_sin_cos->P[0].mpc_inex);

  /* output */
  mpc_set (params->P[1].mpc, params_sin_cos->P[1 + index].mpc, MPC_RNDNN);

  /* input */
  mpc_set (params->P[2].mpc, params_sin_cos->P[3].mpc, MPC_RNDNN);

  /* rnd mode is already set */
}

static void
gather_params (mpc_fun_param_t *params_sin_cos,
               mpc_fun_param_t *params_sin,
               mpc_fun_param_t *params_cos)
{
  /* do not check inex value */
  params_sin_cos->P[6].mpc_inex_data.real = TERNARY_NOT_CHECKED;
  params_sin_cos->P[6].mpc_inex_data.imag = TERNARY_NOT_CHECKED;

  mpc_set (params_sin_cos->P[7].mpc_data.mpc,
           params_sin->P[5].mpc_data.mpc,
           MPC_RNDNN);
  params_sin_cos->P[7].mpc_data.known_sign_real = -1;
  params_sin_cos->P[7].mpc_data.known_sign_imag = -1;

  mpc_set (params_sin_cos->P[8].mpc_data.mpc,
           params_cos->P[5].mpc_data.mpc,
           MPC_RNDNN);
  params_sin_cos->P[8].mpc_data.known_sign_real = -1;
  params_sin_cos->P[8].mpc_data.known_sign_imag = -1;
}

static void
gather_rounding_modes (mpc_fun_param_t *params_sin_cos,
                       mpc_fun_param_t *params_sin,
                       mpc_fun_param_t *params_cos)
{
  params_sin_cos->P[4].mpc_rnd = params_sin->P[3].mpc_rnd;
  params_sin_cos->P[5].mpc_rnd = params_cos->P[3].mpc_rnd;
}

static void
check_against_quadruple_precision (mpc_fun_param_t *params,
                                   mpc_fun_param_t *params_sin,
                                   mpc_fun_param_t *params_cos,
                                   mpfr_prec_t prec,
                                   mpfr_exp_t exp_min, mpfr_exp_t exp_max,
                                   int special)
{
  mpc_operand_t *P = params->P; /* developer-friendly alias, used in macros */

  set_input_precision (params, prec);
  set_reference_precision (params, prec);
  set_output_precision (params, 4 * prec);

  set_input_precision (params_sin, prec);
  set_reference_precision (params_sin, prec);
  set_output_precision (params_sin, 4 * prec);

  set_input_precision (params_cos, prec);
  set_reference_precision (params_cos, prec);
  set_output_precision (params_cos, 4 * prec);


  for (first_rnd_mode (params_sin);
       is_valid_rnd_mode (params_sin);
       next_rnd_mode (params_sin))
    {
      for (first_rnd_mode (params_cos);
           is_valid_rnd_mode (params_cos);
           next_rnd_mode (params_cos))
        {
          gather_rounding_modes (params, params_sin, params_cos);
          do
            {
              random_params (params, exp_min, exp_max, special);
              P[0].mpc_inex = mpc_sin_cos (P[1].mpc, P[2].mpc, P[3].mpc,
                                           P[4].mpc_rnd, P[5].mpc_rnd);
              filter_params (params, params_sin, 0);
              filter_params (params, params_cos, 1);
            } while (double_rounding (params_sin)
                     || double_rounding (params_cos));
          gather_params (params, params_sin, params_cos);

          set_output_precision (params, prec);
          P[0].mpc_inex = mpc_sin_cos (P[1].mpc, P[2].mpc, P[3].mpc,
                                       P[4].mpc_rnd, P[5].mpc_rnd);
          check_data (NULL, params, 0);

          set_output_precision (params, 4 * prec);
        }
    }
}


/* special cases */

enum {
  SPECIAL_MINF,
  SPECIAL_MZERO,
  SPECIAL_PZERO,
  SPECIAL_PINF,
  SPECIAL_COUNT
};

static void
special_mpfr (mpfr_ptr x, int special)
{
  switch (special)
    {
    case SPECIAL_MINF:
      mpfr_set_inf (x, -1);
      break;
    case SPECIAL_MZERO:
      mpfr_set_zero (x, -1);
      break;
    case SPECIAL_PZERO:
      mpfr_set_zero (x, +1);
      break;
    case SPECIAL_PINF:
      mpfr_set_inf (x, +1);
      break;
   }
}

static void
special_random_mpc (mpc_ptr z, mpfr_exp_t exp_min, mpfr_exp_t exp_max,
                    int special)
{
  mpfr_ptr special_part;
  mpfr_ptr random_part;
  int mpfr_special;

  if (special < SPECIAL_COUNT)
    {
      mpfr_special = special;
      special_part = mpc_realref (z);
      random_part  = mpc_imagref (z);
    }
  else
    {
      mpfr_special = special - SPECIAL_COUNT;
      special_part = mpc_imagref (z);
      random_part  = mpc_realref (z);
    }

  special_mpfr (special_part, mpfr_special);
  test_random_mpfr (random_part, exp_min, exp_max, 128);
}

static void
random_params (mpc_fun_param_t *params,
               mpfr_exp_t exp_min, mpfr_exp_t exp_max,
               int special)
{
  int i;
  int base_index = 0;
  const int start = params->nbout;
  const int end = start + params->nbin - 2;

  for (i = start; i < end; i++)
    {
      if (base_index <= special
          && special - base_index < 2 * SPECIAL_COUNT)
        special_random_mpc (params->P[i].mpc, exp_min, exp_max,
                            special - base_index);
      else
        test_random_mpc (params->P[i].mpc, exp_min, exp_max, 128);
      base_index += 2 * SPECIAL_COUNT;
    }
}

int
main (void)
{
  test_start ();

  tgeneric_custom (2, 512, 13, 7);

  test_end ();

  return 0;
}
