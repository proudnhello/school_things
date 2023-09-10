#include "bf.h"
#include "bv.h"
#include "city.h"
#include <stdio.h>

#define N_HASHES 5

struct BloomFilter {
    uint64_t salts[N_HASHES]; // Salts for five hash functions
    uint32_t n_keys;
    uint32_t n_hits;
    uint32_t n_misses;
    uint32_t n_bits_examined;
    BitVector *filter;
};

static uint64_t default_salts[] = {0x5adf08ae86d36f21, 0x419d292ea2ffd49e,
                                   0x50d8bb08de3818df, 0x272347aea4045dd5,
                                   0x7c8e16f768811a21};

BloomFilter *bf_create(uint32_t size) {

    BloomFilter *bf = (BloomFilter *)malloc(sizeof(BloomFilter));
    if (bf) {
        bf->n_keys = bf->n_hits = 0;
        bf->n_misses = bf->n_bits_examined = 0;
        for (int i = 0; i < N_HASHES; i++) {
            bf->salts[i] = default_salts[i];
        }
        bf->filter = bv_create(size);
        if (bf->filter == NULL) {
            free(bf);
            bf = NULL;
        }
    }
    return bf;
}

void bf_delete(BloomFilter **bf) {
    bv_delete(&((*bf)->filter));
    free(*bf);
    *bf = NULL;
    return;
}

uint32_t bf_size(BloomFilter *bf) { return bv_length(bf->filter); }

void bf_insert(BloomFilter *bf, char *oldspeak) {
    bf->n_keys += 1;

    // For every salt
    for (int i = 0; i < N_HASHES; i += 1) {
        // Get the hash value
        uint64_t j = hash(bf->salts[i], oldspeak);
        j = j % bv_length(bf->filter);
        // Set the corrisponding bit
        bv_set_bit(bf->filter, j);
    }
    return;
}

bool bf_probe(BloomFilter *bf, char *oldspeak) {

    // For every salt
    for (int i = 0; i < N_HASHES; i += 1) {
        // Get the hash value
        bf->n_bits_examined += 1;
        uint64_t j = hash(bf->salts[i], oldspeak);
        j = j % bv_length(bf->filter);
        // Get the bit at that hash value
        uint8_t bit = bv_get_bit(bf->filter, j);
        // If the bit is 0, then the value does not appear, return false and
        // increment misses
        if (bit == 0) {
            bf->n_misses += 1;
            return false;
        }
    }

    // If the value appeared after every hash, then it either was added, or we
    // hit a false positive, return true and increment hits
    bf->n_hits += 1;
    return true;
}

uint32_t bf_count(BloomFilter *bf) {

    uint32_t count = 0;

    // Iterate over the entire internal bv
    for (uint32_t i = 0; i < bv_length(bf->filter); i += 1) {
        // Get the bit
        if (bv_get_bit(bf->filter, i) == 1) {
            // If it's 1, increment count
            count += 1;
        }
    }

    return count;
}

void bf_print(BloomFilter *bf) {
    bv_print(bf->filter);
    return;
}

void bf_stats(BloomFilter *bf, uint32_t *nk, uint32_t *nh, uint32_t *nm,
              uint32_t *ne) {
    *nk = bf->n_keys;
    *nh = bf->n_hits;
    *nm = bf->n_misses;
    *ne = bf->n_bits_examined;
    return;
}
