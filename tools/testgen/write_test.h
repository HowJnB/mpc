#include <stdio.h>
#include <string.h>

char* licence =
"# This file is part of the MPC Library.\n"
"#\n"
"# The MPC Library is free software; you can redistribute it and/or modify\n"
"# it under the terms of the GNU Lesser General Public License as published by\n"
"# the Free Software Foundation; either version 2.1 of the License, or (at your\n"
"# option) any later version.\n"
"#\n"
"# The MPC Library is distributed in the hope that it will be useful, but\n"
"# WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY\n"
"# or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public\n"
"# License for more details.\n"
"#\n"
"# You should have received a copy of the GNU Lesser General Public License\n"
"# along with the MPC Library; see the file COPYING.LIB.  If not, write to\n"
"# the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,\n"
"# MA 02111-1307, USA.\n"
"#\n"
;

char* values [] = {"-inf", "-1", "-0", "+0", "1", "+inf", "nan"};
int valueslen = 7;
char* infvalues [] = {"-inf", "+inf", "nan"};
int infvalueslen = 3;

int equals (char* a, char* b) {
   return !strcmp (a, b);
}

int index (char** table, int length, char* value) {
   // returns the index of value in the table,
   // or -1 if it is not found
   int i;
   for (i = 0; i < length; i++)
      if (equals (table [i], value))
         return i;
   return -1;
}

char* mul_fr_fr (char *op1, char *op2) {
   // returns op1*op2
   if (equals (op1, "nan") || equals (op2, "nan"))
      return "nan";
   // none is nan
   else if (equals (op1, "-inf")) {
      // res = op1 * values
      char* res [] = {"+inf", "+inf", "nan", "nan", "-inf", "-inf", ""};
      return res [index (values, valueslen, op2)];
   }
   else if (equals (op1, "-1")) {
      char* res [] = {"+inf", "1", "+0", "-0", "-1", "-inf", ""};
      return res [index (values, valueslen, op2)];
   }
   else if (equals (op1, "-0")) {
      char* res [] = {"nan", "+0", "+0", "-0", "-0", "nan", ""};
      return res [index (values, valueslen, op2)];
   }
   else if (equals (op1, "+0")) {
      char* res [] = {"nan", "-0", "-0", "+0", "+0", "nan", ""};
      return res [index (values, valueslen, op2)];
   }
   else if (equals (op1, "1")) {
      char* res [] = {"-inf", "-1", "-0", "+0", "1", "+inf", ""};
      return res [index (values, valueslen, op2)];
   }
   else if (equals (op1, "+inf")) {
      char* res [] = {"-inf", "-inf", "nan", "nan", "+inf", "+inf", ""};
      return res [index (values, valueslen, op2)];
   }
   else
      return "?";
}

char* div_fr_fr (char *op1, char *op2) {
   // returns op1/op2
   char *r;
//   printf (" %s %s", op1, op2);
   if (equals (op1, "nan") || equals (op2, "nan"))
      r = "nan";
   else if (equals (op2, "-inf")) {
      char* res [] = {"nan", "+0", "+0", "-0", "-0", "nan", ""};
      r = res [index (values, valueslen, op1)];
   }
   else if (equals (op2, "-2")) {
      char* res [] = {"+inf", "0.5", "+0", "-0", "-0.5", "-inf", ""};
      if (equals (op1, "-2"))
         r = "1";
      else if (equals (op1, "2"))
         r =  "-1";
      else if (equals (op1, "0"))
         r = "0";
      else
         r = res [index (values, valueslen, op1)];
   }
   else if (equals (op2, "-1")) {
      char* res [] = {"+inf", "1", "+0", "-0", "-1", "-inf", ""};
      r = res [index (values, valueslen, op1)];
   }
   else if (equals (op2, "-0")) {
      char* res [] = {"+inf", "+inf", "nan", "nan", "-inf", "-inf", ""};
      if (equals (op1, "0"))
         r = "nan";
      else
         r = res [index (values, valueslen, op1)];
   }
   else if (equals (op2, "+0")) {
      char* res [] = {"-inf", "-inf", "nan", "nan", "+inf", "+inf", ""};
      if (equals (op1, "0"))
         r = "nan";
      else
         r = res [index (values, valueslen, op1)];
   }
   else if (equals (op2, "1")) {
      char* res [] = {"-inf", "-1", "-0", "+0", "1", "+inf", ""};
      r = res [index (values, valueslen, op1)];
   }
   else if (equals (op2, "2")) {
      char* res [] = {"-inf", "-0.5", "-0", "+0", "0.5", "+inf", ""};
      if (equals (op1, "-2"))
         r = "-1";
      else if (equals (op1, "2"))
         r = "1";
      else if (equals (op1, "0"))
         r = "0";
      else
         r = res [index (values, valueslen, op1)];
   }
   else if (equals (op2, "+inf")) {
      char* res [] = {"nan", "-0", "-0", "+0", "+0", "nan", ""};
      r = res [index (values, valueslen, op1)];
   }
   else
      r = "?";
//   printf ("  %s\n", r);
   return r;
}

