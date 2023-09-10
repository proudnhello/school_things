//Moore Macauley
//mmacule
//pa3

#include "List.h"
#include "Graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

int time;

typedef struct GraphObj
{
    List* neighbors;
    int* colors;
    int* parents;
    int* discoverTime;
    int* finishTime;

    int order; //Number of Verticies
    int size; //Number of Edges
}GraphObj;

//Creates a new, empty Graph
Graph newGraph(int n){
    Graph new = malloc(sizeof(GraphObj));
    assert(new != NULL);
    new->order = n;
    new->size = 0;

    new->colors = calloc(n+1, sizeof(int));
    new->parents = calloc(n+1, sizeof(int));
    new->discoverTime = calloc(n+1, sizeof(int));
    new->finishTime = calloc(n+1, sizeof(int));

    assert(new->colors != NULL);
    assert(new->parents != NULL);
    assert(new->discoverTime != NULL);
    assert(new->finishTime != NULL);

    new->neighbors = calloc(n+1, sizeof(List));
    assert(new->neighbors != NULL);
    for(int i = 0; i < n+1; i++){
        new->neighbors[i] = newList();
        new->discoverTime[i] = UNDEF;
        new->finishTime[i] = UNDEF;
        new->parents[i] = NIL;
    }

    return new;

}

//Frees a Graph
void freeGraph(Graph* pG){
    if(pG == NULL || *pG == NULL){
        return;
    }

    Graph gone = *pG;

    for(int i = 0; i < gone->order+1; i++){
        freeList(&(gone->neighbors[i]));
    }

    free(gone->neighbors);
    free(gone->colors);
    free(gone->parents);
    free(gone->discoverTime);
    free(gone->finishTime);

    free(gone);
    *pG = NULL;
}

// Returns true if the vertex is valid
bool validVertex(Graph G, int u){
    if(u <= getOrder(G) && u >= 1){
        return true;
    }
    return false;
}

// Returns false if an arc already exists 
bool arcExists(Graph G, int u, int v){
    List working = G->neighbors[u];

    moveFront(working);
    while(index(working) != -1){
        if(get(working) == v){
            return true;
        }
        moveNext(working);
    }
    return false;
}

