#include "node.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

Node *node_create(uint8_t symbol, uint64_t frequency) {
    Node *node = (Node *)malloc(sizeof(Node));
    if (node != NULL) {
        node->left = NULL;
        node->right = NULL;
        node->symbol = symbol;
        node->frequency = frequency;
    }
    return node;
}

void node_delete(Node **n) {
    free(*n);
    *n = NULL;
}

Node *node_join(Node *left, Node *right) {
    uint64_t frequency = left->frequency + right->frequency;
    Node *parent = node_create('$', frequency);
    parent->left = left;
    parent->right = right;

    return parent;
}

void node_print(Node *n) {

    if (isprint(n->symbol)) {
        printf("%c %lu\n", n->symbol, n->frequency);
    } else {
        printf("0x%02x %lu\n", n->symbol, n->frequency);
    }
}

bool node_cmp(Node *n, Node *m) { return n->frequency > m->frequency; }

void node_print_sym(Node *n) {
    if (isprint(n->symbol)) {
        printf("%c\n", n->symbol);
    } else {
        printf("0x%02x \n", n->symbol);
    }
}
