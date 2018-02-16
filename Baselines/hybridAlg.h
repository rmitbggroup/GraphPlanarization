#ifndef HYBRID_H
#define HYBRID_H

#include "graph.h"
#include "addVertexAlg.h"

#define ADDING 1
#define REMOVING 2
#define FINISH 0

int hybrid(Vertex* G, int size, int save);
int removeVertex(Vertex* G, int size, VertInfo* info, int v, int component);
int selectReplacementVert(Vertex* G, VertInfo* info, int size, int* R, int vx);
#endif
