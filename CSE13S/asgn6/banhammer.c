#include "bf.h"
#include "ht.h"
#include "ll.h"
#include "messages.h"
#include "parser.h"
#include <error.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define OPTIONS "ht:f:ms"

static void help_message(char *name) {
    fprintf(
        stderr,
        "Usage: %s [options]\n  %s will read in words from stdin, identify any "
        "badspeak or old speak and output an\n  appropriate punishment "
        "message. "
        "The badspeak and oldspeak (with the newspeak translation)\n  that "
        "caused the punishment will be printed after the message. If "
        "statistics "
        "are enabled\n  punishment messages are suppressed and only statistics "
        "will be printed.\n    -t <ht_size>: Hash table size set to <ht_size>. "
        "(default: 10000)\n    -f <bf_size>: Bloom filter size set to "
        "<bf_size>. "
        "(default 2^19)\n    -s          : Enables the printing of "
        "statistics.\n "
        "   -m          : Enables move-to-front rule.\n    -h          : "
        "Display "
        "program synopsis and usage.\n",
        name, name);
}

int main(int argc, char **argv) {
    bool mtf = false;
    uint32_t bf_length = 524288;
    uint32_t ht_size = 10000;
    bool help = false;
    bool err = false;
    bool stats = false;
    int opt = 0;
    char *endptr;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case ('h'):
            help = true;
            break;
        case ('t'):
            ht_size = strtol(optarg, &endptr, 10);
            if (strtol(optarg, &endptr, 10) < 1) {
                error(0, 0, "Invalid hash table size.");
                return 1;
            }
            break;
        case ('f'):
            bf_length = strtol(optarg, &endptr, 10);
            if (strtol(optarg, &endptr, 10) < 1) {
                error(0, 0, "Invalid bloom filter table size.");
                return 1;
            }
            break;
        case ('m'):
            mtf = true;
            break;
        case ('s'):
            stats = true;
            break;
        default:
            err = true;
            break;
        }
    }

    if (help) {
        help_message(argv[0]);
        return 0;
    }
    if (err) {
        help_message(argv[0]);
        return 1;
    }

    // Create all the stack variables used, freeing the ones already created if
    // something goes wrong
    HashTable *ht = ht_create(ht_size, mtf);
    if (ht == NULL) {
        error(0, 0, "Something went wrong, most likely a lack of memory.");
        return 1;
    }

    BloomFilter *bf = bf_create(bf_length);
    if (bf == NULL) {
        ht_delete(&ht);
        error(0, 0, "Something went wrong, most likely a lack of memory.");
        return 1;
    }

    FILE *b_file = fopen("badspeak.txt", "r");
    if (b_file == NULL) {
        ht_delete(&ht);
        bf_delete(&bf);
        fprintf(
            stderr,
            "%s: Couldn't open badspeak.txt to read bad speak.\n: No such file "
            "or directory\n",
            argv[0]);
        return 1;
    }

    FILE *n_file = fopen("newspeak.txt", "r");
    if (n_file == NULL) {
        ht_delete(&ht);
        bf_delete(&bf);
        fclose(b_file);
        fprintf(
            stderr,
            "%s: Couldn't open newspeak.txt to read old speak and new speak "
            "translation.\n: No such file or directory\n",
            argv[0]);
        return 1;
    }

    Parser *b_p = parser_create(b_file);
    if (b_p == NULL) {
        ht_delete(&ht);
        bf_delete(&bf);
        fclose(b_file);
        fclose(n_file);
        error(0, 0, "Something went wrong, most likely a lack of memory.");
        return 1;
    }

    Parser *n_p = parser_create(n_file);
    if (n_p == NULL) {
        ht_delete(&ht);
        bf_delete(&bf);
        fclose(b_file);
        fclose(n_file);
        parser_delete(&b_p);
        error(0, 0, "Something went wrong, most likely a lack of memory.");
        return 1;
    }

    HashTable *badspeak = ht_create(ht_size, true);
    if (badspeak == NULL) {
        ht_delete(&ht);
        bf_delete(&bf);
        fclose(b_file);
        fclose(n_file);
        parser_delete(&b_p);
        parser_delete(&n_p);
        error(0, 0, "Something went wrong, most likely a lack of memory.");
        return 1;
    }

    HashTable *newspeak = ht_create(ht_size, true);
    if (newspeak == NULL) {
        ht_delete(&ht);
        bf_delete(&bf);
        fclose(b_file);
        fclose(n_file);
        parser_delete(&b_p);
        parser_delete(&n_p);
        ht_delete(&badspeak);
        error(0, 0, "Something went wrong, most likely a lack of memory.");
        return 1;
    }

    Parser *input = parser_create(stdin);
    if (input == NULL) {
        ht_delete(&ht);
        bf_delete(&bf);
        fclose(b_file);
        fclose(n_file);
        parser_delete(&b_p);
        parser_delete(&n_p);
        ht_delete(&badspeak);
        ht_delete(&newspeak);
        error(0, 0, "Something went wrong, most likely a lack of memory.");
        return 1;
    }

    char buffer[MAX_PARSER_LINE_LENGTH + 1];
    char buffer2[MAX_PARSER_LINE_LENGTH + 1];

    // Add all the badspeak words to the ht and bf
    while (next_word(b_p, buffer)) {
        ht_insert(ht, buffer, NULL);
        bf_insert(bf, buffer);
    }

    // Add all the old/newspeak pairs to the ht and bf
    while (next_word(n_p, buffer) && next_word(n_p, buffer2)) {
        ht_insert(ht, buffer, buffer2);
        bf_insert(bf, buffer);
    }

    bool bad_used = false;
    bool new_used = false;

    // Get the next inputed word
    while (next_word(input, buffer)) {
        // If it's in bf
        if (bf_probe(bf, buffer)) {
            Node *node = ht_lookup(ht, buffer);
            // And in ht
            if (node != NULL) {
                // add it to the list of used old/badspeak
                if (node->newspeak != NULL) {
                    new_used = true;
                    ht_insert(newspeak, node->oldspeak, node->newspeak);
                } else {
                    bad_used = true;
                    ht_insert(badspeak, node->oldspeak, node->newspeak);
                }
            }
        }
    }

    // if stats was specified, print out stats
    if (stats) {
        uint32_t ht_keys;
        uint32_t ht_hits;
        uint32_t ht_misses;
        uint32_t ht_examined;
        ht_stats(ht, &ht_keys, &ht_hits, &ht_misses, &ht_examined);
        printf("ht keys: %u\n", ht_keys);
        printf("ht hits: %u\n", ht_hits);
        printf("ht misses: %u\n", ht_misses);
        printf("ht probes: %u\n", ht_examined);
        uint32_t bf_keys;
        uint32_t bf_hits;
        uint32_t bf_misses;
        uint32_t bf_examined;
        bf_stats(bf, &bf_keys, &bf_hits, &bf_misses, &bf_examined);
        printf("bf keys: %u\n", bf_keys);
        printf("bf hits: %u\n", bf_hits);
        printf("bf misses: %u\n", bf_misses);
        printf("bf bits examined: %u\n", bf_examined);
        double print_value;
        if (bf_misses == 0) {
            printf("Bits examined per miss: %.6f\n", 0.0);
        } else {
            print_value = (bf_examined - (5 * bf_hits)) / (bf_misses * 1.0);
            printf("Bits examined per miss: %.6f\n",
                   print_value);
        }
        if (bf_hits == 0) {
            printf("False positives: %.6f\n", 0.0);
        } else {
            print_value = (ht_misses * 1.0 / bf_hits * 1.0);
            printf("False positives: %.6f\n",
                   print_value);
        }
        if (ht_hits == 0 && ht_misses == 0) {
            printf("Average seek length: %.6f\n", 0.0);
        } else {
            print_value = (ht_examined * 1.0) / ((ht_hits * 1.0) + (ht_misses * 1.0));
            printf(
                "Average seek length: %.6f\n",
                print_value);
        }
        if (bf_size(bf) == 0) {
            printf("Bloom filter load: %.6f\n", 0.0);

        } else {
            print_value = (bf_count(bf) * 1.0 / bf_size(bf) * 1.0);
            printf("Bloom filter load: %.6f\n",
                   print_value);
        }
    } else { // Otherwise print out the standard message and bad/oldspeak used
        if (new_used && bad_used) {
            printf("%s", mixspeak_message);
        } else if (new_used) {
            printf("%s", goodspeak_message);
        } else if (bad_used) {
            printf("%s", badspeak_message);
        }
        ht_print(badspeak);
        ht_print(newspeak);
    }

    ht_delete(&ht);
    bf_delete(&bf);
    fclose(b_file);
    fclose(n_file);
    parser_delete(&b_p);
    parser_delete(&n_p);
    ht_delete(&badspeak);
    ht_delete(&newspeak);
    parser_delete(&input);

    return 0;
}
