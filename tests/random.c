/* Handle seed for random numbers.

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

/* Put test_start at the beginning of your test function and
   test_end at the end.
   These are an adaptation of those of MPFR. */

#include <stdio.h>
#include <stdlib.h>

#include "gmp.h"

#include "config.h"

#ifdef TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# ifdef HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif

gmp_randstate_t  rands;
char             rands_initialized;

void
test_start ()
{
  char *environment_seed;
  unsigned long seed;

  if (rands_initialized)
    {
      fprintf (stderr,
               "Put test_start at the beginning of your test function.\n");
      exit (1);
    }

  gmp_randinit_default (rands);
  rands_initialized = 1;

  environment_seed = getenv ("GMP_CHECK_RANDOMIZE");
  if (environment_seed == NULL)
      gmp_randseed_ui (rands, 0xfac11e);
  else
    {
      seed = atoi (environment_seed);
      if (seed == 0 || seed == 1)
        {
#if HAVE_GETTIMEOFDAY
          struct timeval  tv;
          gettimeofday (&tv, NULL);
          seed = tv.tv_sec + tv.tv_usec;
#else
          time_t  tv;
          time (&tv);
          seed = tv;
#endif
          gmp_randseed_ui (rands, seed);
          printf ("Seed GMP_CHECK_RANDOMIZE=%lu "
                  "(include this in bug reports)\n", seed);
        }
      else
        {
          printf ("Re-seeding with GMP_CHECK_RANDOMIZE=%lu\n", seed);
          gmp_randseed_ui (rands, seed);
        }
    }
}

void
test_end ()
{
  if (rands_initialized)
    {
      rands_initialized = 0;
      gmp_randclear (rands);
    }
}
