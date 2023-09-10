#include "io.h"
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

uint64_t bytes_read = 0;
uint64_t bytes_written = 0;
static uint8_t read_buffer[BLOCK] = {0};
static uint32_t read_bits = 0;
static uint32_t bits_to_read = 0;
static uint8_t code_buffer[BLOCK] = {0};
static uint32_t code_bit = 0;

int read_bytes(int infile, uint8_t *buf, int nbytes) {
    int so_far = 0;
    int this_batch = 1;
    // While we are still reading in bytes and we haven't yet read in the
    // specified number of bytes
    while (so_far < nbytes && this_batch > 0) {
        // Try to read in the number of bytes left to the buffer
        this_batch = read(infile, buf + so_far, nbytes - so_far);
        // Increment the number of bytes read total
        so_far += this_batch;
        bytes_read += this_batch;
    }

    return so_far;
}

int write_bytes(int outfile, uint8_t *buf, int nbytes) {
    int so_far = 0;
    int this_batch = 1;
    // While we still haven't read in all the specified bytes
    while (so_far < nbytes && this_batch > 0) {
        // Try to read in the bytes that haven't been read so far
        this_batch = write(outfile, buf + so_far, nbytes - so_far);
        so_far += this_batch;
        // inrement the total number of bytes read
        bytes_written += this_batch;
    }

    return so_far;
}

bool read_bit(int infile, uint8_t *bit) {
    // If we have distributed all the bits in block, read in more
    if (read_bits == bits_to_read) {
        uint64_t temp = read_bytes(infile, read_buffer, BLOCK);
        bits_to_read = temp * 8;
        read_bits = 0;
    }
    // if we couldn't read in any more bytes, return fakse
    if (bits_to_read == 0) {
        return false;
    }
    // Get the next bit
    uint32_t block = read_bits / 8;
    uint32_t b = read_bits % 8;

    uint8_t j = read_buffer[block];
    j = j >> b;
    j = j & 1;
    // Return it
    if (j % 2 == 1) {
        *bit = 1;
    } else {
        *bit = 0;
    }

    read_bits += 1;

    return true;
}

void write_code(int outfile, Code *c) {
    // Iterate over all the bits in the code
    for (uint32_t i = 0; i < code_size(c); i += 1) {
        // If the block of codes is full, write what's there
        if (code_bit / 8 == BLOCK) {
            write_bytes(outfile, code_buffer, BLOCK);
            code_bit = 0;
        }
        // get the next bit from the code
        uint32_t block = code_bit / 8;
        uint32_t b = code_bit % 8;
        uint8_t value = code_get_bit(c, i);
        // Write it to the corrisponding spot in code buffer
        if (value == 1) {
            uint8_t j = 1;

            // Should create a uint8 with a 1 at bit
            j = j << (b);
            // Or above with the correct block to keep every set bit the same,
            // and set bit to 1 if it is not already.
            code_buffer[block] = code_buffer[block] | j;
        } else {
            uint8_t j = 1;

            // Should create a uint8 with a 1 at every bit except for bit
            j = j << (b);
            j = UINT8_MAX ^ j;

            // And above with the correct block to keep every set bit the same,
            // but set bit to 0.
            code_buffer[block] = code_buffer[block] & j;
        }
        code_bit += 1;
    }
}

void flush_codes(int outfile) {
    uint32_t block = code_bit / 8;
    uint32_t b = code_bit % 8;
    uint8_t j = UINT8_MAX;
    // Make sure the last byte has 0s in it after the last real code
    j = j >> (8 - b);
    code_buffer[block] = code_buffer[block] & j;
    // Write all the remaining codes to the outfil
    if (code_bit % 8 == 0) {
        write_bytes(outfile, code_buffer, (code_bit / 8));
    } else {
        write_bytes(outfile, code_buffer, (code_bit / 8) + 1);
    }
}
