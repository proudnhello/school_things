#include <stdio.h>
// stdio.h moved up to ensure that it appears before gmp.h (included with rsa.h)
// when autoformatted, as the code with otherwise break
#include "randstate.h"
#include "rsa.h"
#define OPTIONS "i:o:n:vh"
#include <errno.h>
#include <error.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
  // Set default values
  FILE *input = stdin;
  FILE *output = stdout;
  char *in = NULL;
  char *out = NULL;
  char *pub = "rsa.pub";
  int verbose = 0;
  int help = 0;
  int err = 0;
  int opt = 0;

  while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
    switch (opt) {
    // User specified input file
    case ('i'):
      in = optarg;
      break;
    // User specified output file
    case ('o'):
      out = optarg;
      break;
    // User specified a different public key file
    case ('n'):
      pub = optarg;
      break;
    // User wants verbose output
    case ('v'):
      verbose = 1;
      break;
    // User wants help
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
        "Usage: %s [options]\n  %s encrypts an input file using "
        "the specified public key file,\n  writing the result to the specified "
        "output file.\n    -i <infile> : Read input from <infile>. Default: "
        "standard input.\n    -o <outfile>: Write output to <outfile>. "
        "Default: standard output.\n    -n <keyfile>: Public key is in "
        "<keyfile>. Default: rsa.pub.\n    -v          : Enable verbose "
        "output.\n    -h          : Display program synopsis and usage.\n",
        argv[0], argv[0]);
    return err;
  }

  // If the user set an input file, try to open that file
  if (in != NULL) {
    errno = 0;
    input = fopen(in, "r");
    // If opening the file failed, print an error, and try to close whatever was
    // opened
    if (errno != 0) {
      error(0, 0, "couldn't open %s to read input file.", in);
      return 1;
    }
  }

  // If the user set an output file, try to open that file
  if (out != NULL) {
    errno = 0;
    output = fopen(out, "w");
    // If opening the file failed, print an error, and try to close whatever was
    // opened
    if (errno != 0) {
      error(0, 0, "couldn't open %s to write output file.", out);
      fclose(input);
      return 1;
    }
  }

  errno = 0;
  FILE *public = fopen(pub, "r");
  // If opening the file failed, print an error, and try to close whatever was
  // opened
  if (errno != 0) {
    error(0, 0, "couldn't open %s to read public key.", pub);
    fclose(output);
    fclose(input);
    return 1;
  }

  mpz_t n, e, s, user;
  mpz_inits(n, e, s, user, NULL);
  // Create a string to hold the username of the user.
  // LOGIN_NAME_MAX is the maximum length of a username, so in theory this
  // should allocate enough memory.
  char *username = calloc(LOGIN_NAME_MAX, sizeof(char));

  // Read the public key into n, e, s, and username
  rsa_read_pub(n, e, s, username, public);

  // If verbose is enabled, print all of the above to stderr
  if (verbose) {

    fprintf(stderr, "username: %s\n", username);

    // mpn_sizeinbase calculates the number of bits used by a variable when
    // in base 2, or the number of bits
    int bits = mpn_sizeinbase(s->_mp_d, s->_mp_size, 2);
    gmp_fprintf(stderr, "user signature (%d bits): %Zd\n", bits, s);

    bits = mpn_sizeinbase(n->_mp_d, n->_mp_size, 2);
    gmp_fprintf(stderr, "n - modulus (%d bits): %Zd\n", bits, n);

    bits = mpn_sizeinbase(e->_mp_d, e->_mp_size, 2);
    gmp_fprintf(stderr, "e - public exponent (%d bits): %Zd\n", bits, e);
  }

  // Turn the username into a base 62 integer
  mpz_set_str(user, username, 62);

  // If rsa verify fails, throw an error, then exit.
  if (!rsa_verify(user, s, e, n)) {
    error(0, 0, "Couldn't verify user signature - exiting!");
    return 1;
  }

  // Encrypt the file from input, then save it in output
  rsa_encrypt_file(input, output, n, e);

  fclose(input);
  fclose(output);
  fclose(public);
  mpz_clears(n, e, s, user, NULL);
  free(username);
  return 0;
}
