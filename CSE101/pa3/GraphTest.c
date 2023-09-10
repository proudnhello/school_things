//Moore Macauley
//mmacule
//pa3

#include "Graph.h"

int main(int argc, char const *argv[])
{
    Graph test = newGraph(10);

    printf("%d\n", getSize(test));
    printf("%d\n", getOrder(test));

    addEdge(test, 1, 3);
    addEdge(test, 1, 5);
    addEdge(test, 1, 4);
    addEdge(test, 1, 2);
    addEdge(test, 1, 2);
    addEdge(test, 2, 7);
    addEdge(test, 7, 10);
    addEdge(test, 3, 7);

    printf("%d\n", getSize(test));

    addEdge(test, 5, 4);
    addArc(test, 1, 2);
    addArc(test, 3, 4);
    
    printGraph(stdout, test);

    printf("%d\n", getSize(test));

    freeGraph(&test);

    test = newGraph(10);
    addArc(test, 1, 3);
    addArc(test, 1, 5);
    addArc(test, 1, 4);
    addArc(test, 1, 2);
    addArc(test, 1, 2);
    addArc(test, 2, 7);
    addArc(test, 7, 10);
    addArc(test, 3, 7);
    addArc(test, 10, 1);
    addArc(test, 6, 8);
    addArc(test, 8, 9);
    addArc(test, 9, 6);

    printGraph(stdout, test);
    printf("%d\n", getSize(test));
    printf("%d\n", getOrder(test));
    printf("---------------------------------------------------------\n");
    Graph copy = copyGraph(test);
    printGraph(stdout, copy);
    printf("%d\n", getSize(copy));
    printf("%d\n", getOrder(copy));
    printf("---------------------------------------------------------\n");
    Graph trans = transpose(copy);
    printGraph(stdout, trans);
    printf("%d\n", getSize(trans));
    printf("%d\n", getOrder(trans));

    List order = newList();

    for (size_t i = 1; i < 11; i++){
        append(order, i);
    }

    DFS(test, order);
    printf("---------------------------------------------------------\n");
    printList(stdout, order);
    printf("\n");
    clear(order);
    for(int i = 1; i < 11; i++){
        printf("%d parent - %d\n", i, getParent(test, i));
        printf("%d discover - %d\n", i, getDiscover(test, i));
        printf("%d finish - %d\n", i, getFinish(test, i));
        printf("\n");
        prepend(order, i);
    }

    DFS(test, order);
    printf("---------------------------------------------------------\n");
    printList(stdout, order);
    printf("\n");
    for(int i = 1; i < 11; i++){
        printf("%d parent - %d\n", i, getParent(test, i));
        printf("%d discover - %d\n", i, getDiscover(test, i));
        printf("%d finish - %d\n", i, getFinish(test, i));
        printf("\n");
    }

    freeList(&order);
    freeGraph(&test);
    freeGraph(&trans);
    freeGraph(&copy);
    return 0;
}
