//Moore Macauley
//mmacule
//pa1

#include "List.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[])
{

    List test = newList();
    freeList(&test);

    // Testing access functions that work on an empty list
    List list = newList();
    printf("%d\n", length(list));
    printf("%d\n", index(list));

    printList(stdout, list);
    printf("\n");

    // Filling up a list with append
    for (size_t i = 0; i < 10; i++)
    {
        append(list, i);
    }

    printList(stdout, list);
    printf("\n");

    // Testing acesses functions on a list with elements
    printf("%d\n", length(list));
    printf("%d\n", index(list));
    printf("%d\n", front(list));
    printf("%d\n", back(list));
    moveBack(list);
    printf("%d\n", index(list));
    printf("%d\n", get(list));
    moveFront(list);
    printf("%d\n", index(list));
    printf("%d\n", get(list));
    printf("----------------------------------------------------------------\n\n");

    // Adding more elements with prepend
    for (size_t i = 0; i < 10; i++)
    {
        prepend(list, i);
    }

    // Testing cursor movement
    printList(stdout, list);
    printf("\n");
    printf("%d\n", index(list));
    printf("%d\n", get(list));
    movePrev(list);
    movePrev(list);
    printf("%d\n", index(list));
    printf("%d\n", get(list));
    moveNext(list);
    moveNext(list);
    moveNext(list);
    moveNext(list);
    printf("%d\n", index(list));
    printf("%d\n", get(list));

    set(list, 10000000);
    printf("%d\n", index(list));
    printf("%d\n", get(list));
    printList(stdout, list);
    printf("\n");

    printf("----------------------------------------------------------------\n\n");

    // Testing insert before and after
    insertBefore(list, -42);
    insertBefore(list, -28);
    insertAfter(list, -1);
    insertAfter(list, -2);

    printList(stdout, list);
    printf("\n");
    printf("%d\n", index(list));
    printf("%d\n", get(list));
    printf("%d\n", length(list));

    printf("----------------------------------------------------------------\n\n");

    // Testing cursorless removal functions
    deleteBack(list);
    deleteBack(list);
    deleteBack(list);

    deleteFront(list);
    deleteFront(list);
    deleteFront(list);

    printList(stdout, list);
    printf("\n");

    printf("%d\n", index(list));
    printf("%d\n", get(list));
    printf("%d\n", length(list));

    printf("----------------------------------------------------------------\n\n");

    // Testing removal functions with the cursor
    delete(list);

    printList(stdout, list);
    printf("\n");

    printf("%d\n", index(list));

    moveFront(list);
    deleteFront(list);

    printf("%d\n", index(list));

    moveBack(list);
    deleteBack(list);
    printf("%d\n", index(list));

    printList(stdout, list);
    printf("\n");

    printf("----------------------------------------------------------------\n\n");

    // Testing copy and equals
    List copy = copyList(list);
    printList(stdout, copy);
    printf("\n");

    printf("%d\n", equals(list, copy));

    deleteBack(copy);

    printf("%d\n", equals(list, copy));

    clear(list);

    printf("----------------------------------------------------------------\n\n");

    printList(stdout, list);
    printf("\n");

    append(list, 7);

    printList(stdout, list);
    printf("\n");

    printf("----------------------------------------------------------------\n\n");

    // Testing clearing small lists
    List segFaulter = newList();

    append(segFaulter, 1);
    append(segFaulter, 1);
    append(segFaulter, 1);
    clear(segFaulter);

    printf("Cleared list below\n");
    printList(stdout, segFaulter);
    printf("\n");

    append(segFaulter, 1);
    append(segFaulter, 1);
    clear(segFaulter);

    printf("Cleared list below\n");
    printList(stdout, segFaulter);
    printf("\n");

    append(segFaulter, 1);
    clear(segFaulter);

    printf("Cleared list below\n");
    printList(stdout, segFaulter);
    printf("\n");

    printf("----------------------------------------------------------------\n\n");

    //ListClient.c begins here
    List A = newList();
    List B = newList();
    List C = NULL;
    int i;

    for(i=1; i<=20; i++){
        append(A,i);
        prepend(B,i);
    }

    printList(stdout,A); 
    printf("\n");
    printList(stdout,B); 
    printf("\n");

    for(moveFront(A); index(A)>=0; moveNext(A)){
        printf("%d ", get(A));
    }
    printf("\n");
    for(moveBack(B); index(B)>=0; movePrev(B)){
        printf("%d ", get(B));
    }
    printf("\n");

    C = copyList(A);
    printf("%s\n", equals(A,B)?"true":"false");
    printf("%s\n", equals(B,C)?"true":"false");
    printf("%s\n", equals(C,A)?"true":"false");


    moveFront(A);
    for(i=0; i<5; i++) moveNext(A); // at index 5
    insertBefore(A, -1);            // at index 6
    for(i=0; i<9; i++) moveNext(A); // at index 15
    insertAfter(A, -2);
    for(i=0; i<5; i++) movePrev(A); // at index 10
    delete(A);
    printList(stdout,A);
    printf("\n");
    printf("%d\n", length(A));
    clear(A);
    printf("%d\n", length(A));

    freeList(&A);
    freeList(&B);
    freeList(&C);

    //ListClient.c ends here

    freeList(&list);
    freeList(&copy);
    freeList(&segFaulter);

    return 0;
}
