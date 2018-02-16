#ifndef TREE_H
#define TREE_H

#include "addVertexAlg.h"
#include "graph.h"

int growSingleTreeAlg(Vertex* G, int size, int save);
int growTreeAlg(Vertex* G, int size, int save);
int growSingletons(Vertex* G, int size, VertInfo* info);
int extendTree(Vertex* G, int size, int v,  VertInfo* info, int num);
int growTrees(Vertex* G, int size,  VertInfo* info, int *num);

#endif