// Returns the order, or number of vertecies
int getOrder(Graph G){
    if(G == NULL){
        fprintf(stderr, "Graph Error: Calling getOrder() on a NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }
    return G->order;
}

// Returns size, or number of edges
int getSize(Graph G){
    if(G == NULL){
        fprintf(stderr, "Graph Error: Calling getSize() on a NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }
    return G->size;
}

// Returns the parent of node u for the most recent dfs call
int getParent(Graph G, int u){
    if(G == NULL){
        fprintf(stderr, "Graph Error: Calling getParent() on a NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }
    if(!validVertex(G, u)){
        fprintf(stderr, "Graph Error: Calling getParent() on an invalid vertex\n");
        exit(EXIT_FAILURE);
    }
    return G->parents[u];
}

// Returns the discover time of node u for the most recent dfs call
int getDiscover(Graph G, int u){
    if(G == NULL){
        fprintf(stderr, "Graph Error: Calling getDiscover() on a NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }
    if(!validVertex(G, u)){
        fprintf(stderr, "Graph Error: Calling getDiscover() on an invalid vertex\n");
        exit(EXIT_FAILURE);
    }
    return G->discoverTime[u];
}

// Returns the finish time of node u for the most recent dfs call
int getFinish(Graph G, int u){
    if(G == NULL){
        fprintf(stderr, "Graph Error: Calling getFinish() on a NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }
    if(!validVertex(G, u)){
        fprintf(stderr, "Graph Error: Calling getFinish() on an invalid vertex\n");
        exit(EXIT_FAILURE);
    }
    return G->finishTime[u];
}

// Creates an edge between u and v
void addEdge(Graph G, int u, int v){
    if(G == NULL){
        fprintf(stderr, "Graph Error: Calling addEdge() on a NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }
    if(!validVertex(G, u) || !validVertex(G, v)){
        fprintf(stderr, "Graph Error: Calling addEdge() on an invalid vertex\n");
        exit(EXIT_FAILURE);
    }

    // If the edge doesn't exist already
    if(arcExists(G, u, v) && arcExists(G, v, u)){
        return;
    }

    // Simply add an arc from u to v and another from v to u
    addArc(G, u, v);
    addArc(G, v, u);
    // Then decreace size as each arc increaces size by 1, and the edge should only increace it by 1
    G->size -= 1;
}

// Adds an arc from u to v
void addArc(Graph G, int u, int v){
    if(G == NULL){
        fprintf(stderr, "Graph Error: Calling addArc() on a NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }
    if(!validVertex(G, u) || !validVertex(G, v)){
        fprintf(stderr, "Graph Error: Calling addArc() on an invalid vertex\n");
        exit(EXIT_FAILURE);
    }
    // If the arc doesn't already exist
    if(arcExists(G, u, v)){
        return;
    }

    List working = G->neighbors[u];

    // Iterate over the u's adjacency list
    moveFront(working);
    while(index(working) != -1){
        int temp = get(working);
        if(temp == v){
            return;
        }
        // and insert v in the correct position to keep the list in order
        if(v < temp){
            insertBefore(working, v);
            G->size += 1;
            return;
        }
        moveNext(working);
    }
    // If the position was never found, then v is bigger then all other vertexs currently added, so add v to the end
    append(working, v);
    G->size += 1;
}

// Helper recursive function for dfs
void visit(Graph G, List s, int currentVertex){
    // Store discovery time and updates color
    G->discoverTime[currentVertex] = ++time;
    G->colors[currentVertex] = GRAY;

    // Iterate of the adjList of the current vector, running visit on its neighbors as we discover them
    List adjList = G->neighbors[currentVertex];
    moveFront(adjList);
    while(index(adjList) != -1){
        int nextVertex = get(adjList);
        if(G->colors[nextVertex] == WHITE){
            G->parents[nextVertex] = currentVertex;
            visit(G, s, nextVertex);
        }
        moveNext(adjList);
    }

    // Store finish time, updates color, and adds the vertex to the stack
    G->colors[currentVertex] = BLACK;
    G->finishTime[currentVertex] = ++time;
    prepend(s, currentVertex);
}

// Run dfs on G
void DFS(Graph G, List s){
    if(G == NULL){
        fprintf(stderr, "Graph Error: Calling DFS() on a NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }
    if(s == NULL){
        fprintf(stderr, "Graph Error: Calling DFS() on a NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if(length(s) != getOrder(G)){
        fprintf(stderr, "Graph Error: Calling addEdge() on a NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }

    // Set initial values for dfs
    for(int i = 0; i < G->order + 1; i++){
        G->colors[i] = WHITE;
        G->parents[i] = NIL;
        G->discoverTime[i] = UNDEF;
        G->finishTime[i] = UNDEF;
    }
    
    time = 0;

    // Create a copy of s and clear it, so that the finish order can be returned in s
    List order = copyList(s);
    clear(s);
    
    // Iterate over the passed in list, peforming the actual search on them in order
    moveFront(order);
    while(index(order) != -1){
        int currentVertex = get(order);
        if(G->colors[currentVertex] == WHITE){
            visit(G, s, currentVertex);
        }
        moveNext(order);
    }

    freeList(&order);
}

// Return a version of G where all the arcs have been inverted
Graph transpose(Graph G){
    Graph trans = newGraph(G->order);

    for(int i = 1; i < G->order + 1; i++){
        List current = G->neighbors[i];
        moveFront(current);
        while(index(current) != -1){
            addArc(trans, get(current), i);
            moveNext(current);
        }
    }
    return trans;
}

// Return a copy of G
Graph copyGraph(Graph G){
    Graph copy = newGraph(G->order);
    
    for(int i = 0; i < G->order + 1; i++){
        freeList(&(copy->neighbors[i]));
        copy->neighbors[i] = copyList(G->neighbors[i]);
        copy->colors[i] = G->colors[i];
        copy->parents[i] = G->parents[i];
        copy->discoverTime[i] = G->discoverTime[i];
        copy->finishTime[i] = G->finishTime[i];
    }
    copy->size = G->size;

    return copy;
}

// Print the graph
void printGraph(FILE* out, Graph G){
    if(G == NULL){
        fprintf(stderr, "Graph Error: Calling printGraph() on a NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }
    for(int i = 1; i < G->order + 1; i++){
        fprintf(out, "%d: ", i);
        printList(out, G->neighbors[i]);
        fprintf(out, "\n");
    }
}
