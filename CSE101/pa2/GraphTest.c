//Moore Macauley
//mmacule
//pa2

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

    BFS(test, 1);

    printf("%d\n", getSource(test));
    printf("2: %d\n", getParent(test, 2));
    printf("7: %d\n", getParent(test, 7));
    printf("10: %d\n", getParent(test, 10));
    printf("9: %d\n", getParent(test, 9));

    printf("9: %d\n", getDist(test, 9));
    printf("7: %d\n", getDist(test, 7));
    printf("10: %d\n", getDist(test, 10));

    freeGraph(&test);
    return 0;
}
