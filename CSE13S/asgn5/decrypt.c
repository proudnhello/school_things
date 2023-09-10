#include <stdio.h>
// stdio.h moved up to ensure that it appears before gmp.h (included with rsa.h)
// when autoformatted, as the code with otherwise break
#include "randstate.h"
#include "rsa.h"
#define OPTIONS "i:o:n:vh"
#include <errno.h>
#include <error.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
  // Set default values
  FILE *input = stdin;
  FILE *output = stdout;
  char *in = NULL;
  char *out = NULL;
  char *priv = "rsa.priv";
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
      priv = optarg;
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
        "Usage: %s [options]\n  %s decrypts an input "
        "file using the specified private key file,\n  writing the result to "
        "the specified output file.\n    -i <infile> : Read input from "
        "<infile>. Default: standard input.\n    -o <outfile>: Write output to "
        "<outfile>. Default: standard output.\n    -n <keyfile>: Private key "
        "is in <keyfile>. Default: rsa.priv.\n    -v          : Enable verbose "
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
  FILE *private = fopen(priv, "r");
  // If opening the file failed, print an error, and try to close whatever was
  // opened
  if (errno != 0) {
    error(0, 0, "couldn't open %s to read private key.", priv);
    fclose(output);
    fclose(input);
    return 1;
  }

  mpz_t n, d;
  mpz_inits(n, d, NULL);

  // Read the private file into n and d
  rsa_read_priv(n, d, private);

  // If the user asked for verbose outputs, print those to stderr
  if (verbose) {
    int bits = mpn_sizeinbase(n->_mp_d, n->_mp_size, 2);
    gmp_fprintf(stderr, "n - modulus (%d bits): %Zd\n", bits, n);

    bits = mpn_sizeinbase(d->_mp_d, d->_mp_size, 2);
    gmp_fprintf(stderr, "d - private exponent (%d bits): %Zd\n", bits, d);
  }

  // Decrypt the file in input, and put the result into output.
  rsa_decrypt_file(input, output, n, d);

  mpz_clears(n, d, NULL);
  fclose(private);
  fclose(input);
  fclose(output);
  return 0;
}
