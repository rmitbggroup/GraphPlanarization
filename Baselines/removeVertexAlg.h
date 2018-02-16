#ifndef VERTEX_REMOVE_ALG_H
#define VERTEX_REMOVE_ALG_H
#include "graph.h"
#include "bool.h"

/********************** structures ****************************************/
struct P_InfoRec {
	int inP;	/* TRUE if in P otherwise FALSE */
	int degree;	/* degree in P */
};
typedef struct P_InfoRec P_Info;

struct DegreeInfoRec {
	int vertex; 	/* vertex number */
	int degree; 	/* degree of vertex in P */
};
typedef struct DegreeInfoRec DegreeInfo;

/********************* Function Prototypes ******************************/

int removeVertAlg(Vertex* G, int size, int save);
float calcRho(Vertex* G, int size);
void duplicateGraph(Vertex* G, int size);
void unDuplicateGraph(Vertex* G, int size);
void operation3_2(Vertex* G, int size, int v);
void operation3_3and4(Vertex* G, int size, int cur);
void getAdjacentVert(Vertex* G,int cur,int* v,int* w);
void getReducedGraph(Vertex* G, int size);
int getMaxDegreeVertex(Vertex* G, int size);
void furtherReduceGraph(Vertex* G, int size, List* adjList);

#endif
