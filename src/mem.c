/* wrapper functions to allocate, reallocate and free memory

Copyright (C) 2009 Andreas Enge, Philippe Th\'eveny, Paul Zimmermann

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

#include <string.h>   /* for strlen */
#include "mpc-impl.h"

char *
mpc_alloc_str (size_t len)
{
#ifdef mp_get_memory_functions /* GMP >= 4.2 */
  void * (*allocfunc) (size_t);
  mp_get_memory_functions (&allocfunc, NULL, NULL);
  return (char *) ((*allocfunc) (len));
#else /* GMP < 4.2 */
  return (char *) __gmp_allocate_func (len);
#endif
}

char *
mpc_realloc_str (char * str, size_t oldlen, size_t newlen)
{
#ifdef mp_get_memory_functions /* GMP >= 4.2 */
  void * (*reallocfunc) (void *, size_t, size_t);
  mp_get_memory_functions (NULL, &reallocfunc, NULL);
  return (char *) ((*reallocfunc) (str, oldlen, newlen));
#else /* GMP < 4.2 */
  return (char *) __gmp_reallocate_func (str, oldlen, newlen);
#endif
}

void
mpc_free_str (char *str)
{
#ifdef mp_get_memory_functions /* GMP >= 4.2 */
  void (*freefunc) (void *, size_t);
  mp_get_memory_functions (NULL, NULL, &freefunc);
  (*freefunc) (str, strlen (str) + 1);
#else /* GMP < 4.2 */
  __gmp_free_func (str, strlen (str) + 1);
#endif
}
