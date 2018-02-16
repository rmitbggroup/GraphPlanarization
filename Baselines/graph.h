#ifndef	GRAPH_H
#define	GRAPH_H
#include "linkList.h"
#include "bool.h"

/*************** constants **********************************************/
#define UNKNOWN 0
#define KNOWN 1

/*************** Structures **********************************************/
struct VertexRec{
	List adjVertices;
	int* adjVerts;  
	int member;	
	int degree;
};

typedef  struct VertexRec Vertex;


/*************** Function Prototypes ************************************/
Vertex* readGraph(int* size, int* d);
Vertex* initGraph(int size);
void killGraph(Vertex* G, int size);
void printGraph(Vertex* G, int size);
int addEdge(Vertex* G, int size, int v, int w);
int edgeExists(Vertex* G, int size, int v, int w);
void initMembers(Vertex* G, int size);
int validEdge(Vertex* G, int size, int v, int w);
int validateGraph(Vertex* G, int size);
int addEdge2(Vertex* G, int size, int v, int w);
int edgeExists2(Vertex* G, int size, int v, int w);
void saveGraph(Vertex* G, int size, int edges);
void writePlanarSubgraph(Vertex* G, int size);
void initAdjLists(Vertex* G, int size);
Vertex* readGraphFromFile(char* filename, int size, int d);


#endif
