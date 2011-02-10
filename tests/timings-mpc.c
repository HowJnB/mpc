/* timings-mpc -- measure efficiency of MPC functions

Copyright (C) 2001, 2010, 2011 INRIA
Copyright (C) 2001 Norbert Mueller (Univ. Trier, Germany)

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2.1 of the License, or (at your
option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program; see the file COPYING.LIB.  If not, write to
the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
MA 02111-1307, USA. */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "gmp.h"
#include "mpfr.h"
#include "mpc.h"

#if defined (USG) || defined (__SVR4) || defined (_UNICOS) || defined(HPUX)
#include <time.h>

int
cputime ()
{
  if (CLOCKS_PER_SEC < 100000)
    return clock () * 1000 / CLOCKS_PER_SEC;
  return clock () / (CLOCKS_PER_SEC / 1000);
}
#else
#include <sys/types.h>
#include <sys/resource.h>

static int
cputime ()
{
  struct rusage rus;

  getrusage (0, &rus);
  return rus.ru_utime.tv_sec * 1000 + rus.ru_utime.tv_usec / 1000;
}
#endif

int
main (int argc, char *argv[])
{
  int n, prec, st, st2, N, i;
  mpc_t x, y, z;
  
  if (argc != 2 && argc != 3)
    {
      fprintf(stderr, "Usage: timing digits \n");
      exit(1);
    }

  printf ("Using MPC %s with MPFR %s and GMP-%s\n",
	  mpc_get_version (), mpfr_version, gmp_version);
  n = atoi(argv[1]);
  prec = (int) ( n * log(10.0) / log(2.0) + 1.0 );
  printf ("[precision is %u bits]\n", prec);
 
  mpc_init2 (x, prec);
  mpc_init2 (y, prec);
  mpc_init2 (z, prec); 

  mpc_set_ui_ui (x, 3, 1, MPC_RNDNN);
  mpc_sqrt (x, x, MPC_RNDNN);
  mpc_set_ui_ui (y, 5, 1, MPC_RNDNN);
  mpc_sqrt (y, y, MPC_RNDNN);

  mpc_log (z, x, MPC_RNDNN);

  N=1;  st = cputime();
  do {
    for (i=0;i<N;i++) mpc_mul (z, x, y, MPC_RNDNN);
    N=2*N;
    st2=cputime();
  } while (st2-st<1000); 	  
  printf("x*y        took %f ms (%d eval in %d ms)\n", 
	 (double)(st2-st)/(N-1),N-1,st2-st);

  N=1;  st = cputime();
  do {
    for (i=0;i<N;i++) mpc_div (z, x, y, MPC_RNDNN);
    N=2*N;
    st2=cputime();
  } while (st2-st<1000); 	  
  printf("x/y        took %f ms (%d eval in %d ms)\n", 
	 (double)(st2-st)/(N-1),N-1,st2-st);
  
  N=1;  st = cputime();
  do {
    for (i=0;i<N;i++) mpc_sqrt (z, x, MPC_RNDNN);
    N=2*N;
    st2=cputime();
  } while (st2-st<1000); 	  
  printf("sqrt(x)    took %f ms (%d eval in %d ms)\n", 
	 (double)(st2-st)/(N-1),N-1,st2-st);
  
  N=1;  st = cputime();
  do {
    for (i=0;i<N;i++) mpc_exp (z, x, MPC_RNDNN);
    N=2*N;
    st2=cputime();
  } while (st2-st<1000); 	  
  printf("exp(x)     took %f ms (%d eval in %d ms)\n", 
	 (double)(st2-st)/(N-1),N-1,st2-st);
  
  N=1;  st = cputime();
  do {
    for (i=0;i<N;i++) mpc_log (z, x, MPC_RNDNN);
    N=2*N;
    st2=cputime();
  } while (st2-st<1000); 	  
  printf("log(x)     took %f ms (%d eval in %d ms)\n", 
	 (double)(st2-st)/(N-1),N-1,st2-st);
  
  N=1;  st = cputime();
  do {
    for (i=0;i<N;i++) mpc_sin (z, x, MPC_RNDNN);
    N=2*N;
    st2=cputime();
  } while (st2-st<1000); 	  
  printf("sin(x)     took %f ms (%d eval in %d ms)\n", 
	 (double)(st2-st)/(N-1),N-1,st2-st);
  
  N=1;  st = cputime();
  do {
    for (i=0;i<N;i++) mpc_cos (z, x, MPC_RNDNN);
    N=2*N;
    st2=cputime();
  } while (st2-st<1000);
  printf("cos(x)     took %f ms (%d eval in %d ms)\n",
         (double)(st2-st)/(N-1),N-1,st2-st);

  N=1;  st = cputime();
  do {
    for (i=0;i<N;i++) mpc_acos (z, x, MPC_RNDNN);
    N=2*N;
    st2=cputime();
  } while (st2-st<1000);
  printf("acos(x)    took %f ms (%d eval in %d ms)\n",
         (double)(st2-st)/(N-1),N-1,st2-st);
  
  N=1;  st = cputime();
  do {
    for (i=0;i<N;i++) mpc_atan (z, x, MPC_RNDNN);
    N=2*N;
    st2=cputime();
  } while (st2-st<1000);
  printf("atan(x)    took %f ms (%d eval in %d ms)\n",
         (double)(st2-st)/(N-1),N-1,st2-st);

  mpc_clear(x);
  mpc_clear(y);
  mpc_clear(z);

  return 0;
}
