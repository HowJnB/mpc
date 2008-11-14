#include "write_test.h"

char* header1 =
"# Data file for mpc_norm.\n"
"#\n"
"# Copyright (C) 2008 Andreas Enge\n"
"#\n"
;
char* header2 =
"# For explanations on the file format, see abs.dat.\n\n"
;

int main () {
   int oprei, opimi;
      // index in values table
   char *opre, *opim;

   printf ("%s%s%s", header1, licence, header2);

   printf ("# special values\n");
   for (oprei = 0; oprei < valueslen; oprei++)
   for (opimi = 0; opimi < valueslen; opimi++)
   {
      opre = values [oprei];
      opim = values [opimi];
      if (   (index (infvalues, infvalueslen, opre) != -1)
          || (index (infvalues, infvalueslen, opim) != -1))
      {
         printf ("7 %4s   ", norm (opre, opim));
         printf ("7 %4s 7 %4s N\n", opre, opim);
      }
   }

   printf ("\n# values with only 1, -1, +0, -0\n");
   for (oprei = 0; oprei < valueslen; oprei++)
   for (opimi = 0; opimi < valueslen; opimi++)
   {
      opre = values [oprei];
      opim = values [opimi];
      if (   (index (infvalues, infvalueslen, opre) == -1)
          && (index (infvalues, infvalueslen, opim) == -1))
      {
         printf ("7 %4s   ", norm (opre, opim));
         printf ("7 %4s 7 %4s N\n", opre, opim);
      }
   }

   return 1;
}
