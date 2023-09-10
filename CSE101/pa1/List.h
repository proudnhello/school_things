//Moore Macauley
//mmacule
//pa1

#ifndef LIST
#define LIST
#include <stdbool.h>
#include <stdio.h>

typedef struct ListObj* List;

// Contructors and destructors
List newList(void);
void freeList(List* pl);

// Access funtions - Returns the corrisponding value
int length(List L);
int index(List L); // of the cursor
int front(List L); // the first value
int back(List L); // the last value
int get(List L); // the cursor's value
bool equals(List A, List B); // if the two lists are equal

// Manipulation - changes the list
void clear(List L); // Removes all values in the list
void moveFront(List L); // Moves the cursor to the front
void moveBack(List L);  // Moves the cursor to the back
void movePrev(List L);  // Moves the cursor one element closer to the front
void moveNext(List L); // Moves the cursor one element closer to the back
void prepend(List L, int x); // Add x before the first element
void append(List L, int x); // Add x after the last element
void insertBefore(List L, int x); // Insert x before the cursor
void insertAfter(List L, int x); // Insert x after the cursor
void deleteFront(List L); // Delete the first calue
void deleteBack(List L); // Delete the last value
void delete(List L); // Delete the cursor's value, which then becomes undefined
void set(List L, int x); // Sets the value of the cursor

// Other
List copyList(List L); // Returns a copy of L, the cursor in the copy is undefined
void printList(FILE* out, List L); // Print the list, values seperated by a ' '
#endif