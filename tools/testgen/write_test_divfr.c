#include "write_test.h"

char* header1 =
"# Data file for mpc_div_fr.\n"
"#\n"
"# Copyright (C) 2008 Andreas Enge\n"
"#\n"
;
char* header2 =
"# For explanations on the file format, see add_fr.dat.\n\n"
;

int main () {
   int op1rei, op1imi, op2i;
      // index in values table
   char *op1re, *op1im, *op2;

   printf ("%s%s%s", header1, licence, header2);

   printf ("# special values\n");
   for (op1rei = 0; op1rei < valueslen; op1rei++)
   for (op1imi = 0; op1imi < valueslen; op1imi++)
   for (op2i = 0; op2i < valueslen; op2i++)
   {
      op1re = values [op1rei];
      op1im = values [op1imi];
      op2 = values [op2i];
      if (   (index (infvalues, infvalueslen, op1re) != -1)
          || (index (infvalues, infvalueslen, op1im) != -1)
          || (index (infvalues, infvalueslen, op2) != -1))
      {
         // at least one infinite value
         printf ("7 %4s 7 %4s   ",
            div_fr_fr (op1re, op2), div_fr_fr (op1im, op2));
         printf ("7 %4s 7 %4s  7 %4s  N N\n",
            op1re, op1im, op2);
      }
   }

   printf ("\n# values with only 1, -1, +0, -0\n");
   for (op1rei = 0; op1rei < valueslen; op1rei++)
   for (op1imi = 0; op1imi < valueslen; op1imi++)
   for (op2i = 0; op2i < valueslen; op2i++)
   {
      op1re = values [op1rei];
      op1im = values [op1imi];
      op2 = values [op2i];
      if (   (index (infvalues, infvalueslen, op1re) == -1)
          && (index (infvalues, infvalueslen, op1im) == -1)
          && (index (infvalues, infvalueslen, op2) == -1))
      {
         printf ("7 %4s 7 %4s   ",
            div_fr_fr (op1re, op2), div_fr_fr (op1im, op2));
         printf ("7 %4s 7 %4s  7 %4s  N N\n",
            op1re, op1im, op2);
      }
   }

   return 1;
}
