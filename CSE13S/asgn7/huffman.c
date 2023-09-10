#include "huffman.h"
#include "defines.h"
#include "io.h"
#include "pq.h"
#include "stack.h"
#include <stdint.h>
#include <stdio.h>

Node *build_tree(uint64_t hist[static ALPHABET]) {
    PriorityQueue *q = pq_create(ALPHABET);
    // Make a queue, and create a node for every letter in the histogram
    for (int i = 0; i < ALPHABET; i += 1) {
        if (hist[i] != 0) {
            enqueue(q, node_create(i, hist[i]));
        }
    }

    Node *left;
    Node *right;

    // pop the queue and join the resulting two nodes until there is only one
    // left
    while (pq_size(q) > 1) {
        dequeue(q, &left);
        dequeue(q, &right);
        enqueue(q, node_join(left, right));
    }

    // pop the last node, which will be the root of the tree, and return it
    dequeue(q, &left);

    pq_delete(&q);

    return left;
}

static Code current_code;

static void code_traversal(Node *current, Code table[]) {
    if (current == NULL) {
        return;
    }
    uint8_t temp = 0;
    // If the node is a leaf, then the current code is correct, so set the
    // corrisponding code in table to the current code
    if (current->left == NULL && current->right == NULL) {
        table[current->symbol] = current_code;
        return;
    }
    // Push a 0 to the code for going left, then call the function on the left
    // subtree
    code_push_bit(&current_code, 0);
    code_traversal(current->left, table);
    // Remove the 0
    code_pop_bit(&current_code, &temp);
    // Push a 1 to the code for going right, then call the function on the right
    // subtree
    code_push_bit(&current_code, 1);
    code_traversal(current->right, table);
    // Remove the 1
    code_pop_bit(&current_code, &temp);
}

void build_codes(Node *root, Code table[static ALPHABET]) {
    current_code = code_init();
    // I only want to innit the code once, so we do that here, then call a
    // different static funtion
    code_traversal(root, table);
}

void dump_tree(int outfile, Node *root) {
    if (root == NULL) {
        return;
    }
    uint8_t L = 'L';
    uint8_t I = 'I';
    // If the node is a leaf, write an L and the current symbol to the outfile
    if (root->left == NULL && root->right == NULL) {
        write_bytes(outfile, &L, 1);
        write_bytes(outfile, &(root->symbol), 1);
        return;
    }
    // If it's a interior node, dump the left subtree, the right subtree, then
    // write an I to the outfile
    dump_tree(outfile, root->left);
    dump_tree(outfile, root->right);
    write_bytes(outfile, &I, 1);
}

Node *rebuild_tree(uint16_t nbytes, uint8_t tree[static nbytes]) {
    Stack *stack = stack_create(MAX_TREE_SIZE);
    // Iterate over all the passed in bytes
    for (int i = 0; i < nbytes; i += 1) {
        // If the next character is an L, add a node with the next char to a
        // stack
        if (tree[i] == 'L') {
            stack_push(stack, node_create(tree[i + 1], 0));
            i += 1;
        } else if (tree[i] == 'I') {
            // If it's an I, pop the next two nodes, join them, then add the
            // resulting parent to stack
            Node *r;
            Node *l;
            stack_pop(stack, &r);
            stack_pop(stack, &l);
            stack_push(stack, node_join(l, r));
        }
    }
    // Get the last remaining node in the stack, and return the rebuilt tree
    Node *retunable;
    stack_pop(stack, &retunable);
    stack_delete(&stack);
    return retunable;
}

void delete_tree(Node **root) {
    if (*root == NULL) {
        return;
    }

    delete_tree(&(*root)->left);
    delete_tree(&(*root)->right);

    node_delete(root);
}
