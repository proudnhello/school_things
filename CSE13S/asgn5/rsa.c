#include "rsa.h"
#include "numtheory.h"
#include "randstate.h"
#include <stdlib.h>

//
// Generates the components for a new public RSA key.
// p and q will be large primes with n their product.
// The product n will be of a specified minimum number of bits.
// The primality is tested using Miller-Rabin.
// The public exponent e will have around the same number of bits as n.
// All mpz_t arguments are expected to be initialized.
//
// p: will store the first large prime.
// q: will store the second large prime.
// n: will store the product of p and q.
// e: will store the public exponent.
//
void rsa_make_pub(mpz_t p, mpz_t q, mpz_t n, mpz_t e, uint64_t nbits,
                  uint64_t iters) {
  // Determine the number of bits that should be used for each prime
  uint64_t pbits = (random() % (nbits / 2)) + (nbits / 4);
  uint64_t qbits = nbits - pbits;

  make_prime(p, pbits, iters);
  make_prime(q, qbits, iters);
  mpz_mul(n, p, q);

  mpz_t p_minus1;
  mpz_t q_minus1;
  mpz_t temp;
  mpz_t prime_gcd;
  mpz_t lam_n;

  mpz_init(p_minus1);
  mpz_init(q_minus1);
  mpz_init(temp);
  mpz_init(prime_gcd);
  mpz_init(lam_n);

  // We need p-1 and q-1 for later calculations, and this is the easiest way of
  // doing it with mpz_ts
  mpz_sub_ui(p_minus1, p, 1);
  mpz_sub_ui(q_minus1, q, 1);

  // Calculated lamda(n)
  mpz_mul(temp, p_minus1, q_minus1);
  gcd(prime_gcd, p_minus1, q_minus1);
  mpz_fdiv_q(lam_n, temp, prime_gcd);

  // Generates random numbers until one is coprime with lamda(n), generating e.
  do {
    mpz_urandomb(e, state, nbits);
    gcd(temp, lam_n, e);
  } while (mpz_cmp_ui(temp, 1) != 0);
  mpz_clear(p_minus1);
  mpz_clear(q_minus1);
  mpz_clear(temp);
  mpz_clear(prime_gcd);
  mpz_clear(lam_n);
  return;
}

//
// Writes a public RSA key to a file.
// Public key contents: n, e, signature, username.
// All mpz_t arguments are expected to be initialized.
//
// n: the public modulus.
// e: the public exponent.
// s: the signature of the username.
// username: the username that was signed as s.
//
void rsa_write_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile) {
  gmp_fprintf(pbfile, "%Zx\n%Zx\n%Zx\n%s\n", n, e, s, username);
  return;
}

//
// Reads a public RSA key from a file.
// Public key contents: n, e, signature, username.
// All mpz_t arguments are expected to be initialized.
//
// n: will store the public modulus.
// e: will store the public exponent.
// s: will store the signature.
// username: an allocated array to hold the username.
// pbfile: the file containing the public key
//
void rsa_read_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile) {
  gmp_fscanf(pbfile, "%Zx\n%Zx\n%Zx\n%s\n", n, e, s, username);
  return;
}

//
// Generates the components for a new private RSA key.
// Requires an accompanying RSA public key to complete the pair.
// All mpz_t arguments are expected to be initialized.
//
// d: will store the RSA private key.
// e: the precomputed public exponent.
// p: the first large prime from the public key generation.
// p: the second large prime from the public key generation.
//
void rsa_make_priv(mpz_t d, mpz_t e, mpz_t p, mpz_t q) {
  mpz_t p_minus1;
  mpz_t q_minus1;
  mpz_t temp;
  mpz_t prime_gcd;
  mpz_t lam_n;

  mpz_init(p_minus1);
  mpz_init(q_minus1);
  mpz_init(temp);
  mpz_init(prime_gcd);
  mpz_init(lam_n);

  // We need p-1 and q-1 for later calculations, and this is the easiest way of
  // doing it with mpz_ts
  mpz_sub_ui(p_minus1, p, 1);
  mpz_sub_ui(q_minus1, q, 1);

  // Calculated lamda(n)
  mpz_mul(temp, p_minus1, q_minus1);
  gcd(prime_gcd, p_minus1, q_minus1);
  mpz_fdiv_q(lam_n, temp, prime_gcd);

  mod_inverse(d, e, lam_n);

  mpz_clears(p_minus1, q_minus1, temp, prime_gcd, lam_n, NULL);
  return;
}

//
// Writes a private RSA key to a file.
// Private key contents: n, d.
// All mpz_t arguments are expected to be initialized.
//
// n: the public modulus.
// d: the private key.
// pvfile: the file to write the private key to.
//
void rsa_write_priv(mpz_t n, mpz_t d, FILE *pvfile) {
  gmp_fprintf(pvfile, "%Zx\n%Zx\n", n, d);
  return;
}

//
// Reads a private RSA key from a file.
// Private key contents: n, d.
// All mpz_t arguments are expected to be initialized.
//
// n: will store the public modulus.
// d: will store the private key.
void rsa_read_priv(mpz_t n, mpz_t d, FILE *pvfile) {
  gmp_fscanf(pvfile, "%Zx\n%Zx\n", n, d);
  return;
}

