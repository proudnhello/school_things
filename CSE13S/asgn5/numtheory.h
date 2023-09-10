#pragma once

#include <stdio.h>
// stdio.h moved up to ensure that it appears before gmp.h when autoformatted,
// as the code with otherwise break
#include <gmp.h>
#include <stdbool.h>
#include <stdint.h>

void gcd(mpz_t d, mpz_t a, mpz_t b);

void mod_inverse(mpz_t o, mpz_t a, mpz_t n);

void pow_mod(mpz_t o, mpz_t a, mpz_t d, mpz_t n);

bool is_prime(mpz_t n, uint64_t iters);

void make_prime(mpz_t p, uint64_t bits, uint64_t iters);
