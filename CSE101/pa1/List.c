//Moore Macauley
//mmacule
//pa1

#include "List.h"
#include<assert.h>
#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>

// Structs ----------------------------------------------------------------------------------------

typedef struct NodeObj* Node;

typedef struct NodeObj
{
    Node next;
    Node prev;
    int value;
}NodeObj;

typedef struct ListObj
{
    Node front;
    Node back;
    Node cursor;
    int cursorIndex;
    int length;
}ListObj;

// Constructors and Destructors  -------------------------------------------------------------------

// Creates a new Node
Node newNode(int i){
    Node new = malloc(sizeof(NodeObj));
    assert(new != NULL);
    new->value = i;
    new->next = NULL;
    new->prev = NULL;
    return new;
}

// Creates a new List
List newList(void){
    List new = malloc(sizeof(ListObj));
    assert(new != NULL);
    new->front = newNode(0);
    new->back = newNode(0);

    assert(new->front != NULL && new->back != NULL);
    new->front->next = new->back;
    new->front->prev = NULL;
    new->back->prev = new->front;
    new->back->next = NULL;

    new->cursor = NULL;
    new->cursorIndex = -1;
    new->length = 0;

    return new;
}

// Frees a node
void freeNode(Node* node){
    if(node == NULL || *node == NULL){
        return;
    }
    free(*node);
    node = NULL;
}

// Frees a List
void freeList(List* pl){ 
    if(pl == NULL || *pl == NULL){
        return;
    }
    List L = *pl;

    Node current = L->front->next;
    while(current->next != NULL){
        freeNode(&current->prev);
        current = current->next;
    }
    freeNode(&current->prev);
    freeNode(&current);
    free(L);
    *pl = NULL;

    return;
}

// Access Functions  -------------------------------------------------------------------

// Returns true if the List is empty
bool isEmpty(List L){
    if(L->length <= 0){
        return true;
    }
    return false;
}

// Returns true if the cursor is undefined
bool undefinedCursor(List L){
    if(index(L) == -1){
        return true;
    }
    return false;
}

