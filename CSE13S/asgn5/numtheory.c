#include "numtheory.h"
#include "randstate.h"

void gcd(mpz_t d, mpz_t a, mpz_t b) {
  mpz_t proper_a;
  mpz_t proper_b;
  mpz_t t;
  mpz_init_set(proper_a, a);
  mpz_init_set(proper_b, b);
  mpz_init(t);

  while (mpz_cmp_ui(proper_b, 0) != 0) {
    // t = b
    mpz_set(t, proper_b);
    // b = a mod b
    mpz_mod(proper_b, proper_a, proper_b);
    // a = t
    mpz_set(proper_a, t);
  }
  // out = a
  mpz_set(d, proper_a);
  mpz_clear(proper_a);
  mpz_clear(proper_b);
  mpz_clear(t);
  return;
}

void mod_inverse(mpz_t o, mpz_t a, mpz_t n) {
  mpz_t r;
  mpz_t r_prime;
  mpz_t t;
  mpz_t t_prime;
  mpz_t q;
  mpz_t temp;
  mpz_t temp2;

  mpz_init_set(r, n);
  mpz_init_set(r_prime, a);
  mpz_init_set_ui(t, 0);
  mpz_init_set_ui(t_prime, 1);
  mpz_init(q);
  mpz_init(temp);
  mpz_init(temp2);

  while (mpz_cmp_ui(r_prime, 0) != 0) {
    // q = r/r'
    mpz_fdiv_q(q, r, r_prime);

    // r = r', r' = r-(q*r')
    mpz_set(temp, r);
    mpz_set(r, r_prime);
    mpz_mul(temp2, q, r_prime);
    mpz_sub(r_prime, temp, temp2);

    // t = t', t' = t-(q*t')
    mpz_set(temp, t);
    mpz_set(t, t_prime);
    mpz_mul(temp2, q, t_prime);
    mpz_sub(t_prime, temp, temp2);
  }
  // if r>0, there is no inverse, return 0
  if (mpz_cmp_ui(r, 1) > 0) {
    mpz_set_ui(o, 0);

    mpz_clear(r);
    mpz_clear(r_prime);
    mpz_clear(t);
    mpz_clear(t_prime);
    mpz_clear(q);
    mpz_clear(temp);
    mpz_clear(temp2);
    return;
  } else {
    // If t is negitive, find abs(t)
    if (mpz_cmp_ui(t, 0) < 1) {
      mpz_add(t, t, n);
    }
    mpz_set(o, t);

    mpz_clear(r);
    mpz_clear(r_prime);
    mpz_clear(t);
    mpz_clear(t_prime);
    mpz_clear(q);
    mpz_clear(temp);
    mpz_clear(temp2);
    return;
  }
}

void pow_mod(mpz_t o, mpz_t a, mpz_t d, mpz_t n) {
  mpz_t base;
  mpz_t exponent;
  mpz_t holder;
  mpz_init(base);
  mpz_init(exponent);
  mpz_init(holder);

  mpz_set(base, a);
  mpz_set(exponent, d);
  mpz_set_ui(o, 1);

  // while the exponent is greater than 0
  while (mpz_cmp_ui(exponent, 0) > 0) {
    // If the exponent is odd
    if (mpz_mod_ui(holder, exponent, 2) == 1) {
      // set the output to output*base mod n
      mpz_mul(holder, o, base);
      mpz_mod(o, holder, n);
    }
    // Set the base to base*base mod n
    mpz_mul(holder, base, base);
    mpz_mod(base, holder, n);
    // set the exponent to exponent/2
    mpz_fdiv_q_ui(exponent, exponent, 2);
  }
  mpz_clear(base);
  mpz_clear(exponent);
  mpz_clear(holder);
}

