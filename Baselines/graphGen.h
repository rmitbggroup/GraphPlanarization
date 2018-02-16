#ifndef GRAPH_GEN_H
#define GRAPH_GEN_H
#include "graph.h"

/**************** structures ***********************************************/
struct edgeRec{
	int v;
	int w;
};
typedef struct edgeRec Edge;

/**************** function prototypes *************************************/
Vertex* genGraph(int size, int d);
double update_r(Vertex* G, int size, int d, float* r, int v, int w, double sum);
void update_A(float* A, float* r, float R, int size);
int get_index(int i, int j);
Vertex* genGraph2(int size, int d);
Edge* initEdges(int size);
void printEdges(Edge* edges, int size);
Vertex* classicalGen(int size, int d);

#endif
