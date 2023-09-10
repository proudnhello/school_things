//Moore Macauley
//mmacule
//pa2

#include "Graph.h"
#include "List.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MAX_LENGTH 300

int main(int argc, char const *argv[])
{
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

    // Add edges to the graph until the dummy line is found
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
        addEdge(G, *input1, *input2);
    }

    printGraph(out, G);

    List path = newList();

    // Peform bfs and print the required information on the remaining lines of the imput file
    while(true){
        fgets(line, MAX_LENGTH, in);
        errno = 0;
        if(2 != sscanf(line, "%d %d %d\n", input1, input2, throwaway) || errno != 0){
            fprintf(stderr,"%s is in incorrect format\n", argv[1]);
            exit(1);
        }else if (errno != 0){
            fprintf(stderr,"%s is in incorrect format\n", argv[1]);
            exit(1);
        }
        if(*input1 == 0 && *input2 == 0){
            break;
        }
        BFS(G, *input1);

        getPath(path, G, *input2);

        if(front(path) != NIL){
            fprintf(out, "\nThe distance from %d to %d is %d\nA shortest %d-%d path is: ", *input1, *input2, getDist(G, *input2), *input1, *input2);
            printList(out, path);
            fprintf(out, "\n");
        }else{
            fprintf(out, "\nThe distance from %d to %d is infinity\nNo %d-%d path exists\n", *input1, *input2, *input1, *input2);
        }
        clear(path);        
    }

    freeGraph(&G);
    freeList(&path);

    return 0;

}