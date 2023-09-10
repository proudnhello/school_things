#include "Graph.h"
#include "List.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MAX_LENGTH 300

int main(int argc, char const *argv[])
{
    // This portion was taken from pa2
    FILE *in, *out;
    char line[MAX_LENGTH];

    // Ensure the correct values were passed in
    if(argc != 3){
        fprintf(stderr, "Usage: %s <input file> <output file>\n", argv[0]);
        exit(1);
    }

    in = fopen(argv[1], "r");
        if( in==NULL ){
        fprintf(stderr,"Unable to open file %s for reading\n", argv[1]);
        exit(1);
    }

    out = fopen(argv[2], "w");
    if( out==NULL ){
        fprintf(stderr,"Unable to open file %s for writing\n", argv[2]);
        exit(1);
    }
    int x = 1;
    int *input1 = &x;
    int y = 1;
    int *input2 = &y;
    int z = 1;
    int *throwaway = &z;

    fgets(line, MAX_LENGTH, in);

    // Get the number of vertexes from the file
    if(1 != sscanf(line, "%d %d\n", input1, input2)){
        fprintf(stderr,"%s is in incorrect format\n", argv[1]);
        exit(1);
    }

    Graph G = newGraph(*input1);

    // Add arcs to the graph until the dummy line is found
    while(true){
        fgets(line, MAX_LENGTH, in);
        errno = 0;
        if(2 != sscanf(line, "%d %d %d\n", input1, input2, throwaway)){
            fprintf(stderr,"%s is in incorrect format\n", argv[1]);
            exit(1);
        }else if (errno != 0){
            fprintf(stderr,"%s is in incorrect format\n", argv[1]);
            exit(1);
        }
        
        if(*input1 == 0 && *input2 == 0){
            break;
        }
        addArc(G, *input1, *input2);
    }
    // New pa3 code starts here
    // Print out the graph
    fprintf(out, "Adjacency list representation of G:\n");
    printGraph(out, G);

    // Create initial dfs order
    List L = newList();

    for(int i = 1; i < getOrder(G) + 1; i++){
        append(L, i);
    }

    // Run DFS on the graph, then on the transpose
    DFS(G, L);

    Graph tran = transpose(G);

    DFS(tran, L);

    int numOfComponents = 0;
    moveFront(L);
    // Count the number of components
    while(index(L) != -1){
        int current = get(L);
        if(getParent(tran, current) == NIL){
            numOfComponents++;
        }
        moveNext(L);
    }

    fprintf(out, "\nG contains %d strongly connected components:", numOfComponents);

    numOfComponents = 0;
    List order = newList();
    moveBack(L);
    // Print out the strongly connected components as they are found
    while(index(L) != -1){
        int current = get(L);
        prepend(order, current);
        if(getParent(tran, current) == NIL){
            numOfComponents++;
            fprintf(out, "\nComponent %d: ", numOfComponents);
            printList(out, order);
            clear(order);
        }
        movePrev(L);
    }

    fprintf(out, "\n");

    freeGraph(&G);
    freeList(&L);
    freeList(&order);
    return 0;
}