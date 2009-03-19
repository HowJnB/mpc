/* mpc_inp_str -- Input a complex number from a given stream.

Copyright (C) 2009 Andreas Enge

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
#include <ctype.h>
#include <string.h>
#include "mpc-impl.h"

static size_t
skip_whitespace (FILE *stream) {
   int c = getc (stream);
   size_t size = 0;
   while (c != EOF && isspace ((unsigned char) c)) {
      c = getc (stream);
      size++;
   }
   if (c != EOF)
      ungetc (c, stream);
   return size;
}


static char *
mpc_alloc_str (size_t len) {
   void * (*allocfunc) (size_t);
   mp_get_memory_functions (&allocfunc, NULL, NULL);
   return (char *) ((*allocfunc) (len * sizeof (char)));
}


static char *
mpc_realloc_str (char * str, size_t oldlen, size_t newlen) {
   void * (*reallocfunc) (void *, size_t, size_t);
   mp_get_memory_functions (NULL, &reallocfunc, NULL);
   return (char *) ((*reallocfunc) (str, oldlen * sizeof (char),
                    newlen * sizeof (char)));
}


static void
mpc_free_str_len (char *str, size_t len) {
   void (*freefunc) (void *, size_t);
   mp_get_memory_functions (NULL, NULL, &freefunc);
   (*freefunc) (str, len * sizeof (char));
}



size_t
mpc_inp_str (mpc_ptr rop, FILE *stream, int base, mpc_rnd_t rnd_mode) {
   size_t size, nread;
   int c;

   if (stream == NULL)
      stream = stdin;

   size = skip_whitespace (stream);
   c = getc (stream);
   if (c != EOF) {
      if (c != '(') {
         /* real number without parentheses */
         ungetc (c, stream);
         nread = mpfr_inp_str (MPC_RE(rop), stream, base, MPC_RND_RE(rnd_mode));
         if (nread != 0) {
            size += nread;
            mpfr_set_ui (MPC_IM(rop), 0ul, GMP_RNDN);
            return size;
         }
      }
      else {
         /* Complex number in parentheses; read everything from '(' to ')'
            into a string and have mpc_setstr do the work                  */
         size_t strsize = 100;
         char *str = mpc_alloc_str (strsize);
         nread = 0;
         while (c != EOF && c != ')') {
            str [nread] = (char) c;
            nread++;
            if (nread == strsize) {
               str = mpc_realloc_str (str, strsize, 2 * strsize);
               strsize *= 2;
            }
            c = getc (stream);
         }
         if (c != EOF) {
            str [nread] = ')';
            nread++;
            str = mpc_realloc_str (str, strsize, nread + 1);
            str [nread] = '\0';
            size += nread;
            if (mpc_set_str (rop, str, base, rnd_mode) != -1)
               return size;
         }
         mpc_free_str_len (str, strsize);
      }
   }

   mpfr_set_nan (MPC_RE(rop));
   mpfr_set_nan (MPC_IM(rop));
   return 0;
}
