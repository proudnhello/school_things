#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

struct Stack {
    uint32_t top;
    uint32_t capacity;
    Node **items;
};

Stack *stack_create(uint32_t capacity) {
    // Allocate memory that needs to be allocated, return null if it fails
    Stack *s = (Stack *)malloc(sizeof(Stack));
    if (s != NULL) {
        s->top = 0;
        s->capacity = capacity;
        s->items = (Node **)calloc(capacity, sizeof(Node *));
        if (s->items == NULL) {
            free(s);
            return NULL;
        }
    }
    return s;
}

void stack_delete(Stack **s) {
    // Free the internal array and the stack. Don't free the nodes, as
    // tree_delete in huffman.c will do that
    free((*s)->items);
    free(*s);
    *s = NULL;
}

bool stack_empty(Stack *s) {
    // If the top is 0, then there are no items in the stack
    if (s->top == 0) {
        return true;
    }
    return false;
}

bool stack_full(Stack *s) {
    // If the top is at the capacity, then the stack is full
    if (s->top == s->capacity) {
        return true;
    }
    return false;
}

uint32_t stack_size(Stack *s) {
    // The top of the stack is the current size
    return s->top;
}

bool stack_push(Stack *s, Node *n) {
    if (stack_full(s)) {
        return false;
    }
    // if the stack isn't full, set the top to be the passed in item
    s->items[s->top] = n;
    s->top += 1;

    return true;
}

bool stack_pop(Stack *s, Node **n) {
    if (stack_empty(s)) {
        return false;
    }
    // If the stack isn't empty, get the top item
    s->top -= 1;
    *n = s->items[s->top];

    return true;
}

void stack_print(Stack *s) {
    // Iterate over all the nodes, and print them out.
    for (uint32_t i = 0; i < s->top; i += 1) {
        Node *n = s->items[i];
        node_print(n);
    }
}