// Returns the length
int length(List L){
    if(L == NULL){
        fprintf(stderr, "List Error: Calling length() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    return L->length;
}

// Returns the cursor's index if it is defined, -1 if it is not
int index(List L){
    if(L == NULL){
        fprintf(stderr, "List Error: Calling index() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if (L->cursorIndex < L->length && L->cursorIndex >= 0){
        return L->cursorIndex;
    }
    return -1;
}

// Returns the value of the first element in the list
int front(List L){
    if(L == NULL){
        fprintf(stderr, "List Error: Calling front() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if(isEmpty(L)){
        fprintf(stderr, "List Error: Calling front() on empty List\n");
        exit(EXIT_FAILURE);
    }
    return L->front->next->value;
}

// Returns the value of the last element
int back(List L){
    if(L == NULL){
        fprintf(stderr, "List Error: Calling back() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if(isEmpty(L)){
        fprintf(stderr, "List Error: Calling back() on empty List\n");
        exit(EXIT_FAILURE);
    }
    return L->back->prev->value;
}

// Returns the value of the cursor
int get(List L){
    if(L == NULL){
        fprintf(stderr, "List Error: Calling get() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if(isEmpty(L)){
        fprintf(stderr, "List Error: Calling get() on empty List\n");
        exit(EXIT_FAILURE);
    }
    if(undefinedCursor(L)){
        fprintf(stderr, "List Error: Calling get() on a List with undefined cursor\n");
        exit(EXIT_FAILURE);
    }
    return L->cursor->value;
}

// Returns true if A and B are equal, false otherwise
bool equals(List A, List B){ 
    if(A == NULL || B == NULL || A->length != B->length){
        return false;
    }
    Node aCur = A->front->next;
    Node bCur = B->front->next;
    while(aCur->next != NULL){
        if(bCur->next == NULL){
            return false;
        }
        if(aCur->value != bCur->value){
            return false;
        }
        aCur = aCur->next;
        bCur = bCur->next;
    }
    if(bCur->next == NULL){
        return true;
    }
    return false;
}

// Manipulation  -------------------------------------------------------------------

// Removes all elements from the list and undefines the cursor 
void clear(List L){
    if(L == NULL){
        fprintf(stderr, "List Error: Calling clear() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if(isEmpty(L)){
        return;
    }

    while (L->length > 0){
        deleteFront(L);
    }
    
    L->cursorIndex = -1;
}

// Sets the value of the cursor
void set(List L, int x){
    if(L == NULL){
        fprintf(stderr, "List Error: Calling get() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if(isEmpty(L)){
        fprintf(stderr, "List Error: Calling get() on empty List\n");
        exit(EXIT_FAILURE);
    }
    if(undefinedCursor(L)){
        fprintf(stderr, "List Error: Calling get() on a List with undefined cursor\n");
        exit(EXIT_FAILURE);
    }

    L->cursor->value = x;
}

// Moves the cursor to the front element
void moveFront(List L){
    if(L == NULL){
        fprintf(stderr, "List Error: Calling moveFront() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if (isEmpty(L)){
        return;
    }
    L->cursor = L->front->next;
    L->cursorIndex = 0;
}

// Moves the cursor to the back element
void moveBack(List L){
    if(L == NULL){
        fprintf(stderr, "List Error: Calling moveBack() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if (isEmpty(L)){
        return;
    }

    L->cursor = L->back->prev;
    L->cursorIndex = L->length - 1;
}

// Moves the cursor one element closer to the front
void movePrev(List L){
    if(L == NULL){
        fprintf(stderr, "List Error: Calling movePrev() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if(undefinedCursor(L)){
        return;
    }
    if(index(L) == 0){
        L->cursorIndex = -1;
        return;
    }

    L->cursor = L->cursor->prev;
    L->cursorIndex -= 1;
}

// Moves the cursor one element closer to the back
void moveNext(List L){
    if(L == NULL){
        fprintf(stderr, "List Error: Calling moveNext() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if(undefinedCursor(L)){
        return;
    }
    if(index(L) >= length(L) - 1){
        L->cursorIndex = -1;
        return;
    }

    L->cursor = L->cursor->next;
    L->cursorIndex += 1;
}

// Adds x before the first element
void prepend(List L, int x){
    if(L == NULL){
        fprintf(stderr, "List Error: Calling prepend() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    Node new = newNode(x);
    assert(new != NULL);

    if(!undefinedCursor(L)){
        L->cursorIndex += 1;
    }
    L->length += 1;

    new->prev = L->front;
    new->next = L->front->next;

    L->front->next->prev = new;
    L->front->next = new;
}

// Adds x after the last element
void append(List L, int x){
    if(L == NULL){
        fprintf(stderr, "List Error: Calling append() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    Node new = newNode(x);
    assert(new != NULL);

    L->length += 1;

    new->next = L->back;
    new->prev = L->back->prev;

    L->back->prev->next = new;
    L->back->prev = new;
}

// Adds x before the cursor
void insertBefore(List L, int x){
    if(L == NULL){
        fprintf(stderr, "List Error: Calling insertBefore() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if(isEmpty(L)){
        fprintf(stderr, "List Error: Calling insertBefore() on empty List\n");
        exit(EXIT_FAILURE);
    }
    if(undefinedCursor(L)){
        fprintf(stderr, "List Error: Calling insertBefore() on a List with undefined cursor\n");
        exit(EXIT_FAILURE);
    }
    Node new = newNode(x);
    assert(newNode != NULL);

    new->next = L->cursor;
    new->prev = L->cursor->prev;

    L->cursor->prev->next = new;
    L->cursor->prev = new;

    L->length += 1;
    L->cursorIndex += 1;
}

// Adds x after the cursor
void insertAfter(List L, int x){
    if(L == NULL){
        fprintf(stderr, "List Error: Calling insertBefore() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if(isEmpty(L)){
        fprintf(stderr, "List Error: Calling insertBefore() on empty List\n");
        exit(EXIT_FAILURE);
    }
    if(undefinedCursor(L)){
        fprintf(stderr, "List Error: Calling insertBefore() on a List with undefined cursor\n");
        exit(EXIT_FAILURE);
    }
    Node new = newNode(x);
    assert(newNode != NULL);

    new->prev = L->cursor;
    new->next = L->cursor->next;

    L->cursor->next->prev = new;
    L->cursor->next = new;

    L->length += 1;
}

// Deletes the first value
void deleteFront(List L){
    if(L == NULL){
        fprintf(stderr, "List Error: Calling deleteFront() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if(isEmpty(L)){
        fprintf(stderr, "List Error: Calling deleteFront() on empty List\n");
        exit(EXIT_FAILURE);
    }

    Node toBeDeleted = L->front->next;

    toBeDeleted->next->prev = L->front;
    L->front->next = toBeDeleted->next;

    if (L->cursor == toBeDeleted)
    {
        L->cursorIndex = -1;
    }else if (!undefinedCursor(L))
    {
        L->cursorIndex -= 1;
    }
    
    L->length -= 1;
    
    freeNode(&toBeDeleted);
}

//Deletes the last value
void deleteBack(List L){
    if(L == NULL){
        fprintf(stderr, "List Error: Calling deleteBack() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if(isEmpty(L)){
        fprintf(stderr, "List Error: Calling deleteBack() on empty List\n");
        exit(EXIT_FAILURE);
    }

    Node toBeDeleted = L->back->prev;

    toBeDeleted->prev->next = L->back;
    L->back->prev = toBeDeleted->prev;

    if (L->cursor == toBeDeleted)
    {
        L->cursorIndex = -1;
    }

    L->length -= 1;
    
    freeNode(&toBeDeleted);
}

// Deletes the cursor's value
void delete(List L){
    if(L == NULL){
        fprintf(stderr, "List Error: Calling delete() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if(isEmpty(L)){
        fprintf(stderr, "List Error: Calling delete() on empty List\n");
        exit(EXIT_FAILURE);
    }
    if(undefinedCursor(L)){
        fprintf(stderr, "List Error: Calling delete() on a List with undefined cursor\n");
        exit(EXIT_FAILURE);
    }

    Node toBeDeleted = L->cursor;

    toBeDeleted->prev->next = toBeDeleted->next;
    toBeDeleted->next->prev = toBeDeleted->prev;

    L->cursorIndex = -1;

    L->length -= 1;
    
    freeNode(&toBeDeleted);
}

// Other  -------------------------------------------------------------------

// Prints the list
void printList(FILE *out, List L){
    Node current = L->front->next;
    while(current->next != NULL){
        fprintf(out, "%d ", current->value);
        current = current->next;
    }

}

// Returns a copy of L
List copyList(List L){ 
    if(L == NULL){
        return NULL;
    }
    if(L->length == 0){
        return newList();
    }
    List copy = newList();
    Node current = L->front->next;
    while(current->next->next != NULL){
        append(copy, current->value);
        current = current->next;
    }
    append(copy, current->value);
    return copy;
}
