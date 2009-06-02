/* Return non-zero iff c+i*d is an exact square (a+i*b)^2,
   with a, b both of the form m*2^e with m, e integers.
   If so, returns in a+i*b the corresponding square root, with a >= 0.
   The variables a, b must not overlap with c, d.

   We have c = a^2 - b^2 and d = 2*a*b.

   Case 1: a <> 0 and b <> 0.
   Let a = m*2^e and b = n*2^f with m, e, n, f integers, m and n odd
   (we will treat apart the case a = 0 or b = 0).
   Then 2*a*b = m*n*2^(e+f+1), thus necessarily e+f >= -1.
   Assume e < 0, then f >= 0, then a^2 - b^2 = m^2*2^(2e) - n^2*2^(2f) cannot
   be an integer, since n^2*2^(2f) is an integer, and m^2*2^(2e) is not.
   Similarly when f < 0 (and thus e >= 0).
   Thus we have e, f >= 0, and a, b are both integers.
   Let A = 2a^2, then eliminating b between c = a^2 - b^2 and d = 2*a*b
   gives A^2 - 2c*A - d^2 = 0, which has solutions c +/- sqrt(c^2+d^2).
   We thus need c^2+d^2 to be a square, and c + sqrt(c^2+d^2) --- the solution
   we are interested in --- to be two times a square. Then b = d/(2a) is
   necessarily an integer.

   Case 2: a = 0. Then d is necessarily zero, thus it suffices to check
   whether c = -b^2, i.e., if -c is a square.

   Case 3: b = 0. Then d is necessarily zero, thus it suffices to check
   whether c = a^2, i.e., if c is a square.       
*/
int
mpc_is_exact_square (mpz_t a, mpz_t b, mpz_t c, mpz_t d)
{
  if (mpz_cmp_ui (d, 0)) /* case a = 0 or b = 0 */
    {
      if (mpz_perfect_square_p (c)) /* case 3 above, also covers c=d=0 */
        {
          mpz_sqrt (a, c);
          mpz_set_ui (b, 0);
          return 1; /* c + i*d = (a + i*0)^2 */
        }
      if (mpz_cmp_ui (c, 0) < 0)
        {
          mpz_neg (b, c);
          if (mpz_perfect_square_p (b)) /* case 2 above */
            {
              mpz_sqrt (b, b);
              mpz_set_ui (a, 0);
              return 1; /* c + i*d = (0 + i*b)^2 */
            }
        }
    }
  else /* both a and b are non-zero */
    {
      if (mpz_divisible_2exp_p (d, 1) == 0)
        return 0; /* d must be even */
      mpz_mul (a, c, c);
      mpz_addmul (a, d, d); /* c^2 + d^2 */
      if (mpz_perfect_square_p (a))
        {
          mpz_sqrt (a, a);
          mpz_add (a, c, a); /* c + sqrt(c^2+d^2) */
          if (mpz_divisible_2exp_p (a, 1))
            {
              mpz_div_2exp (a, a, 1);
              if (mpz_perfect_square_p (a))
                {
                  mpz_sqrt (a, a);
                  mpz_div_2exp (b, d, 1); /* d/2 */
                  mpz_divexact (b, b, a); /* d/(2a) */
                  return 1;
                }
            }
        }
    }
  return 0; /* not a square */
}
