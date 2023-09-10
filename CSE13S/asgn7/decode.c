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

    // Process the passed in command line arguments

    bytes_read = 0;
    bytes_written = 0;
    int infile = STDIN_FILENO;
    int outfile = STDOUT_FILENO;
    bool stats = false;
    bool help = false;
    bool err = false;

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
                "SYNOPSIS\n  A Huffman decoder.\n  Decompresses a file using "
                "the Huffman coding algorithm.\n\nUSAGE\n  %s [-h] [-i infile] "
                "[-o outfile]\n\nOPTIONS\n  -h             Program usage and "
                "help.\n  -v             Print compression statistics.\n  -i "
                "infile      Input file to decompress.\n  -o outfile     "
                "Output of decompressed data.\n",
                argv[0]);
        if (help) {
            return 0;
        }
        if (err) {
            return 1;
        }
    }

    // Try to read in the header file from infile. Print an error message and
    // return 1 if a problem occurs

    Header h;
    if (!read_bytes(infile, (uint8_t *)&h, sizeof(Header))) {
        fprintf(stderr, "Error: unable to read header\n");
        return 1;
    }
    if (h.magic != MAGIC) {
        fprintf(stderr, "Invalid magic number\n");
        return 1;
    }

    if (fchmod(outfile, h.permissions) == -1) {
        fprintf(stderr, "Something went wrong changing file permissions.\n");
        return 1;
    }

    // Read the tree dump in from the infile

    uint8_t *tree_dump = calloc(h.tree_size, sizeof(uint8_t));

    if (read_bytes(infile, tree_dump, h.tree_size) != h.tree_size) {
        fprintf(stderr, "Could not read tree from file.\n");
        return 1;
    }

    // Rebuild the tree from the dump
    Node *root = rebuild_tree(h.tree_size, tree_dump);

    uint8_t buf[BLOCK] = {0};
    uint64_t counter = 0;
    Node *current = root;
    uint8_t bit = 0;

    // While we have not written all of the bytes in the passed in file
    while (counter != h.file_size) {
        // Read in the next bit
        if (!read_bit(infile, &bit)) {
            fprintf(stderr,
                    "Some of the compressed file seems to be missing.\n");
            return 1;
        }
        if (current == NULL) {
            fprintf(stderr,
                    "Something is wrong with the constuction of the tree\n");
            return 1;
            ;
        }
        // If the current node is a leaf, write the corrisponding symbol to the
        // outbuffer
        if (current->left == NULL && current->right == NULL) {
            buf[counter % BLOCK] = current->symbol;
            counter += 1;
            current = root;
            // If the buffer is full, write it to the outfile
            if (counter % BLOCK == 0) {
                if (!write_bytes(outfile, buf, BLOCK)) {
                    fprintf(stderr, "Can't wriite to the outfile\n");
                    return 1;
                }
            }
        }
        // If the read in bit is a 0, go left, otherwise, go right
        if (bit == 0) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    // If the buffer was not just written, write the remaining chars to the
    // outfile
    if (counter % BLOCK != 0) {
        if (!write_bytes(outfile, buf, counter % BLOCK)) {
            fprintf(stderr, "Something has gone wrong writing the result.\n");
            return 1;
        }
    }
    // Print the stats if asked
    if (stats) {
        fprintf(stderr, "Compressed file size: %lu bytes\n", bytes_read);
        fprintf(stderr, "Decompressed file size: %lu bytes\n", bytes_written);
        fprintf(stderr, "Space saving: %.2f%%\n",
                (1.0 - ((1.0 * bytes_read) / (1.0 * bytes_written))) * 100);
    }
    // Free everything
    delete_tree(&root);
    free(tree_dump);
    close(infile);
    close(outfile);
    return 0;
}
