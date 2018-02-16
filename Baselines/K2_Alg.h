#ifndef K2_ALG_H
#define K2_ALG_H

#include "graph.h"
#include "K_Alg.h"
#include "linkList2.h"
#include "linkList.h"
#include "bool.h"

/********************* FUNCTIONS *****************************************/
int addOPVertices(Vertex* G, int size, K_VertInfo* info, int* R, R_Index* idx, int idxSize, int* component);
int PT_2006(Vertex* G, int size, int save);
int M_MIPS_II(Vertex* G, int size, int save);
int M_MIPS_III(Vertex* G, int size, int save);
int growPalmTrees(Vertex* G, int size, K_VertInfo* info, int* R, int* component, R_Index* idx, int idxSize);
int growPalmTreesII(Vertex* G, int size, K_VertInfo* info, int* R, int* component, R_Index* idx, int idxSize);
int growPalmTreesIII(Vertex* G, int size, K_VertInfo* info, int* R, int* component, R_Index* idx, int idxSize);
int growPalmTreesIV(Vertex* G, int size, K_VertInfo* info, int* R, int* component, R_Index* idx, int idxSize);
int selectTriangleVertex(Vertex* G, K_VertInfo* info, int* R, R_Index* idx, int idxSize);
int selectLargeTriangleVertex(Vertex* G, K_VertInfo* info, int* R, R_Index* idx, int idxSize, int* component);
int closePaths(Vertex* G, K_VertInfo* info, int* R,  R_Index* idx, int idxSize, int* component);
void findTwinNeighbours(Vertex* G, int v, int* w1, int* w2);
int neighbours(Vertex* G, int w1, int w2);
void markAsNonTree_K(Vertex* G, K_VertInfo* info, int v);
int numberOfNeighbours(Vertex* G, K_VertInfo* info, int v);
int vertDegreeGreaterThanTwo(Vertex* G, int w1,int w2);
void updateCompDetails(Vertex* G, K_VertInfo*  info, int v, int comp);

void removeFromKTree(Vertex* G, K_VertInfo* info, int* comp, int v);
void add2_KTree(Vertex* G, K_VertInfo* info, int* component, int v);
void add2_Non_KTree(Vertex* G, K_VertInfo* info, int* component, int v);
int addable_Vert(Vertex* G, K_VertInfo* info, int v, int* component, int* R, R_Index* idx, int idxSize);
int vertDegreeGreaterThanTwo_NonTree(Vertex* G, K_VertInfo* info,  int w1, int w2);
int find_KPath(Vertex* G, List* Path, K_VertInfo* info,int w1, int w2);
void remove_KVertex(Vertex* G, K_VertInfo* info, int* comp, int v);
void alterToTree(Vertex* G, K_VertInfo* info, int v);
void add_KVertex(Vertex* G, K_VertInfo* info, int* comp, int v);
void infoUpdate(Vertex* G, K_VertInfo* info, int v, int comp, int* edges, int* vertices);
void updateR(int* R, R_Index* idx, int idxSize,int v);
void getSmallCompList(List* L, K_VertInfo* info, int size);
void getSize2CompList(List* L, K_VertInfo* info, int size);
int mergeSmallComponents(Vertex* G, int size, K_VertInfo* info, int* R, int* comp, R_Index* idx, int idxSize, List* L);
int allMultiNeighboursInSmallComponents(Vertex* G,int v, List* L, K_VertInfo* info);
int addOneNeighbourInEachCompVertices(Vertex* G, int size, K_VertInfo* info, int* R, R_Index* idx, int idxSize, int* component);
void addTree2NonTree(Vertex* G, int size, K_VertInfo* info, int v, int compNum);
void removeFromNonP(Vertex* G, K_VertInfo* info, int v, int compNum, int type);
void updateNonTree(Vertex* G,int size, K_VertInfo* info, int v, int compNum);
int checkUniquePaths(Vertex* G, int size, K_VertInfo* info, int v);
int checkUniquePathComp(Vertex* G, int size, K_VertInfo* info, int v, int compNum);
int findKPath(Vertex* G, int size, K_VertInfo* info, int w1, int w2, List* Path);
int findAPath(Vertex* G, int size, K_VertInfo* info, int w1,  List* Path, List* Path2);
int uniqueKPath(Vertex* G, int size, K_VertInfo* info, int w1, int w2, List* Path);
int obliterateSmallComponents(Vertex* G, int size, K_VertInfo* info,  int* component, int* last, int* changed);
int obliterateSmallComponents2(Vertex* G, int size, K_VertInfo* info,  int* component, int* last, int* changed);
//int obliterateSmallComponents(Vertex* G, int size, K_VertInfo* info, int* R, R_Index* idx, int idxSize, int* component);
int Nf_Comp_Small(Vertex* G, int size, K_VertInfo* info, int v, List* L);
int locateDivergentPath2(Vertex* G, int size, K_VertInfo* info, int w1, int w2, List* Path);
int locateDivergentVertex2(Vertex* G, int size, K_VertInfo* info, int v);
int locateDivergentPath(Vertex* G, int size, K_VertInfo* info, int w1, int w2, List* Path);
int locateDivergentVertex(Vertex* G, int size, K_VertInfo* info, int v);
void removeFromP(Vertex* G, K_VertInfo* info, int v , int* comp);
int countEdges(Vertex* G, int size, K_VertInfo* info, int comp, int* num);
void alterCompType(Vertex* G,  K_VertInfo* info,  int v);
void updateRemoval(Vertex* G,  K_VertInfo* info, int v, int num, int* edges, int* vertices);

#endif
