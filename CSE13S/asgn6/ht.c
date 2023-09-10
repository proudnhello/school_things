#include "ht.h"
#include "city.h"
#include "stdio.h"

struct HashTable {
    uint64_t salt;
    uint32_t size;
    uint32_t n_keys;
    uint32_t n_hits;
    uint32_t n_misses;
    uint32_t n_examined;
    bool mtf;
    LinkedList **lists;
};

HashTable *ht_create(uint32_t size, bool mtf) {
    HashTable *ht = (HashTable *)malloc(sizeof(HashTable));
    if (ht != NULL) {
        ht->mtf = mtf;
        ht->salt = 0x9846e4f157fe8840;
        ht->n_hits = ht->n_misses = ht->n_examined = 0;
        ht->n_keys = 0;
        ht->size = size;
        ht->lists = (LinkedList **)calloc(size, sizeof(LinkedList *));
        if (!ht->lists) {
            free(ht);
            ht = NULL;
        }
    }
    return ht;
}

void ht_delete(HashTable **ht) {
    HashTable *table = *ht;

    for (uint32_t i = 0; i < table->size; i += 1) {
        ll_delete(&(table->lists[i]));
        table->lists[i] = NULL;
    }

    free(table->lists);
    free(table);
    *ht = NULL;

    return;
}

uint32_t ht_size(HashTable *ht) { return ht->size; }

Node *ht_lookup(HashTable *ht, char *oldspeak) {
    // Find the hash of oldspeak
    uint32_t index = hash((ht->salt), oldspeak) % (ht->size);

    // If the ll there dne, return null
    if (ht->lists[index] == NULL) {
        ht->n_misses += 1;
        return NULL;
    }

    uint32_t initial, final, temp;
    ll_stats(&temp, &initial);

    // If get the node from the ll
    Node *node = ll_lookup(ht->lists[index], oldspeak);

    ll_stats(&temp, &final);
    ht->n_examined += (final - initial);

    // If the node exists, then it was in the ll. Otherwise, it does not
    if (node == NULL) {
        ht->n_misses += 1;
    } else {
        ht->n_hits += 1;
    }

    return node;
}

void ht_insert(HashTable *ht, char *oldspeak, char *newspeak) {
    // Find the index of oldspeak
    uint32_t index = hash((ht->salt), oldspeak) % (ht->size);

    // If the list at index dne, create it
    if (ht->lists[index] == NULL) {
        ht->lists[index] = ll_create(ht->mtf);
    }
    if (ht->lists[index] == NULL) {
        return;
    }

    Node *node = ll_lookup(ht->lists[index], oldspeak);

    if (node != NULL)
    {
        return;
    }
    ht->n_keys += 1;

    // Insert oldspeak to the ll at index
    ll_insert(ht->lists[index], oldspeak, newspeak);

    return;
}

uint32_t ht_count(HashTable *ht) {
    // Iterate over all the lls, counting each one that isn't null
    uint32_t count = 0;
    for (uint32_t i = 0; i < ht->size; i += 1) {
        if (ht->lists[i] != NULL) {
            count += 1;
        }
    }
    return count;
}

void ht_print(HashTable *ht) {
    for (uint32_t i = 0; i < ht->size; i += 1) {
        ll_print(ht->lists[i]);
    }
    return;
}

void ht_stats(HashTable *ht, uint32_t *nk, uint32_t *nh, uint32_t *nm,
              uint32_t *ne) {
    *nk = ht->n_keys;
    *nh = ht->n_hits;
    *nm = ht->n_misses;
    *ne = ht->n_examined;
    return;
}
