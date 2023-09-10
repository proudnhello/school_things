#include "node.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static uint32_t str_len(char *str) {
    uint32_t count = 0;
    char cha = str[count];

    while (cha != '\0') {
        count += 1;
        cha = str[count];
    }

    return count;
}

static void str_copy(char *org, char *new) {
    uint32_t len = str_len(org);
    for (uint32_t i = 0; i < len + 1; ++i) {
        new[i] = org[i];
    }
}

Node *node_create(char *oldspeak, char *newspeak) {
    Node *node = (Node *)malloc(sizeof(Node));
    // If the node was created
    if (node != NULL) {
        node->next = NULL;
        node->prev = NULL;
        char *old_dupe = NULL;
        char *new_dupe = NULL;
        // If there is an old speak value
        if (oldspeak != NULL) {
            // Copy the oldspeak value into old_dupe
            old_dupe = calloc(str_len(oldspeak) + 1, sizeof(char));
            // If this fails, free node, and set it to null
            if (old_dupe != NULL) {
                str_copy(oldspeak, old_dupe);
            } else {
                free(node);
                return NULL;
            }
        }
        // If there is a newspeak value
        if (newspeak != NULL) {
            // Copy it into new_dupe
            new_dupe = calloc(str_len(newspeak) + 1, sizeof(char));
            // If this fails, free old_dupe it if it exists, and node, set both
            // to null
            if (new_dupe != NULL) {
                str_copy(newspeak, new_dupe);
            } else {
                free(node);
                if (old_dupe != NULL) {
                    free(old_dupe);
                    old_dupe = NULL;
                }
                return NULL;
            }
        }
        // Set internal node values to old dupe and new dupe
        node->oldspeak = old_dupe;
        node->newspeak = new_dupe;
    }

    return node;
}

void node_delete(Node **n) {
    Node *node = *n;

    // If it has newspeak, free that
    if (node->newspeak != NULL) {
        free(node->newspeak);
        node->newspeak = NULL;
    }
    // If it has oldspeak, free that
    if (node->oldspeak != NULL) {
        free(node->oldspeak);
        node->oldspeak = NULL;
    }
    // If the node has a next, but no prev, set the next node's prev to null
    // Makes NULL<-node<->next to NULL<-next
    if (node->next != NULL && node->prev == NULL) {
        node->next->prev = NULL;
    }
    // If the node has a prev, but no next, set the next node's prev to null
    // Makes prev<->node->NULL into prev->NULL
    if (node->prev != NULL && node->next == NULL) {
        node->prev->next = NULL;
    }
    // If the node has both a prev and a next, set the node's next's prev to
    // node's prev, and node's prev's next to node's next If we have
    // prev<->node<->next this should result in prev<->next
    if (node->prev != NULL && node->next != NULL) {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }
    free(node);
    *n = NULL;

    return;
}

void node_print(Node *n) {
    if (n == NULL) {
        return;
    }

    if (n->newspeak != NULL && n->oldspeak != NULL) {
        printf("%s -> %s\n", n->oldspeak, n->newspeak);
    } else if (n->oldspeak != NULL) {
        printf("%s\n", n->oldspeak);
    }
    return;
}