//
// Encrypts a message given an RSA public exponent and modulus.
// All mpz_t arguments are expected to be initialized.
//
// c: will store the encrypted message.
// m: the message to encrypt.
// e: the public exponent.
// n: the public modulus.
//
void rsa_encrypt(mpz_t c, mpz_t m, mpz_t e, mpz_t n) {
  pow_mod(c, m, e, n);
  return;
}

//
// Encrypts an entire file given an RSA public modulus and exponent.
// All mpz_t arguments are expected to be initialized.
// All FILE * arguments are expected to be properly opened.
//
// infile: the input file to encrypt.
// outfile: the output file to write the encrypted input to.
// n: the public modulus.
// e: the public exponent.
//
void rsa_encrypt_file(FILE *infile, FILE *outfile, mpz_t n, mpz_t e) {
  mpz_t message;
  mpz_t cypher;
  mpz_init(message);
  mpz_init(cypher);
  // Calculates the number of characters that can be processed with n w/o
  // repeats
  int block_size = (mpn_sizeinbase(n->_mp_d, n->_mp_size, 2) - 1) / 8;

  // Then creates an array that can store that many characters
  uint8_t *read_char = calloc(block_size, sizeof(uint8_t));
  read_char[0] = 0xFF;
  int num;

  do {
    // Read num characters out of the file
    num = fread((read_char + 1), sizeof(uint8_t), block_size - 1, infile);
    // If we read any characters
    if (num == 0) {
      break;
    }
    // Encrypt the imported characters
    mpz_import(message, num + 1, 1, sizeof(uint8_t), 1, 0, read_char);
    rsa_encrypt(cypher, message, e, n);
    // Then export them to the outfile
    gmp_fprintf(outfile, "%Zx\n", cypher);

    // Continue doing this until we don't read the maximum number of characters,
    // and are therefore at the end of the file
  } while (num == block_size - 1);

  mpz_clear(message);
  mpz_clear(cypher);
  free(read_char);
  return;
}

//
// Decrypts some ciphertext given an RSA private key and public modulus.
// All mpz_t arguments are expected to be initialized.
//
// m: will store the decrypted message.
// c: the ciphertext to decrypt.
// d: the private key.
// n: the public modulus.
//
void rsa_decrypt(mpz_t m, mpz_t c, mpz_t d, mpz_t n) {
  pow_mod(m, c, d, n);
  return;
}

//
// Decrypts an entire file given an RSA public modulus and private key.
// All mpz_t arguments are expected to be initialized.
// All FILE * arguments are expected to be properly opened.
//
// infile: the input file to decrypt.
// outfile: the output file to write the decrypted input to.
// n: the public modulus.
// d: the private key.
//
void rsa_decrypt_file(FILE *infile, FILE *outfile, mpz_t n, mpz_t d) {
  // Calculates the number of characters that can be processed with n w/o
  // repeats
  int block_size = (mpn_sizeinbase(n->_mp_d, n->_mp_size, 2) - 1) / 8;
  // Then creates an array that can store that many characters
  uint8_t *read_char = calloc(block_size, sizeof(uint8_t));

  mpz_t cypher;
  mpz_t message;
  mpz_init(cypher);
  mpz_init(message);
  size_t x = 1;
  size_t *converted = &x;
  int processed;

  do {
    // Read in processed number of characters from the infile
    processed = gmp_fscanf(infile, "%Zx\n", cypher);
    // If we read any numbers
    if (processed <= 0) {
      break;
    }
    // Decrypt the read in characters, and save the number of characters created
    // in converted
    rsa_decrypt(message, cypher, d, n);
    mpz_export(read_char, converted, 1, sizeof(uint8_t), 1, 0, message);

    // Write the converted characters into the outfile
    for (size_t i = 1; i < *converted; i += 1) {
      gmp_fprintf(outfile, "%c", read_char[i]);
    }

    // Continue to do this until we don't read in any more characters
  } while (processed != 0);

  free(read_char);
  mpz_clear(cypher);
  mpz_clear(message);
  return;
}

//
// Signs some message given an RSA private key and public modulus.
// All mpz_t arguments are expected to be initialized.
//
// s: will store the signed message (the signature).
// m: the message to sign.
// d: the private key.
// n: the public modulus.
//
void rsa_sign(mpz_t s, mpz_t m, mpz_t d, mpz_t n) {
  pow_mod(s, m, d, n);
  return;
}

//
// Verifies some signature given an RSA public exponent and modulus.
// Requires the expected message for verification.
// All mpz_t arguments are expected to be initialized.
//
// m: the expected message.
// s: the signature to verify.
// e: the public exponent.
// n: the public modulus.
// returns: true if signature is verified, false otherwise.
//
bool rsa_verify(mpz_t m, mpz_t s, mpz_t e, mpz_t n) {
  mpz_t test;
  mpz_init(test);
  // Undo the signing
  pow_mod(test, s, e, n);
  // If the undone signing results in the same as message, then return true
  if (mpz_cmp(m, test) == 0) {
    mpz_clear(test);
    return true;
  }
  mpz_clear(test);
  return false;
}
