#ifndef K_ALG_H
#define K_ALG_H

#include "graph.h"
#include "linkList2.h"
#include "linkList.h"
#include "bool.h"
#include "addVertexAlg.h"

/*********************** CONSTANTS *****************************************/
#define UNDECIDED 0
#define FOREST 1
#define NON_TREE 2

#define MEGAVERT 1
#define NON_MEGAVERT 0

/*********************** STRUCTURES ***************************************/
struct K_VertInfoRec{
	int inP;
	int type;
	int cycle;		/* member of a cycle */
	int component;
	int Nn;
	int Nf;
	int RG_Index;		/* index into megavertices information */
	//int adjTrees; 	
	//int adjNonTrees; 	
	List2 adjTrees;		/* number of adjacent tree components*/
	List2 adjNonTrees;	/* number of adjacent non-tree components */
};
typedef struct K_VertInfoRec K_VertInfo;

struct R_IndexRec{
	int start;
	int finish;
	int degree;
};
typedef struct R_IndexRec R_Index;

struct RGVertRec{
	int vertex;
	int type; 	/* megavertex or non-megavertex*/
	List	vertList;	/* if this vertex is a megavertex, this list
				   contains a list of vertices 'contained' 
				   within this vertex, 
				   otherwise, it contains a list of megavertices
				   that this vertex is contained in */
};
typedef struct RGVertRec RGVert;

/* an edge between two megavertices occurs when either
 * their was an edge {v1,v2} in the original graph
 * or some vertex v was reduced to both these megavertices*/
struct MVEdgeRec{
	int v1;
	int v2;
};
typedef struct MVEdgeRec MV_Edge;

/********************* FUNCTIONS *****************************************/
int M_MIPS(Vertex* G, int size, int save);
K_VertInfo* initK_Info(Vertex* G, int size);
int growIndpendentSet(Vertex* G, int size, K_VertInfo* info, int* R, int * Rsize);
int growKTrees(Vertex* G, int size, K_VertInfo* info, int* R,  int* component, R_Index* idx, int idxSize);
int pickLowDegVert(Vertex* G, K_VertInfo* info, int* R, int Rsize, int low);
int pickVertex( K_VertInfo* info, int* R, int Rsize, int cur, int* free);
int pickTreeVert( K_VertInfo* info, int* R, int Rsize, int cur, int* free, int* low1);
int pickTreeVert1( K_VertInfo* info, int* R, int size, int cur, R_Index* idx, int idxSize, int set, int low);
void printKInfo(K_VertInfo* info, int size);
int growCleverSingleTreeAlg(Vertex* G, int size, int save);
void sort(Vertex* G, int* R, int size);
void mergeSort(Vertex* G, int* R, int size, int* R2);
void merge(Vertex* G, int* R, int Rsize, int* R2, int R2size, int* temp);
int getAdjTreeVertex(Vertex* G, int size, K_VertInfo* info, int v);
int getAdjVertex(Vertex* G, int size, K_VertInfo* info, int v);
void updateTree(Vertex* G, int size, K_VertInfo* info, int v, int compNum);
R_Index* createIndex(Vertex* G, int* R, int size, int* idxSize);
int growOuterplanar(Vertex* G, int size, K_VertInfo* info, int* R, R_Index* idx,int idxSize, int P, int* component);
int numCycleNeighbours(Vertex* G,  K_VertInfo* info, int v);
void addToComponent(Vertex* G, int v, K_VertInfo* info, int component);
int pickOPVertex(Vertex* G, K_VertInfo* info,int* R, int cur, R_Index* idx,int idxSize,int set,int* low);     
int checkAdjComp(K_VertInfo* info, int v);
int pathFinder(Vertex* G, K_VertInfo* info, List* Path, int x, int y, int c);
int legalOP_Add(Vertex* G, K_VertInfo* info, int v);    
void getAdjacentPair(Vertex* G, K_VertInfo* info, int v, int* x, int* y, int comp);      
void markAsCircuits(Vertex* G, K_VertInfo* info, int v);
int GrowCleverTrees(Vertex* G, int size, int save);
RGVert*  createReducedGraph(Vertex* G, int size, int Psize, K_VertInfo* info);
RGVert* createMegaVerts(K_VertInfo* info, int size, int Psize);
int extendGraph(Vertex* G, int size, K_VertInfo* info, int* R, R_Index* idx,int idxSize, int P, int* component);
void initReducedGraph(Vertex* G, int size);
void reduceGraph(Vertex* G, int size, K_VertInfo* info, RGVert* MegaVerts);
void reduceOp1(Vertex* G, int size, K_VertInfo* info, RGVert* MegaVerts, int v, int w);
void reduceOp2(Vertex* G, int size, K_VertInfo* info, RGVert* MegaVerts, int v, int w, int x);
void unInitReducedGraph(Vertex* G,int size);
void printMegaList(RGVert* megaVert, int size);
int locateMegaVertices(Vertex* G, K_VertInfo* info,  int v, RGVert* megaVert, List* L, int compNum, MV_Edge* E);
int findVert2Add(Vertex* G, K_VertInfo* info,int* R, int cur, R_Index* idx,int idxSize,int set,int* low, RGVert* megaVerts, int mSize);
int checkAdd(Vertex* G, K_VertInfo* info, RGVert* megaVerts, int  mSize, int v);
int checkCompAdd(Vertex* G, K_VertInfo* info, RGVert* megaVerts, int  mSize, int v, int comp);
int findValidPath(Vertex* G, RGVert* megaVerts, List* L);
int findValidPathII(Vertex* G, RGVert* megaVerts, List* L, MV_Edge* E, int size);
int growPath(List* L, List* L1,Vertex* G,  int v);
int  findAdjVert(Vertex* G, List* L, int v);
int mergeSubPaths(List* Paths, Vertex* G, int size);
int getCommonNeighbour(Vertex* G, int v, int w, List* Paths, int size);
int findVertWithLeastNeighbours(List* L, Vertex* G);
void makePath(Vertex* G, List* L, int v, List* Path);
void extendFromEnd(Vertex* G, List* L, int v, List* Path);
void extendFromStart(Vertex* G, List* L, int v, List* Path);
int CreateOPGraph(Vertex* G, int size,  int save);
int evaluatePath(Vertex* G, List* Paths);
int validSequence(int sequence[], int size);
int checkAdjacent(Vertex* G, int w, int v);
void initEdgeList(MV_Edge* E, int size);
int getMVEdge(List* L, MV_Edge* E);
int MVEdge_exists(MV_Edge* E, MV_Edge edge, int size);
void printMVEdges(MV_Edge* E, int size);
int removeEdges(MV_Edge* E, int size, int v1, int v2, int v3, int num);
int makePathII(Vertex* G, List* L, List* Path, MV_Edge* E, int* size);
int findLowestDegreeNeighbourInList(Vertex* G, int v, List* L);
void insertDeg2VertInPath(Vertex* G, List* Path);
int sharedNeighbour(Vertex* G, int v, int w);


#endif
