#include <stdio.h>
// stdio.h moved up to ensure that it appears before gmp.h (included with rsa.h)
// when autoformatted, as the code with otherwise break
#include "randstate.h"
#include "rsa.h"
#include <stdlib.h>
#define OPTIONS "b:i:n:d:s:vh"
#include <errno.h>
#include <error.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char **argv) {
  // Set all values to their default
  char *private = "rsa.priv";
  char *public = "rsa.pub";
  uint64_t seed = time(NULL);
  uint64_t bits = 1024;
  uint64_t iters = 50;
  int verbose = 0;
  int help = 0;
  int opt = 0;
  int err = 0;
  char *endptr;

  while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
    switch (opt) {
    // User passed in custom interation
    case ('i'):
      iters = strtoul(optarg, &endptr, 10);
      // If the value is out of range, print error message, then set err to 1
      if (iters < 1 || iters > 500) {
        error(0, 0, "Number of iterations must be 1-500, not %s.", optarg);
        err = 1;
      }
      break;
    // User passed in custom number of bits for the public key
    case ('b'):
      // If the value is out of range, print error message, then set err to 1
      bits = strtoul(optarg, &endptr, 10);
      if (bits < 50 || bits > 4096) {
        error(0, 0, "Number of bits must be 50-4096, not %s.", optarg);
        err = 1;
      }
      break;
      // User passed in custom private key file
    case ('d'):
      private
      = optarg;
      break;
    case ('n'):
      // User passed in custom public key file
      public
      = optarg;
      break;
      // User passed in custom seed
    case ('s'):
      seed = strtoul(optarg, &endptr, 10);
      break;
      // User asked for verbose output
    case ('v'):
      verbose = 1;
      break;
      // User asked for help
    case ('h'):
      help = 1;
      break;
    default:
      err = 1;
      break;
    }
  }
  // If the user asked for help, or inputed something wrong, print the help
  // message to stderr
  if (help || err) {
    fprintf(
        stderr,
        "Usage: %s [options]\n  %s generates a public / private "
        "key pair, placing the keys into the public and private\n  key files "
        "as specified below. The keys have a modulus (n) whose length is "
        "specified in\n  the program options.\n    -s <seed>   : Use <seed> as "
        "the random number seed. Default: time()\n    -b <bits>   : Public "
        "modulus n must have at least <bits> bits. Default: 1024\n    -i "
        "<iters>  : Run <iters> Miller-Rabin iterations for primality testing. "
        "Default: 50\n    -n <pbfile> : Public key file is <pbfile>. Default: "
        "rsa.pub\n    -d <pvfile> : Private key file is <pvfile>. Default: "
        "rsa.priv\n    -v          : Enable verbose output.\n    -h          : "
        "Display program synopsis and usage.\n",
        argv[0], argv[0]);
    // If they only asked for help, return 0. Otherwise, return 1
    return err;
  }

  errno = 0;
  FILE *pub = fopen(public, "w");
  // If opening the public file failed, print an error, and try to close
  // whatever was opened
  if (errno != 0) {
    error(0, 0, "couldn't open %s to write public key.", public);
    return 1;
  }
  errno = 0;
  FILE *priv = fopen(private, "w");
  // If opening the private file failed, print an error, then close both the
  // public and private files
  if (errno != 0) {
    error(0, 0, "couldn't open %s to write private key.", private);
    fclose(pub);
    return 1;
  }

  char *username = getenv("USER");
  // Set the private key's perms to read/write by the user only
  int file_num = fileno(priv);
  if (fchmod(file_num, 0600) != 0) {
    error(0, 0, "couldn't change permisions on %s to proctect private key.", private);
    fclose(pub);
    fclose(priv);
  }

  randstate_init(seed);

  mpz_t s, p, q, n, e, d, user;
  mpz_inits(s, p, q, n, e, d, user, NULL);

  // make a public key with bits and iters, setting p, q, n, and e
  rsa_make_pub(p, q, n, e, bits, iters);
  // create a private key with e, p, and q, setting d
  rsa_make_priv(d, e, p, q);
  // Create a signature based on the username fetched above
  mpz_set_str(user, username, 62);
  rsa_sign(s, user, d, n);

  // Write both keys to their files
  rsa_write_pub(n, e, s, username, pub);
  rsa_write_priv(n, d, priv);

  // If verbose is true, print everything out to stderr
  if (verbose) {
    fprintf(stderr, "username: %s\n", username);
    // mpn_sizeinbase calculates the number of bits used by a variable when
    // in base 2, or the number of bits
    int bits = mpn_sizeinbase(s->_mp_d, s->_mp_size, 2);
    gmp_fprintf(stderr, "user signature (%d bits): %Zd\n", bits, s);

    bits = mpn_sizeinbase(p->_mp_d, p->_mp_size, 2);
    gmp_fprintf(stderr, "p (%d bits): %Zd\n", bits, p);

    bits = mpn_sizeinbase(q->_mp_d, q->_mp_size, 2);
    gmp_fprintf(stderr, "q (%d bits): %Zd\n", bits, q);

    bits = mpn_sizeinbase(n->_mp_d, n->_mp_size, 2);
    gmp_fprintf(stderr, "n - modulus (%d bits): %Zd\n", bits, n);

    bits = mpn_sizeinbase(e->_mp_d, e->_mp_size, 2);
    gmp_fprintf(stderr, "e - public exponent (%d bits): %Zd\n", bits, e);

    bits = mpn_sizeinbase(d->_mp_d, d->_mp_size, 2);
    gmp_fprintf(stderr, "d - private exponent (%d bits): %Zd\n", bits, d);
  }

  mpz_clears(s, p, q, n, e, d, user, NULL);
  randstate_clear();
  fclose(pub);
  fclose(priv);
  return 0;
}