char* add_fr_fr (char *op1, char *op2) {
   // returns op1+op2
   if (equals (op1, "nan") || equals (op2, "nan"))
      return "nan";
   else if (equals (op1, "-inf")) {
      char* res [] = {"-inf", "-inf", "-inf", "-inf", "-inf", "nan", ""};
      return res [index (values, valueslen, op2)];
   }
   else if (equals (op1, "-1")) {
      char* res [] = {"-inf", "-2", "-1", "-1", "0", "+inf", ""};
      return res [index (values, valueslen, op2)];
   }
   else if (equals (op1, "-0")) {
      char* res [] = {"-inf", "-1", "-0", "0", "1", "+inf", ""};
      return res [index (values, valueslen, op2)];
   }
   else if (equals (op1, "+0")) {
      char* res [] = {"-inf", "-1", "0", "+0", "1", "+inf", ""};
      return res [index (values, valueslen, op2)];
   }
   else if (equals (op1, "1")) {
      char* res [] = {"-inf", "0", "1", "1", "2", "+inf", ""};
      return res [index (values, valueslen, op2)];
   }
   else if (equals (op1, "+inf")) {
      char* res [] = {"nan", "+inf", "+inf", "+inf", "+inf", "+inf", ""};
      return res [index (values, valueslen, op2)];
   }
   else
      return "?";
}

char* neg_fr (char *op) {
   // returns -op
   if (index (values, valueslen, op) != -1) {
      char* res [] = {"+inf", "1", "+0", "-0", "-1", "-inf", "nan"};
      return res [index (values, valueslen, op)];
   }
   else if (equals (op, "0"))
      return "0";
   else
      return "?";
}

char* sub_fr_fr (char *op1, char *op2) {
   // returns op1-op2
   return add_fr_fr (op1, neg_fr (op2));
}

char* norm (char *opre, char *opim) {
   // returns opre^2+opim^2
   if (equals (opre, "nan") || equals (opim, "nan"))
      if (   (equals (opre, "-inf") || equals (opre, "+inf"))
          || (equals (opim, "-inf") || equals (opim, "+inf")))
         return "+inf";
      else
         return "nan";
   else
      return add_fr_fr (mul_fr_fr (opre, opre), mul_fr_fr (opim, opim));
}

char* div_re (char *op1re, char *op1im, char *op2re, char *op2im) {
   // returns real part of complex division
   if (   equals (op1re, "nan") || equals (op1im, "nan")
       || equals (op2re, "nan") || equals (op2im, "nan"))
       return "nan";
   else if (equals (op2im, "-0") || equals (op2im, "+0"))
      return div_fr_fr (op1re, op2re);
   else if (equals (op2re, "-0") || equals (op2re, "+0"))
      return div_fr_fr (op1im, op2im);
   else
      return div_fr_fr (
         add_fr_fr (mul_fr_fr (op1re, op2re), mul_fr_fr (op1im, op2im)),
         norm (op2re, op2im));
}

char* div_im (char *op1re, char *op1im, char *op2re, char *op2im) {
   // returns imaginary part of complex division
   if (   equals (op1re, "nan") || equals (op1im, "nan")
       || equals (op2re, "nan") || equals (op2im, "nan"))
       return "nan";
   else if (equals (op2im, "-0") || equals (op2im, "+0"))
      return div_fr_fr (op1im, op2re);
   else if (equals (op2re, "-0") || equals (op2re, "+0"))
      return neg_fr (div_fr_fr (op1re, op2im));
   else
      return div_fr_fr (
         sub_fr_fr (mul_fr_fr (op1im, op2re), mul_fr_fr (op1re, op2im)),
         norm (op2re, op2im));
}