bool is_prime(mpz_t n, uint64_t iters) {

  // If n is less than 2, 2, 3, or 4, return immidiately, as these are easy
  // cases to identify, and my code breaks with them
  if (mpz_cmp_ui(n, 2) == 0 || mpz_cmp_ui(n, 3) == 0) {
    return true;
  }
  if (mpz_cmp_ui(n, 4) == 0 || mpz_cmp_ui(n, 2) <= 0) {
    return false;
  }
  // Return immidiately if n is even, as this is an easy check
  mpz_t even_test;
  mpz_init(even_test);
  if (mpz_mod_ui(even_test, n, 2) == 0) {
    mpz_clear(even_test);
    return false;
  }
  mpz_clear(even_test);

  mpz_t s, r, a, y, j, random_bound, two, n_minus_1, holder;

  mpz_init(n_minus_1);
  mpz_sub_ui(n_minus_1, n, 1);
  mpz_init_set_ui(s, 0);
  mpz_init_set(r, n_minus_1);
  mpz_init(a);
  mpz_init(y);
  mpz_init(j);
  mpz_init(random_bound);
  mpz_sub_ui(random_bound, n, 3);
  mpz_init_set_ui(two, 2);
  mpz_init(holder);

  // holder doesn't do anything here, but mpz_mod_ui needs 3 arguments
  // This generates n-1 = r * 2^s, where r is odd
  while (mpz_mod_ui(holder, r, 2) == 0 && mpz_cmp_ui(r, 0) != 0) {
    mpz_fdiv_q_ui(r, r, 2);
    mpz_add_ui(s, s, 1);
  }

  for (uint64_t i = 0; i < iters; i += 1) {
    // generates a number between 0 and n-3 - 1
    mpz_urandomm(a, state, random_bound);
    // moves the random number from between 0, n-4 to 2, n-2
    mpz_add_ui(a, a, 2);
    // y = a^r mod n
    pow_mod(y, a, r, n);
    // If y is not 0 nor n-1
    if (mpz_cmp_ui(y, 1) != 0 && mpz_cmp(y, n_minus_1) != 0) {
      mpz_set_ui(j, 1);
      // Iterate from j = 1 to s, ending if y = n-1
      while (mpz_cmp(j, s) < 0 && mpz_cmp(y, n_minus_1) != 0) {
        // y = y^2 mod n
        pow_mod(y, y, two, n);
        // if y == 1, then the value is not prime
        if (mpz_cmp_ui(y, 1) == 0) {
          mpz_clear(s);
          mpz_clear(r);
          mpz_clear(a);
          mpz_clear(y);
          mpz_clear(j);
          mpz_clear(random_bound);
          mpz_clear(two);
          mpz_clear(n_minus_1);
          mpz_clear(holder);
          return false;
        }
        mpz_add_ui(j, j, 1);
      }
      // If the loop ended because y j = s, the value is not prime
      if (mpz_cmp(y, n_minus_1) != 0) {
        mpz_clear(s);
        mpz_clear(r);
        mpz_clear(a);
        mpz_clear(y);
        mpz_clear(j);
        mpz_clear(random_bound);
        mpz_clear(two);
        mpz_clear(n_minus_1);
        mpz_clear(holder);
        return false;
      }
    }
  }
  // If the number survived the above loops w/o returning false, then it might
  // be prime, so return true
  mpz_clear(s);
  mpz_clear(r);
  mpz_clear(a);
  mpz_clear(y);
  mpz_clear(j);
  mpz_clear(random_bound);
  mpz_clear(two);
  mpz_clear(n_minus_1);
  mpz_clear(holder);

  return true;
}

void make_prime(mpz_t p, uint64_t bits, uint64_t iters) {
  mpz_t min;
  mpz_t holder;
  mpz_init_set_ui(min, 1);
  mpz_init(holder);

  // Generate a random number that has bits-1 bits, then set the bits bit to 1

  mpz_mul_2exp(min, min, (bits - 1));

  mpz_urandomb(p, state, (bits - 1));

  mpz_ior(p, p, min);

  // if p is even, make it odd
  if (mpz_mod_ui(holder, p, 2) == 0) {
    mpz_add_ui(p, p, 1);
  }

  // Iterate over all the odd numbers, starting at p, until one is prime
  while (is_prime(p, iters) == false) {
    mpz_add_ui(p, p, 2);
  }

  mpz_clears(min, holder, NULL);
  return;
}
