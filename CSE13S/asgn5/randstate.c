#include "randstate.h"
#include <stdlib.h>

gmp_randstate_t state;

void randstate_init(uint64_t seed) {
  gmp_randinit_mt(state);
  gmp_randseed_ui(state, seed);
  srandom(seed);
}
void randstate_clear(void) { gmp_randclear(state); }