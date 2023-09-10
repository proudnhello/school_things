//Moore Macauley
//mmacule
//pa2

#include "List.h"
#include "Graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

typedef struct GraphObj
{
    List* neighbors;
    int* colors;
    int* parents;
    int* distances;

    int order; //Number of Verticies
    int size; //Number of Edges
    int currentSource; //Most recent vertex used for BFS
}GraphObj;


//Creates a new, empty Graph
Graph newGraph(int n){
    Graph new = malloc(sizeof(GraphObj));
    assert(new != NULL);
    new->order = n;
    new->size = 0;
    new->currentSource = NIL;

    new->neighbors = calloc(n+1, sizeof(List));
    assert(new->neighbors != NULL);
    for(int i = 0; i < n+1; i++){
        new->neighbors[i] = newList();
    }

    new->colors = calloc(n+1, sizeof(int));
    new->parents = calloc(n+1, sizeof(int));
    new->distances = calloc(n+1, sizeof(int));
    
    assert(new->colors != NULL);
    assert(new->parents != NULL);
    assert(new->distances != NULL);

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
    free(gone->distances);

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

// Returns the vertex bfs was called on most recently
int getSource(Graph G){
    if(G == NULL){
        fprintf(stderr, "Graph Error: Calling getSource() on a NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }
    return G->currentSource;
}

// Returns the parent of node u for the most recent bfs call
int getParent(Graph G, int u){
    if(G == NULL){
        fprintf(stderr, "Graph Error: Calling getParent() on a NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }
    if(!validVertex(G, u)){
        fprintf(stderr, "Graph Error: Calling getParent() on an invalid vertex\n");
        exit(EXIT_FAILURE);
    }
    if(getSource(G) == NIL){
        return NIL;
    }
    return G->parents[u];
}

// Returns the distance from source to u, for most recent bfs call
int getDist(Graph G, int u){
    if(G == NULL){
        fprintf(stderr, "Graph Error: Calling getDist() on a NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }
    if(!validVertex(G, u)){
        fprintf(stderr, "Graph Error: Calling getDist() on an invalid vertex\n");
        exit(EXIT_FAILURE);
    }
    if(getSource(G) == NIL){
        return INF;
    }
    return G->distances[u];
}

// Appends the path from u to source to L
void getPath(List L, Graph G, int u){
    if(G == NULL){
        fprintf(stderr, "Graph Error: Calling getPath()) on a NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }
    if(!validVertex(G, u)){
        fprintf(stderr, "Graph Error: Calling getPath() on an invalid vertex\n");
        exit(EXIT_FAILURE);
    }
    if(getSource(G) == NIL){
        fprintf(stderr, "Graph Error: Calling getPath() before BFS\n");
        exit(EXIT_FAILURE);
    }

    // If u is the source, we add u
    if(getSource(G) == u){
        append(L, u);
    // If u has no parents, we add NIL
    }else if (G->parents[u] == NIL){
        append(L, NIL);
    // Otherwise, we call getPath on u's parent, which adds the path from the parent to the source, then append u.
    }else{
        getPath(L, G, G->parents[u]);
        append(L, u);
    }
    
}

// Empties the graph
void makeNull(Graph G){
    if(G == NULL){
        fprintf(stderr, "Graph Error: Calling makeNull() on a NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }
    for(int i = 0; i < G->order+1; i++){
        clear(G->neighbors[i]);
    }
    G->size = 0;
    G->currentSource = NIL;
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

// Perform bfs on the graph, with source of s
void BFS(Graph G, int s){
    if(G == NULL){
        fprintf(stderr, "Graph Error: Calling BFS() on a NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }
    if(!validVertex(G, s)){
        fprintf(stderr, "Graph Error: Calling BFS() on an invalid vertex\n");
        exit(EXIT_FAILURE);
    }

    G->currentSource = s;

    // Set inital values
    for(int i = 0; i < getOrder(G) + 1; i++){
        G->colors[i] = WHITE;
        G->distances[i] = INF;
        G->parents[i] = NIL;
    }

    G->colors[s] = GRAY;
    G->distances[s] = 0;
    G->parents[s] = NIL;

    // Create a queue, and add s to it
    List q = newList();
    prepend(q, s);

    // While there are values left in the queue
    while (length(q) != 0){
        // Get the next item
        int currentVertex = front(q);
        deleteFront(q);
        List inUse = G->neighbors[currentVertex];
        moveFront(inUse);
        // Iterate over its adjacency list
        while(index(inUse) != -1){
            int nextVertex = get(inUse);
            // If a vertex is undiscovered, discover it
            if(G->colors[nextVertex] == WHITE){
                G->colors[nextVertex] = GRAY;
                G->distances[nextVertex] = G->distances[currentVertex] + 1;
                G->parents[nextVertex] = currentVertex;
                append(q, nextVertex);
            }
            moveNext(inUse);
        }
        G->colors[currentVertex] = BLACK;
    }

    freeList(&q);
    
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
