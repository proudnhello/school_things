#include "pq.h"
#include <stdio.h>
#include <stdlib.h>

struct PriorityQueue {
    Node **nodes;
    uint32_t capacity;
    uint32_t current_element;
};
// Queue is implemented with a min heap
static uint32_t left_child(uint32_t n) { return ((2 * n) + 1); }

static uint32_t right_child(uint32_t n) { return ((2 * n) + 2); }

static uint32_t parent(uint32_t n) { return (n - 1) / 2; }

PriorityQueue *pq_create(uint32_t capacity) {
    PriorityQueue *q = (PriorityQueue *)malloc(sizeof(PriorityQueue));
    if (q != NULL) {
        q->current_element = 0;
        q->capacity = capacity;
        q->nodes = (Node **)calloc(capacity, sizeof(Node *));
        if (q->nodes == NULL) {
            free(q);
            return NULL;
        }
    }
    return q;
}

void pq_delete(PriorityQueue **q) {
    // We don't delete the nodes in the queue, as that will be done in
    free((*q)->nodes);
    free(*q);
    *q = NULL;
    return;
}

bool pq_empty(PriorityQueue *q) {
    // If the current element is 0, then there are no elements
    if (q->current_element == 0) {
        return true;
    }
    return false;
}

bool pq_full(PriorityQueue *q) {
    // If the current element is the capacity, then the queue is full
    if (q->current_element == q->capacity) {
        return true;
    }
    return false;
}

uint32_t pq_size(PriorityQueue *q) {
    // The current element is the size
    return q->current_element;
}

bool enqueue(PriorityQueue *q, Node *n) {
    if (pq_full(q)) {
        return false;
    }
    // Add the item to the bottom of the queue
    uint32_t pos = q->current_element;
    q->current_element += 1;
    q->nodes[pos] = n;
    Node *temp;
    // Swap the node and its parent until the node has a larger frequency than
    // its parent, and as such is in the right place
    while (pos != 0 && node_cmp(q->nodes[parent(pos)], q->nodes[pos])) {
        temp = q->nodes[pos];
        q->nodes[pos] = q->nodes[parent(pos)];
        q->nodes[parent(pos)] = temp;
        pos = parent(pos);
    }
    return true;
}

bool dequeue(PriorityQueue *q, Node **n) {
    if (pq_empty(q)) {
        return false;
    }

    uint32_t pos = 0;
    uint32_t smaller = 0;

    *n = q->nodes[0];

    // Return the top, move a bottom node to the top
    q->nodes[0] = q->nodes[q->current_element - 1];
    q->current_element -= 1;

    // Swap the bottom node down the heap until it is larger than its smaller
    // child
    while (left_child(pos) < q->current_element) {

        if (right_child(pos) == q->current_element) {
            smaller = left_child(pos);
        } else {
            if (node_cmp(q->nodes[right_child(pos)],
                         q->nodes[left_child(pos)])) {
                smaller = left_child(pos);
            } else {
                smaller = right_child(pos);
            }
        }
        if (node_cmp(q->nodes[smaller], q->nodes[pos])) {
            break;
        }
        Node *temp = q->nodes[pos];
        q->nodes[pos] = q->nodes[smaller];
        q->nodes[smaller] = temp;
        pos = smaller;
    }
    return true;
}

void pq_print(PriorityQueue *q) {
    // Create a duplicate of the queue
    PriorityQueue *temp = pq_create(q->capacity);
    if (temp == NULL) {
        return;
    }

    temp->current_element = q->current_element;
    for (uint32_t i = 0; i < temp->current_element; i += 1) {
        temp->nodes[i] = q->nodes[i];
    }
    // Repeatedly pop the queue and print the node until there's nothing in the
    // queue
    Node *printable;
    while (dequeue(temp, &printable)) {
        node_print(printable);
    }
    free(temp);
    return;
}
