#include "code.h"
#include "defines.h"
#include "header.h"
#include "huffman.h"
#include "io.h"
#include "node.h"
#include "pq.h"
#include "stack.h"
#include <error.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define OPTIONS "hi:o:v"
/*
//Code taken from Athanasius Kircher#1515 begins here
void spaces(int j){
        for(int i = 0; i<=j; i++){
                printf(" ");
        }
}

void print_tree(Node *t, int depth) {
    if (t) {
        print_tree(t->left, depth + 1);
        spaces(4 * depth);
        node_print(t);
        print_tree(t->right, depth + 1);
    }
    return;
}
//Code taken from Athanasius Kircher#1515 ends here
*/
int main(int argc, char **argv) {

    bytes_read = 0;
    bytes_written = 0;
    int infile = STDIN_FILENO;
    int outfile = STDOUT_FILENO;
    bool stats = false;
    bool help = false;
    bool err = false;
    // Process the command line inputs
    int opt = 0;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case ('h'):
            help = true;
            break;
        case ('v'):
            stats = true;
            break;
        case ('i'):
            infile = open(optarg, O_RDONLY);
            if (infile == -1) {
                error(0, 0, "Something went wrong opening the infile.");
                err = true;
            }
            break;
        case ('o'):
            outfile = open(optarg, O_WRONLY | O_TRUNC | O_CREAT, S_IRWXU);
            if (outfile == -1) {
                error(0, 0, "Something went wrong opening the outfile.");
                err = true;
            }
            break;
        default:
            err = true;
        }
    }

    if (err || help) {
        fprintf(stderr,
                "SYNOPSIS\n  A Huffman encoder.\n  Compresses a file using the "
                "Huffman coding algorithm.\n\nUSAGE\n  %s [-h] [-i infile] [-o "
                "outfile]\n\nOPTIONS\n  -h             Program usage and "
                "help.\n  -v             Print compression statistics.\n  -i "
                "infile      Input file to compress.\n  -o outfile     Output "
                "of compressed data.\n",
                argv[0]);
        if (help) {
            return 0;
        }
        if (err) {
            return 1;
        }
    }

    int unique_characters = 0;

    uint8_t in_buffer[BLOCK] = {0};
    uint8_t *buffer2 = NULL;
    uint64_t hist[ALPHABET] = {0};
    uint64_t read = 1;
    uint64_t processed = 1;
    uint64_t total = 0;

    // While we are still reading bytes
    while (read != 0) {
        // Try to read in block bytes
        read = read_bytes(infile, in_buffer, BLOCK);
        // Make the buffer array larger to contain all of the newly read in
        // bytes
        uint8_t *temp = (uint8_t *)realloc(buffer2, bytes_read);
        if (temp == NULL) {
            fprintf(stderr, "Not enough memory\n");
            free(buffer2);
            return 1;
        } else {
            buffer2 = temp;
        }
        // Save the read in bytes to the buffer, then increment the histogram
        // based on the character
        for (processed = 0; processed < read; processed += 1) {
            if (hist[in_buffer[processed]] == 0) {
                unique_characters += 1;
            }
            hist[in_buffer[processed]] += 1;
            buffer2[total] = in_buffer[processed];
            total += 1;
        }
    }

    struct stat s;
    fstat(infile, &s);

    if (fchmod(outfile, s.st_mode) == -1) {
        fprintf(stderr, "Something went wrong changing outfile permissions.\n");
        free(buffer2);
        return 1;
    }
    // Add 1 to hist[0] and hist[1] to prevent the tree from breaking when there
    // is only one unique character
    if (hist[0] == 0) {
        hist[0] += 1;
        unique_characters += 1;
    }
    if (hist[1] == 0) {
        hist[1] += 1;
        unique_characters += 1;
    }
    // Build the tree with the hist
    Node *tree = build_tree(hist);

    if (tree == NULL) {
        fprintf(stderr, "Something went wrong buiding the tree.\n");
        free(buffer2);
        return 1;
    }

    Code codes[ALPHABET] = {0};
    // Build the codes with the tree
    build_codes(tree, codes);

    // Set the appropriate header values
    Header h;
    h.magic = MAGIC;
    h.permissions = s.st_mode;
    h.tree_size = (unique_characters * 3) - 1;
    h.file_size = total;

    // Tell write bytes that the header absoultely definitely is an array of
    // uint8s, then write it all to the outfile
    write_bytes(outfile, (uint8_t *)&h, sizeof(Header));

    // Write the tree to the outfill
    dump_tree(outfile, tree);

    // For every character in the infile, write the corrisponding code to the
    // outfile
    for (uint64_t i = 0; i < total; i += 1) {
        write_code(outfile, &codes[buffer2[i]]);
    }
    // Flush whatever is left in the outfile
    flush_codes(outfile);
    // Print the stats if it was specified
    if (stats) {
        fprintf(stderr, "Uncompressed file size: %lu bytes\n", bytes_read);
        fprintf(stderr, "Compressed file size: %lu bytes\n", bytes_written);
        fprintf(stderr, "Space saving: %.2f%%\n",
                (1.0 - ((1.0 * bytes_written) / (1.0 * bytes_read))) * 100);
    }
    // Free everything allocated
    delete_tree(&tree);
    close(infile);
    close(outfile);
    free(buffer2);

    return 0;
}
