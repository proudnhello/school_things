//Moore Macauley
//mmacule
//pa3

#ifndef GRAPH
#define GRAPH

#define UNDEF -1
#define NIL 0
#define BLACK 0
#define GRAY 1
#define WHITE 2

#include "List.h"
#include <stdio.h>

typedef struct GraphObj* Graph;

Graph newGraph(int n);
void freeGraph(Graph* pG);

int getOrder(Graph G);
int getSize(Graph G);
int getParent(Graph G, int u);
int getDiscover(Graph G, int u);
int getFinish(Graph G, int u);

void addEdge(Graph G, int u, int v);
void addArc(Graph G, int u, int v);
void DFS(Graph G, List s);

Graph transpose(Graph G);
Graph copyGraph(Graph G);
void printGraph(FILE* out, Graph G);

#endif
