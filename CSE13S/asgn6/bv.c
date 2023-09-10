#include "bv.h"
#include <stdio.h>
#include <stdlib.h>

struct BitVector {
    uint32_t length;
    uint64_t *vector;
};

BitVector *bv_create(uint32_t length) {
    BitVector *bv = (BitVector *)malloc(sizeof(BitVector));
    if (bv != NULL) {
        bv->length = length;
        uint32_t arr_len = length / 64;
        // Because of floor div, if length is a multiple of 64, this gets the
        // correct value. Otherwise, we have to add 1 to it.
        if (length % 64 != 0) {
            arr_len += 1;
        }
        // If calloc fails here, the bv isn't going to be created properly, so
        // we free it and null the pointer
        bv->vector = calloc(arr_len, sizeof(uint64_t));
        if (bv->vector == NULL) {
            free(bv);
            bv = NULL;
        }
    }
    return bv;
}

void bv_delete(BitVector **bv) {
    BitVector *delete = *bv;
    free(delete->vector);
    delete->vector = NULL;
    free(delete);
    *bv = NULL;
    return;
}

uint32_t bv_length(BitVector *bv) { return bv->length; }

void bv_set_bit(BitVector *bv, uint32_t i) {
    if (i > bv->length) {
        return;
    }

    uint32_t block = i / 64;
    uint32_t bit = i % 64;

    uint64_t j = 1;

    // Should create a uint64 with a 1 at bit
    j = j << (bit);
    // Or above with the correct block to keep every set bit the same, and set
    // bit to 1 if it is not already.
    bv->vector[block] = bv->vector[block] | j;

    return;
}

void bv_clr_bit(BitVector *bv, uint32_t i) {
    if (i > bv->length) {
        return;
    }

    uint32_t block = i / 64;
    uint32_t bit = i % 64;

    uint64_t j = 1;

    // Should create a uint64 with a 1 at every bit except for bit
    j = j << (bit);
    j = UINT64_MAX ^ j;

    // And above with the correct block to keep every set bit the same, but set
    // bit to 0.
    bv->vector[block] = bv->vector[block] & j;

    return;
}

uint8_t bv_get_bit(BitVector *bv, uint32_t i) {
    if (i > bv->length) {
        return 0;
    }

    uint32_t block = i / 64;
    uint32_t bit = i % 64;
    uint8_t returnable = 0;

    uint64_t j = bv->vector[block];

    // Create a uint64 consisting of just bit
    j = j >> bit;
    j = j & 1;

    // If the bit is 1, return 1
    if (j == 1) {
        returnable = 1;
    }

    // Otherwise, return 0
    return returnable;
}

void bv_print(BitVector *bv) {

    if (bv == NULL) {
        char *null = NULL;
        printf("%s\n", null);
        return;
    }

    uint32_t count = 0;

    // Get the length of the internal array
    uint32_t arr_len = bv->length / 64;
    if (bv->length % 64 != 0) {
        arr_len += 1;
    }

    // Iterate over the internal array
    for (uint32_t i = 0; i < arr_len; i += 1) {
        // Iterate over the bits inside each uint64
        for (uint32_t j = 0; j < 64; j += 1) {
            // If we are still inside the bit vector
            if (((i * 64) + j) < bv->length) {
                // Shift the current uint64 to the right by j
                // As the least signifigant bit is the only bit capable of
                // making a number odd, if the resulting uint64 is odd, then the
                // lsb must be 1 Therefore, print 1
                if ((bv->vector[i] >> j) % 2 == 1) {
                    printf("1");
                } else {
                    // Otherwise, print a 0
                    printf("0");
                }
                count += 1;
                // For legability, print a ' ' every 8 bits
                if (count == 8) {
                    count = 0;
                    printf(" ");
                }
            }
        }
        // For legability, print a '\n' every 64 bits
        printf("\n");
    }

    return;
}
