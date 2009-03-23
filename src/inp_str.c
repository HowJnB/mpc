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


int
mpc_inp_str (mpc_ptr rop, FILE *stream, size_t *read, int base, mpc_rnd_t rnd_mode) {
   size_t white, nread = 0;
   int inex = -1;
   int c;
   int par = 0;

   if (stream == NULL)
      stream = stdin;

   white = skip_whitespace (stream);
   c = getc (stream);
   if (c != EOF) {
      /* If c=='(', set par=1 and read everything up to ')' skipping one level
         of nested pair of parentheses; otherwise, par=0 already and read
         everything up to white space.  Then have mpc_setstr do the work.  */
      size_t strsize = 100;
      char *str = mpc_alloc_str (strsize);
      while (c != EOF && c != '\n' 
             && (par != 0 || !isspace ((unsigned char) c))) {
         str [nread] = (char) c;
         nread++;
         if (nread == strsize) {
            str = mpc_realloc_str (str, strsize, 2 * strsize);
            strsize *= 2;
         }
         if (c == '(') {
           par++;
           if (par > 2)
             break;
         }
         else if (c == ')') {
           par--;
           if (par < 0)
             break;
         }
         c = getc (stream);
      }
      if (isspace ((unsigned char) c))
        ungetc (c, stream);
      
      str = mpc_realloc_str (str, strsize, nread + 1);
      strsize = nread + 1;
      str [nread] = '\0';
      inex = mpc_set_str (rop, str, base, rnd_mode);
      
      mpc_free_str (str);
   }

   if (inex == -1) {
      mpfr_set_nan (MPC_RE(rop));
      mpfr_set_nan (MPC_IM(rop));
   }
   if (read != NULL)
      *read = white + nread;
   return inex;
}
