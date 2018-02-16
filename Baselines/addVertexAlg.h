#ifndef ADD_VERT_H
#define ADD_VERT_H

#include "graph.h"
#include "bool.h"

/********************** CONSTANTS ****************************************/
#define UNDECIDED 0
#define FOREST 1
#define NON_TREE 2

/********************** structures ****************************************/
struct VertInfoRec {
	int inP;	/* TRUE if in P otherwise FALSE */
	int type;	/* TREE, NON_TREE, UNDECIDED */
	int component;  /* component number */
	int Nn;		/* non-tree neighbours in P */
	int Nf; 	/* forest neighbours in P */
};
 
typedef struct VertInfoRec VertInfo;
 
/********************* Function Prototypes ******************************/
int addVertAlg(Vertex* G, int size, int save);
int selectVertexForRemoval(VertInfo* I, int size, int* R, int cur);
VertInfo* initInfo(int size);
int action1(Vertex* G, int size, VertInfo* info,int v, int compNum);
int action2(Vertex* G, int size, VertInfo* info, int x, int compNum);
int action3(Vertex* G, int size, VertInfo* info, int v, int compNum, int x, int y, int* removed,double* timeForPaths);
void getNFNeighbours(Vertex* G, int size, VertInfo* info, int v, int* x, int* y);
void markAsTree(Vertex* G, VertInfo* info, int v, int num);
void incNF(Vertex* G, VertInfo* info, int v);
void decNF(Vertex* G, VertInfo* info, int v);
void incNn(Vertex* G, VertInfo* info, int v);
void decNn(Vertex* G, VertInfo* info, int v);
void printInfo(VertInfo* info, int size);
void markAsNonTree(Vertex* G, VertInfo* info, int v, int num);
int crossroads(Vertex* G, int size, VertInfo* info, int y, int z);
int adjacentVertices(Vertex* G, int v, int w);
int existsPath(Vertex* G, int size, VertInfo* info, int x, int y);   
int getInitPath(Vertex* G, int size, VertInfo* info, List* Path, int x, int y);
void updateInfo(Vertex* G, VertInfo* info, int v, int num, int* edges, int* vertices);    
void changeToTree(Vertex* G, VertInfo* info, int v);
int countAdjTrees(Vertex* G, VertInfo* info, int v);
int* initR(int size);

#endif
