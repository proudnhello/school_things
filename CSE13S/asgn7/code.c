#include "code.h"
#include <stdio.h>

Code code_init(void) {
    Code code;
    code.top = 0;
    for (int i = 0; i < MAX_CODE_SIZE; i += 1) {
        code.bits[i] = 0;
    }
    return code;
}

uint32_t code_size(Code *c) { return c->top; }

bool code_empty(Code *c) { return c->top == 0; }

bool code_full(Code *c) { return c->top == ALPHABET - 1; }

bool code_set_bit(Code *c, uint32_t i) {
    if (i > ALPHABET - 1) {
        return false;
    }
    // Find the unit8 it's in, and the bit inside the uint8 as well
    uint32_t block = i / 8;
    uint32_t bit = i % 8;

    uint8_t j = 1;

    // Should create a uint8 with a 1 at bit
    j = j << (bit);
    // Or above with the correct block to keep every set bit the same, and set
    // bit to 1 if it is not already.
    c->bits[block] = c->bits[block] | j;

    return true;
}

bool code_clr_bit(Code *c, uint32_t i) {
    if (i > ALPHABET - 1) {
        return false;
    }
    // Find the unit8 it's in, and the bit inside the uint8 as well
    uint32_t block = i / 8;
    uint32_t bit = i % 8;

    uint8_t j = 1;

    // Should create a uint8 with a 1 at every bit except for bit
    j = j << (bit);
    j = UINT8_MAX ^ j;

    // And above with the correct block to keep every set bit the same, but set
    // bit to 0.
    c->bits[block] = c->bits[block] & j;

    return true;
}

bool code_get_bit(Code *c, uint32_t i) {
    if (i > ALPHABET - 1) {
        return false;
    }
    // Find the unit8 it's in, and the bit inside the uint8 as well
    uint32_t block = i / 8;
    uint32_t bit = i % 8;
    uint8_t j = c->bits[block];

    // Create a uint8 with the lsb being bit
    j = j >> bit;
    j = j & 1;

    if (j % 2 == 1) {
        return true;
    }

    return false;
}

bool code_push_bit(Code *c, uint8_t bit) {
    if (c->top == ALPHABET) {
        return false;
    }
    // Set the bit at top to the pased in bit
    if (bit == 0) {
        code_clr_bit(c, c->top);
    } else {
        code_set_bit(c, c->top);
    }
    c->top += 1;
    return true;
}

bool code_pop_bit(Code *c, uint8_t *bit) {
    if (c->top == 0) {
        return false;
    }
    // Get and return the bit at top
    c->top -= 1;
    *bit = code_get_bit(c, c->top);
    return true;
}

void code_print(Code *c) {
    int count = 0;
    for (uint32_t i = 0; i < c->top; i += 1) {
        printf("%u", code_get_bit(c, i));
        count += 1;
        if (count % 8 == 0) {
            printf(" ");
        }
        if (count % 64 == 0) {
            printf("\n");
        }
    }
    if (count % 64 != 0) {
        printf("\n");
    }
}
