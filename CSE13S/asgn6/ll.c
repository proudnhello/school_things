#include "ll.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

uint64_t seeks;
uint64_t links;

struct LinkedList {
    uint32_t length;
    Node *head; // Head sentinel node.
    Node *tail; // Tail sentinel node.
    bool mtf;
};

static bool str_cmp(char *a, char *b) {

    uint32_t count = 0;
    char cha_a = a[count];
    char cha_b = b[count];

    while (cha_a != '\0' && cha_b != '\0') {

        if (cha_a != cha_b) {
            return false;
        }
        count += 1;
        cha_a = a[count];
        cha_b = b[count];
    }
    if (cha_a == '\0' && cha_b == '\0') {
        return true;
    } else {
        return false;
    }
}

static void move_to_front(LinkedList *ll, Node *node) {
    Node *n = node->next;
    Node *p = node->prev;

    // Make node's next and previous point to eachother, not node
    n->prev = p;
    p->next = n;

    // Make node point to head and head's next
    Node *head = ll->head;
    node->next = head->next;
    node->prev = head;

    // Make head's next point to node, and head point to node
    head->next->prev = node;
    head->next = node;
}

LinkedList *ll_create(bool mtf) {
    LinkedList *ll = (LinkedList *)malloc(sizeof(LinkedList));

    if (ll != NULL) {
        ll->head = node_create(NULL, NULL);
        if (ll->head == NULL) {
            free(ll);
            return NULL;
        }
        ll->tail = node_create(NULL, NULL);
        if (ll->tail == NULL) {
            free(ll->head);
            free(ll);
            return NULL;
        }
        ll->head->next = ll->tail;
        ll->tail->prev = ll->head;
        ll->length = 0;
        ll->mtf = mtf;
    }

    return ll;
}

void ll_delete(LinkedList **ll) {

    if (*ll == NULL) {
        return;
    }

    LinkedList *list = *ll;

    Node *node = list->head;
    Node *next = list->head->next;

    while (next != NULL) {
        // Iterate over all the nodes, freeing them
        node_delete(&node);
        node = next;
        next = node->next;
    }
    // One last free node to free the tail
    free(node);
    free(list);
    *ll = NULL;

    return;
}

uint32_t ll_length(LinkedList *ll) {
    if (ll != NULL) {
        return ll->length;
    }
    return 0;
}

Node *ll_lookup(LinkedList *ll, char *oldspeak) {
    seeks += 1;

    Node *node = ll->head;

    // If this is the case, the list contains only sentinal nodes, and as such
    // cannot contain the oldspeak value
    if (ll->length == 0) {
        return NULL;
    }
    do {
        links += 1;
        node = node->next;
        // If one node's oldspeak == the passed in oldspeak, we return that node
        if (str_cmp(node->oldspeak, oldspeak)) {
            if (ll->mtf) {
                move_to_front(ll, node);
            }
            return node;
        }
    } while (node->next->oldspeak != NULL);
    // If we get through all the nodes w/o finding a node, return NULL
    return NULL;
}

void ll_insert(LinkedList *ll, char *oldspeak, char *newspeak) {
    // If the node already exists in the ll, don't do anything
    if (ll_lookup(ll, oldspeak) != NULL) {
        return;
    }

    Node *node = node_create(oldspeak, newspeak);

    if (node != NULL) {
        ll->length += 1;
        // Set the new node's next to head's next
        node->next = ll->head->next;
        // Set the new node's previous to head
        node->prev = ll->head;

        // Set the node after head's prev to be the new node
        ll->head->next->prev = node;
        // Set head's next to be node
        ll->head->next = node;
    }
    return;
}

void ll_print(LinkedList *ll) {
    if (ll == NULL) {
        return;
    }

    Node *node = ll->head->next;
    // Iterate over all the nodes, printing as we go
    while (node->next != NULL) {
        node_print(node);
        node = node->next;
    }
    return;
}

void ll_stats(uint32_t *n_seeks, uint32_t *n_links) {
    *n_seeks = seeks;
    *n_links = links;
    return;
}
